#pragma once
#include"ilcplex/ilocplex.h";
#include<chrono>;
typedef IloArray<IloNumVarArray> NumVar2D; // enables us to defien a 2-D decision varialbe
typedef IloArray<NumVar2D> NumVar3D;

class Cplex
{
public:
	Cplex(string** PathsthatContainLeg, int* numPathsthatContainLeg, int numAirCrafts, int numPaths, int numLegs, int numFareClasses, int numOfLegsOfTypeO, int numOfLegsOfTypeM, int numOfPathsWithLegO, int numOfPathsWithLegM, Leg* Legs, Path* Paths, AirCraft* AirCrafts, string* FareClasses, Path* pathsWithTypeM, Path* pathsWithTypeO, Leg* legsOfTypeM, Leg* legsOfTypeO)
		: PathsthatContainLeg(PathsthatContainLeg),
		numPathsthatContainLeg(numPathsthatContainLeg),
		numberofAirCrafts(numAirCrafts),
		numberOfPaths(numPaths),
		numberOfLegs(numLegs),
		numberOfFareClasses(numFareClasses),
		numOfLegsOfTypeO(numOfLegsOfTypeO),
		numOfLegsOfTypeM(numOfLegsOfTypeM),
		numOfPathsWithLegO(numOfPathsWithLegO),
		numOfPathsWithLegM(numOfPathsWithLegM),
		Legs(Legs),
		Paths(Paths),
		AirCrafts(AirCrafts),
		FareClasses(FareClasses),
		pathsWithTypeM(pathsWithTypeM),
		pathsWithTypeO(pathsWithTypeO),
		legsOfTypeM(legsOfTypeM),
		legsOfTypeO(legsOfTypeO){}
	bool System() {
		auto start = chrono::high_resolution_clock::now();
		IloEnv env;
		IloModel Model(env);
#pragma region Define decision variable
		NumVar2D X(env, numberofAirCrafts);
		IloNumVarArray Z(env, numberOfPaths, 0, IloInfinity, ILOINT);
		NumVar2D P(env, numberOfPaths);

		for (int s = 0; s < numberofAirCrafts; s++)
		{
			X[s] = IloNumVarArray(env, numberOfLegs, 0, IloInfinity, ILOINT);
		}
		for (int s = 0; s < numberOfPaths; s++)
		{
			P[s] = IloNumVarArray(env, numberOfFareClasses, 0, IloInfinity, ILOINT);
		}

#pragma endregion

#pragma region Objective Function

		IloExpr exp0(env);

		for (int i = 0; i < numberOfPaths; i++)
		{
			for (int j = 0; j < numberOfFareClasses; j++) {
				exp0 += (Paths[i].getPriceFromFareClass(FareClasses[j]) * P[i][j]);
			}
		}
		for (int i = 0; i < numberofAirCrafts; i++)
		{
			for (int x = 0; x < numberOfLegs; x++)
			{
				exp0 -= (AirCrafts[i].getCostFromLeg(Legs[x].getFrom(), Legs[x].getTo()) * X[i][x]);
			}
		}

		Model.add(IloMaximize(env, exp0));

#pragma endregion
#pragma region Constraints
		//Constrain1
		string Constrain1 = readFromFile();
		int countConstrain1 = countLines(Constrain1);
		char operation = '+';
		int i = 0;
		string Condition;
		IloExpr exp1(env);
		IloExpr exp2(env);
		IloExpr exp3(env);
		while(i < countConstrain1) {
			string MinMax=Constrain1.substr(0,3);
			switch (operation)
			{
			case '+':
			{
				if (Condition.empty()) {
					if (isIntegerExpression(Constrain1[0])) {
						string number = extractNumber(Constrain1);
						int value = stringToInt(number);
						exp1 += value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain1.erase(0, 4);
						exp1 += MinMaxValue(Constrain1.substr(0, Constrain1.find('}')+1), MinMax);
						Constrain1.erase(0, Constrain1.find_first_of('}') + 1);
					}
					else if (Constrain1[0] == '|') {
						Constrain1.erase(0, 1);
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "L") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp1+=Paths[x].getLegCount();

									break;
								}
							}
						}
					}
					else {

						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "Cap")
						{
							string AirCraftType, FareClassName;
							extract1D(Expression, AirCraftType, FareClassName);
							for (int x = 0; x < numberofAirCrafts; x++) {
								for (int y = 0; y < numberOfFareClasses; y++)
								{
									if (AirCrafts[x].getType() == AirCraftType && FareClasses[y] == FareClassName) {
										exp1 += AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "X")
						{
							string From, To, AirCraftName;
							extract2D1To2(Expression, AirCraftName, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberofAirCrafts; y++) {
									if (AirCrafts[y].getType() == AirCraftName && Legs[x].getFrom() == From && Legs[x].getTo() == To) {
										exp1 += X[y][x];
										y = numberofAirCrafts;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}
						else if(operationType == "Z") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp1 += Z[x];
									x = numberOfPaths;
								}
							}
						}
						else if(operationType == "P") {
							string From, To, FareClassName;
							extract2D2To1(Expression, FareClassName, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp1 += P[x][y];
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if(operationType == "M") {
							string From, To, FareClassName;
							extract2D2To1(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp1 += Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
					}
				}
				else
				{
					if (isIntegerExpression(Constrain1[0])) {
						string number = extractNumber(Constrain1);
						int value = stringToInt(number);
						exp2 += value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain1.erase(0, 4);
						exp2 += MinMaxValue(Constrain1.substr(0, Constrain1.find('}')+1), MinMax);
						Constrain1.erase(0, Constrain1.find_first_of('}') + 1);
					}
					else if (Constrain1[0] == '|') {
						Constrain1.erase(0, 1);
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "L") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp2+= Paths[x].getLegCount();
									Constrain1.erase(0, 1);
									break;
								}
							}
						}
					}
					else {
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "Cap")
						{
							string AirCraftType, FareClassName;
							extract1D(Expression, AirCraftType, FareClassName);
							for (int x = 0; x < numberofAirCrafts; x++) {
								for (int y = 0; y < numberOfFareClasses; y++)
								{
									if (AirCrafts[x].getType() == AirCraftType && FareClasses[y] == FareClassName) {
										exp2 += AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "X")
						{
							string From, To, AirCraftName;
							extract2D1To2(Expression, AirCraftName, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberofAirCrafts; y++) {
									if (AirCrafts[y].getType() == AirCraftName && Legs[x].getFrom() == From && Legs[x].getTo() == To) {
										exp2 += X[y][x];
										y = numberofAirCrafts;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}
						else if (operationType == "Z") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp2 += Z[x];
									x = numberOfPaths;
								}
							}
						}
						else if (operationType == "P") {
							string From, To, FareClassName;
							extract2D2To1(Expression, FareClassName, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2 += P[x][y];
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extract2D2To1(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2 += Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
					}
				}
				//Check If end line
				if (Constrain1[0] == '\n') {
					if (Condition == "=") {
						Model.add(exp1 == exp2);
					}
					else if (Condition == "<=") {
						Model.add(exp1 <= exp2);
					}
					else if (Condition == ">=") {
						Model.add(exp1 >= exp2);
					}
					else if (Condition == "<") {
						Model.add(exp1 < exp2);
					}
					else if (Condition == ">") {
						Model.add(exp1 > exp2);
					}
					operation = '+';
					Condition.clear();
					i++;
					exp1 = exp3;
					exp2 = exp3;
				}
				else if (Constrain1[0] != '<' && Constrain1[0] != '=' && Constrain1[0] != '>' && Constrain1[0] + Constrain1[1] != '<=' && Constrain1[0] + Constrain1[1] != '>=')
					operation = Constrain1[0];
				else if (Constrain1.substr(0,2) == "<=" || Constrain1.substr(0, 2) == ">=") {
					operation = '+';
					Condition = Constrain1.substr(0, 2);
					Constrain1.erase(0, 1);
				}
				else
				{
					operation = '+';
					Condition = Constrain1[0];
				}
				Constrain1.erase(0, 1);
				break;
			}
			case '-':
			{
				if (Condition.empty()) {
					if (isIntegerExpression(Constrain1[0])) {
						string number = extractNumber(Constrain1);
						int value = stringToInt(number);
						exp1 -= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain1.erase(0, 4);
						exp1 -= MinMaxValue(Constrain1.substr(0, Constrain1.find('}')+1), MinMax);
						Constrain1.erase(0, Constrain1.find_first_of('}') + 1);
					}
					else if (Constrain1[0] == '|') {
						Constrain1.erase(0, 1);
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "L") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp1 -= Paths[x].getLegCount();

									break;
								}
							}
						}
					}
					else {

						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "Cap")
						{
							string AirCraftType, FareClassName;
							extract1D(Expression, AirCraftType, FareClassName);
							for (int x = 0; x < numberofAirCrafts; x++) {
								for (int y = 0; y < numberOfFareClasses; y++)
								{
									if (AirCrafts[x].getType() == AirCraftType && FareClasses[y] == FareClassName) {
										exp1 -= AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "X")
						{
							string From, To, AirCraftName;
							extract2D1To2(Expression, AirCraftName, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberofAirCrafts; y++) {
									if (AirCrafts[y].getType() == AirCraftName && Legs[x].getFrom() == From && Legs[x].getTo() == To) {
										exp1 -= X[y][x];
										y = numberofAirCrafts;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}
						else if (operationType == "Z") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp1 -= Z[x];
									x = numberOfPaths;
								}
							}
						}
						else if (operationType == "P") {
							string From, To, FareClassName;
							extract2D2To1(Expression, FareClassName, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp1 -= P[x][y];
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extract2D2To1(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp1 -= Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
					}
				}
				else
				{
					if (isIntegerExpression(Constrain1[0])) {
						string number = extractNumber(Constrain1);
						int value = stringToInt(number);
						exp2 -= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain1.erase(0, 4);
						exp2 -= MinMaxValue(Constrain1.substr(0, Constrain1.find('}')+1), MinMax);
						Constrain1.erase(0, Constrain1.find_first_of('}') + 1);
					}
					else if (Constrain1[0] == '|') {
						Constrain1.erase(0, 1);
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "L") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp2 -= Paths[x].getLegCount();
									Constrain1.erase(0, 1);
									break;
								}
							}
						}
					}
					else {
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "Cap")
						{
							string AirCraftType, FareClassName;
							extract1D(Expression, AirCraftType, FareClassName);
							for (int x = 0; x < numberofAirCrafts; x++) {
								for (int y = 0; y < numberOfFareClasses; y++)
								{
									if (AirCrafts[x].getType() == AirCraftType && FareClasses[y] == FareClassName) {
										exp2 -= AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "X")
						{
							string From, To, AirCraftName;
							extract2D1To2(Expression, AirCraftName, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberofAirCrafts; y++) {
									if (AirCrafts[y].getType() == AirCraftName && Legs[x].getFrom() == From && Legs[x].getTo() == To) {
										exp2 -= X[y][x];
										y = numberofAirCrafts;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}
						else if (operationType == "Z") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp2 -= Z[x];
									x = numberOfPaths;
								}
							}
						}
						else if (operationType == "P") {
							string From, To, FareClassName;
							extract2D2To1(Expression, FareClassName, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2 -= P[x][y];
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extract2D2To1(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2 -= Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
					}
				}
				//Check If end line
				if (Constrain1[0] == '\n') {
					if (Condition == "=") {
						Model.add(exp1 == exp2);
					}
					else if (Condition == "<=") {
						Model.add(exp1 <= exp2);
					}
					else if (Condition == ">=") {
						Model.add(exp1 >= exp2);
					}
					else if (Condition == "<") {
						Model.add(exp1 < exp2);
					}
					else if (Condition == ">") {
						Model.add(exp1 > exp2);
					}
					operation = '+';
					Condition.clear();
					i++;
					exp1 = exp3;
					exp2 = exp3;

				}
				else if (Constrain1[0] != '<' && Constrain1[0] != '=' && Constrain1[0] != '>' && Constrain1[0] + Constrain1[1] != '<=' && Constrain1[0] + Constrain1[1] != '>=')
					operation = Constrain1[0];
				else if (Constrain1.substr(0, 2) == "<=" || Constrain1.substr(0, 2) == ">=") {
					operation = '+';
					Condition = Constrain1.substr(0, 2);
					Constrain1.erase(0, 1);
				}
				else
				{
					operation = '+';
					Condition = Constrain1[0];
				}
				Constrain1.erase(0, 1);
				break;
			}
			case '*':
			{
				if (Condition.empty()) {
					if (isIntegerExpression(Constrain1[0])) {
						string number = extractNumber(Constrain1);
						int value = stringToInt(number);
						exp1 *= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain1.erase(0, 4);
						exp1 *= MinMaxValue(Constrain1.substr(0, Constrain1.find('}')+1), MinMax);
						Constrain1.erase(0, Constrain1.find_first_of('}') + 1);
					}
					else if (Constrain1[0] == '|') {
						Constrain1.erase(0, 1);
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "L") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp1 *= Paths[x].getLegCount();

									break;
								}
							}
						}
					}
					else {

						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "Cap")
						{
							string AirCraftType, FareClassName;
							extract1D(Expression, AirCraftType, FareClassName);
							for (int x = 0; x < numberofAirCrafts; x++) {
								for (int y = 0; y < numberOfFareClasses; y++)
								{
									if (AirCrafts[x].getType() == AirCraftType && FareClasses[y] == FareClassName) {
										exp1 *= AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extract2D2To1(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp1 *= Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
					}
				}
				else
				{
					if (isIntegerExpression(Constrain1[0])) {
						string number = extractNumber(Constrain1);
						int value = stringToInt(number);
						exp2 *= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain1.erase(0, 4);
						exp2 *= MinMaxValue(Constrain1.substr(0, Constrain1.find('}')+1), MinMax);
						Constrain1.erase(0, Constrain1.find_first_of('}') + 1);
					}
					else if (Constrain1[0] == '|') {
						Constrain1.erase(0, 1);
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "L") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp2 *= Paths[x].getLegCount();
									Constrain1.erase(0, 1);
									break;
								}
							}
						}
					}
					else {
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "Cap")
						{
							string AirCraftType, FareClassName;
							extract1D(Expression, AirCraftType, FareClassName);
							for (int x = 0; x < numberofAirCrafts; x++) {
								for (int y = 0; y < numberOfFareClasses; y++)
								{
									if (AirCrafts[x].getType() == AirCraftType && FareClasses[y] == FareClassName) {
										exp2 *= AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extract2D2To1(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2 += Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
					}
				}
				//Check If end line
				if (Constrain1[0] == '\n') {
					if (Condition == "=") {
						Model.add(exp1 == exp2);
					}
					else if (Condition == "<=") {
						Model.add(exp1 <= exp2);
					}
					else if (Condition == ">=") {
						Model.add(exp1 >= exp2);
					}
					else if (Condition == "<") {
						Model.add(exp1 < exp2);
					}
					else if (Condition == ">") {
						Model.add(exp1 > exp2);
					}
					operation = '+';
					Condition.clear();
					i++;
					exp1 = exp3;
					exp2 = exp3;

				}
				else if (Constrain1[0] != '<' && Constrain1[0] != '=' && Constrain1[0] != '>' && Constrain1[0] + Constrain1[1] != '<=' && Constrain1[0] + Constrain1[1] != '>=')
					operation = Constrain1[0];
				else if (Constrain1.substr(0, 2) == "<=" || Constrain1.substr(0, 2) == ">=") {
					operation = '+';
					Condition = Constrain1.substr(0, 2);
					Constrain1.erase(0, 1);
				}
				else
				{
					operation = '+';
					Condition = Constrain1[0];
				}
				Constrain1.erase(0, 1);
				break;
			}
			case '/':
			{
				if (Condition.empty()) {
					if (isIntegerExpression(Constrain1[0])) {
						string number = extractNumber(Constrain1);
						int value = stringToInt(number);
						exp1 /= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain1.erase(0, 4);
						exp1 /= MinMaxValue(Constrain1.substr(0, Constrain1.find('}')+1), MinMax);
						Constrain1.erase(0, Constrain1.find_first_of('}') + 1);
					}
					else if (Constrain1[0] == '|') {
						Constrain1.erase(0, 1);
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "L") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp1 /= Paths[x].getLegCount();

									break;
								}
							}
						}
					}
					else {

						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "Cap")
						{
							string AirCraftType, FareClassName;
							extract1D(Expression, AirCraftType, FareClassName);
							for (int x = 0; x < numberofAirCrafts; x++) {
								for (int y = 0; y < numberOfFareClasses; y++)
								{
									if (AirCrafts[x].getType() == AirCraftType && FareClasses[y] == FareClassName) {
										exp1 /= AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extract2D2To1(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp1 /= Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
					}
				}
				else
				{
					if (isIntegerExpression(Constrain1[0])) {
						string number = extractNumber(Constrain1);
						int value = stringToInt(number);
						exp2 /= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain1.erase(0, 4);
						exp2 /= MinMaxValue(Constrain1.substr(0, Constrain1.find('}')+1), MinMax);
						Constrain1.erase(0, Constrain1.find_first_of('}') + 1);
					}
					else if (Constrain1[0] == '|') {
						Constrain1.erase(0, 1);
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "L") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfPaths; x++) {
								if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									exp2 /= Paths[x].getLegCount();
									Constrain1.erase(0, 1);
									break;
								}
							}
						}
					}
					else {
						string Expression = extractExpression(Constrain1);
						string operationType = extractOperationType(Expression);
						if (operationType == "Cap")
						{
							string AirCraftType, FareClassName;
							extract1D(Expression, AirCraftType, FareClassName);
							for (int x = 0; x < numberofAirCrafts; x++) {
								for (int y = 0; y < numberOfFareClasses; y++)
								{
									if (AirCrafts[x].getType() == AirCraftType && FareClasses[y] == FareClassName) {
										exp2 += AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extract2D2To1(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2 /= Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
					}
				}
				//Check If end line
				if (Constrain1[0] == '\n') {
					if (Condition == "=") {
						Model.add(exp1 == exp2);
					}
					else if (Condition == "<=") {
						Model.add(exp1 <= exp2);
					}
					else if (Condition == ">=") {
						Model.add(exp1 >= exp2);
					}
					else if (Condition == "<") {
						Model.add(exp1 < exp2);
					}
					else if (Condition == ">") {
						Model.add(exp1 > exp2);
					}
					operation = '+';
					Condition.clear();
					i++;
					exp1 = exp3;
					exp2 = exp3;
				}
				else if (Constrain1[0] != '<' && Constrain1[0] != '=' && Constrain1[0] != '>' && Constrain1[0] + Constrain1[1] != '<=' && Constrain1[0] + Constrain1[1] != '>=')
					operation = Constrain1[0];
				else if (Constrain1.substr(0, 2) == "<=" || Constrain1.substr(0, 2) == ">=") {
					operation = '+';
					Condition = Constrain1.substr(0, 2);
					Constrain1.erase(0, 1);
				}
				else
				{
					operation = '+';
					Condition = Constrain1[0];
				}
				Constrain1.erase(0, 1);
				break;
			}
			}
		}
#pragma endregion
		IloCplex cplex(Model);
		cplex.exportModel("model.lp");

		if (!cplex.solve()) {
			env.error() << "Failed to optimize the Master Problem!!!" << endl;
			return false;
		}

		double obj = cplex.getObjValue();

		auto end = chrono::high_resolution_clock::now();
		auto Elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
		cout << "\t Elapsed Time(ms): " << Elapsed.count() << endl;

		cout << "\nThe objective value is: " << obj << endl;
		for (int i = 0; i < numberOfPaths; i++)
		{
			for (int j = 0; j < numberOfFareClasses; j++) {
				double Pval = cplex.getValue(P[i][j]);
				if (Pval > 0)
				{
					cout << "\t\t\t P[" << i << "][" << j << "] = " << Pval << endl;
				}
			}
		}
		for (int i = 0; i < numberOfPaths; i++) {
			double Zval = cplex.getValue(Z[i]);
			if (Zval > 0)
			{
				cout << "\t\t\t Z[" << i << "] = " << Zval << endl;
			}

		}
		for (int i = 0; i < numberofAirCrafts; i++)
		{
			for (int x = 0; x < numberOfLegs; x++)
			{
				double Xval = cplex.getValue(X[i][x]);
				if (Xval > 0)
				{
					cout << "\t\t\t X[" << i << "][" << x << "] = " << Xval << endl;
				}
			}
		}
		return true;
	}
	string readFromFile() {
		ifstream file("Output.txt");
		string fileContents;

		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				fileContents += line + '\n';
			}
			file.close();
		}
		else {
			cerr << "Unable to open file: Output.txt" << endl;
		}

		return fileContents;
	}	
	int countLines(const string& data) {
		int numberOfLines = 0; // Initialize to 1 to account for the last line

		for (char ch : data) {
			if (ch == '\n') {
				numberOfLines++;
			}
		}

		return numberOfLines;
	}
	string extractExpression(string& inputString) {
		// Find the position of '+'
		size_t index1 = inputString.find("]+");
		size_t index2 = inputString.find("]-");
		size_t index3 = inputString.find("]*");
		size_t index4 = inputString.find("]/");
		size_t index5 = inputString.find("]=");
		size_t index6 = inputString.find("]<");
		size_t index7 = inputString.find("]>");
		size_t index8 = inputString.find("]<=");
		size_t index9 = inputString.find("]>=");
		size_t index10 = inputString.find("]\n");
		size_t index11 = inputString.find("]|");
		size_t index12 = inputString.find("],");
		size_t index13 = inputString.find("]}");

		// Find the minimum non-negative index among the three
		size_t plusPos = min({ index1, index2, index3, index4,index5, index6, index7, index8, index9, index10, index11, index12, index13 }) +1;

		// Extract the first expression
		string expression = inputString.substr(0, plusPos);

		// Trim leading and trailing whitespaces
		size_t firstNonSpace = expression.find_first_not_of(" \t");
		size_t lastNonSpace = expression.find_last_not_of(" \t");

		if (firstNonSpace != string::npos && lastNonSpace != string::npos) {
			expression = expression.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
		}

		// Remove the extracted expression from the inputString
		inputString.erase(0, plusPos);

		return expression;
	}
	string extractNumber(string& inputString) {
		// Find the position of '+'
		size_t index1 = inputString.find("+");
		size_t index2 = inputString.find("-");
		size_t index3 = inputString.find("*");
		size_t index4 = inputString.find("/");
		size_t index5 = inputString.find("=");
		size_t index6 = inputString.find("<");
		size_t index7 = inputString.find(">");
		size_t index8 = inputString.find("<=");
		size_t index9 = inputString.find(">=");
		size_t index10 = inputString.find("\n");

		// Find the minimum non-negative index among the three
		size_t plusPos = min({ index1, index2, index3, index4,index5, index6, index7, index8, index9, index10 });

		// Extract the first expression
		string expression = inputString.substr(0, plusPos);

		// Trim leading and trailing whitespaces
		size_t firstNonSpace = expression.find_first_not_of(" \t");
		size_t lastNonSpace = expression.find_last_not_of(" \t");

		if (firstNonSpace != string::npos && lastNonSpace != string::npos) {
			expression = expression.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
		}

		// Remove the extracted expression from the inputString
		inputString.erase(0, plusPos);

		return expression;
	}
	void extract2D1To2(const string& text, string& Aircraft, string& From, string& To) {
		// Find the positions of '[' and ']' to extract the substrings
		size_t aircraftStartPos = text.find('[') + 1;
		size_t aircraftEndPos = text.find(']', aircraftStartPos);

		// Extract the aircraft type
		Aircraft = text.substr(aircraftStartPos, aircraftEndPos - aircraftStartPos);

		// Find the positions of '[' and ',' to extract the substrings
		size_t city1StartPos = aircraftEndPos + 2; // Move past "][" to the next character
		size_t commaPos = text.find(',', city1StartPos);

		// Extract the first city
		From = text.substr(city1StartPos, commaPos - city1StartPos);

		// Find the positions of ',' and ']' to extract the substrings
		size_t city2StartPos = commaPos + 1;
		size_t city2EndPos = text.find(']', city2StartPos);

		// Extract the second city
		To = text.substr(city2StartPos, city2EndPos - city2StartPos);
	}
	void extract2D2To1(const string& inputString, string& from, string& to, string& fareClassName) {
		// Find the positions of '[' and ',' to extract the substrings
		size_t city1StartPos = inputString.find('[') + 1;
		size_t comma1Pos = inputString.find(',');

		// Extract the first city
		from = inputString.substr(city1StartPos, comma1Pos - city1StartPos);

		// Find the positions of ',' and ']' to extract the substrings
		size_t city2StartPos = comma1Pos + 1;
		size_t city2EndPos = inputString.find(']', city2StartPos);

		// Extract the second city
		to = inputString.substr(city2StartPos, city2EndPos - city2StartPos);

		// Find the positions of ']' and '[' to extract the substrings
		size_t fareClassStartPos = city2EndPos + 2;
		size_t fareClassEndPos = inputString.find(']', fareClassStartPos);

		// Extract the fare class
		fareClassName = inputString.substr(fareClassStartPos, fareClassEndPos - fareClassStartPos);
	}
	void extract1D(const string& inputString, string& from, string& to) {
		// Find the positions of '[' and ',' to extract the substrings
		size_t city1StartPos = inputString.find('[') + 1;
		size_t commaPos = inputString.find(',');

		// Extract the first city
		from = inputString.substr(city1StartPos, commaPos - city1StartPos);

		// Find the position of ',' and ']' to extract the second city
		size_t city2StartPos = commaPos + 1;
		size_t city2EndPos = inputString.find(']', city2StartPos);

		// Extract the second city
		to = inputString.substr(city2StartPos, city2EndPos - city2StartPos);
	}
	string extractOperationType(const string& inputString) {
		// Find the position of '[' to extract the operation type
		size_t bracketPos = inputString.find('[');

		// Extract the operation type
		string operationType = inputString.substr(0, bracketPos);

		return operationType;
	}
	bool isIntegerExpression(char& expression) {
			if (!isdigit(expression)) {
				return false;  // If a non-digit character is found, it's not an integer expression
			}
		return true;  // All characters are digits
	}
	int stringToInt(const string& str) {
		try {
			return stoi(str);
		}
		catch (const invalid_argument& e) {
			cerr << "Invalid argument: " << e.what() << endl;
		}
		catch (const out_of_range& e) {
			cerr << "Out of range: " << e.what() << endl;
		}

		// Return a default value (e.g., -1) if conversion fails
		return -1;
	}
	int MinMaxValue(string Constrain, string Condition) {
		int result = (Condition == "Max") ? 0 : numeric_limits<int>::max();
		int Value;
		while (true)
		{
			string MinMax = Constrain.substr(0, 3);
			if (Constrain == "") {
				break;
			}
			if (Condition == "Min") {
				if (isIntegerExpression(Constrain[0])) {
					string number = extractNumber(Constrain);
					Value = stringToInt(number);
				}
				else if (MinMax == "Min" || MinMax == "Max") {
					Constrain.erase(0, 4);
					Value = MinMaxValue(Constrain.substr(0, Constrain.find('}')), MinMax);
					Constrain.erase(0, Constrain.find('}' + 1));
				}
				else if (Constrain[0] == '|') {
					Constrain.erase(0, 1);
					string Expression = extractExpression(Constrain);
					string operationType = extractOperationType(Expression);
					if (operationType == "L") {
						string From, To;
						extract1D(Expression, From, To);
						for (int x = 0; x < numberOfPaths; x++) {
							if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
								Value= Paths[x].getLegCount();
								break;
							}
						}
					}
				}
				else {

					string Expression = extractExpression(Constrain);
					string operationType = extractOperationType(Expression);
					if (operationType == "Cap")
					{
						string AirCraftType, FareClassName;
						extract1D(Expression, AirCraftType, FareClassName);
						for (int x = 0; x < numberofAirCrafts; x++) {
							for (int y = 0; y < numberOfFareClasses; y++)
							{
								if (AirCrafts[x].getType() == AirCraftType && FareClasses[y] == FareClassName) {
									Value = AirCrafts[x].getFareClasses()[y].getCapah();
									x = numberofAirCrafts;
									y = numberOfFareClasses;
								}
							}
						}

					}
					else if (operationType == "M") {
						string From, To, FareClassName;
						extract2D2To1(Expression, From, To, FareClassName);
						for (int x = 0; x < numberOfPaths; x++) {
							for (int y = 0; y < numberOfFareClasses; y++) {
								if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									Value = Paths[x].getFareClasses()[y].getMph();
									y = numberOfFareClasses;
									x = numberOfPaths;
								}
							}
						}
					}
				}
				result = (result < Value) ? result : Value;
				Constrain.erase(0, 1);
			}
			else {
				if (isIntegerExpression(Constrain[0])) {
					string number = extractNumber(Constrain);
					Value = stringToInt(number);
				}
				else if (MinMax == "Min" || MinMax == "Max") {
					Constrain.erase(0, 4);
					Value = MinMaxValue(Constrain.substr(0, Constrain.find('}')), MinMax);
					Constrain.erase(0, Constrain.find('}' + 1));
				}
				else if (Constrain[0] == '|') {
					Constrain.erase(0, 1);
					string Expression = extractExpression(Constrain);
					string operationType = extractOperationType(Expression);
					if (operationType == "L") {
						string From, To;
						extract1D(Expression, From, To);
						for (int x = 0; x < numberOfPaths; x++) {
							if (Paths[x].getFrom() == From && Paths[x].getTo() == To) {
								Value = Paths[x].getLegCount();
								break;
							}
						}
					}
				}
				else {

					string Expression = extractExpression(Constrain);
					string operationType = extractOperationType(Expression);
					if (operationType == "Cap")
					{
						string AirCraftType, FareClassName;
						extract1D(Expression, AirCraftType, FareClassName);
						for (int x = 0; x < numberofAirCrafts; x++) {
							for (int y = 0; y < numberOfFareClasses; y++)
							{
								if (AirCrafts[x].getType() == AirCraftType && FareClasses[y] == FareClassName) {
									Value = AirCrafts[x].getFareClasses()[y].getCapah();
									x = numberofAirCrafts;
									y = numberOfFareClasses;
								}
							}
						}

					}
					else if (operationType == "M") {
						string From, To, FareClassName;
						extract2D2To1(Expression, From, To, FareClassName);
						for (int x = 0; x < numberOfPaths; x++) {
							for (int y = 0; y < numberOfFareClasses; y++) {
								if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
									Value = Paths[x].getFareClasses()[y].getMph();
									y = numberOfFareClasses;
									x = numberOfPaths;
								}
							}
						}
					}
				}
				result = (result > Value) ? result : Value;
			}
		}
		return result;
	}
private:
	typedef IloArray<IloNumVarArray> NumVar2D;
	int numberofAirCrafts, numberOfPaths, numberOfLegs, numberOfFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM;
	int* numPathsthatContainLeg;
	//Sets
	Leg* Legs;
	Path* Paths;
	AirCraft* AirCrafts;
	string* FareClasses;
	Path* pathsWithTypeM;
	Path* pathsWithTypeO;
	Leg* legsOfTypeM;
	Leg* legsOfTypeO;
	string** PathsthatContainLeg;
};
