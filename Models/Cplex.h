#pragma once
#include"ilcplex/ilocplex.h";
#include<chrono>;

class Cplex
{
public:
	Cplex(string** PathsthatContainLeg, int* numPathsthatContainLeg, int numAirCrafts, int numPaths, int numLegs, int numFareClasses, int numOfLegsOfTypeO, int numOfLegsOfTypeM, int numOfPathsWithLegO, int numOfPathsWithLegM, int numOfU, int numOfV, Leg* Legs, Path* Paths, AirCraft* AirCrafts, string* FareClasses, Path* pathsWithTypeM, Path* pathsWithTypeO, Leg* legsOfTypeM, Leg* legsOfTypeO, Leg** U, Leg** V, double** S)
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
		numOfU(numOfU),
		numOfV(numOfV),
		Legs(Legs),
		Paths(Paths),
		AirCrafts(AirCrafts),
		FareClasses(FareClasses),
		pathsWithTypeM(pathsWithTypeM),
		pathsWithTypeO(pathsWithTypeO),
		legsOfTypeM(legsOfTypeM),
		legsOfTypeO(legsOfTypeO),
		U(U),
		V(V),
		S(S) {}
	bool System() {
		auto start = chrono::high_resolution_clock::now();
		IloEnv env;
		IloModel Model(env);
#pragma region Define decision variable
		IloArray<IloBoolVarArray> X(env, numberofAirCrafts);
		IloBoolVarArray Z(env, numberOfPaths);
		IloArray<IloNumVarArray> P(env, numberOfPaths);
		IloNumVarArray ATL(env, numberOfLegs, 0, IloInfinity);
		IloNumVarArray Alpha(env, numberOfLegs, 0, IloInfinity);
		IloNumVarArray Byta(env, numberOfLegs, 0, IloInfinity);
		IloArray<IloBoolVarArray> Sigma(env, numberOfLegs);

		for (int s = 0; s < numberofAirCrafts; s++)
		{
			X[s] = IloBoolVarArray(env, numberOfLegs);
			for (int i = 0; i < numberOfLegs; i++) {
				X[s][i] = IloBoolVar(env);
			}
		}
		for (int i = 0; i < numberOfLegs; i++) {
			Z[i] = IloBoolVar(env);
		}
		for (int s = 0; s < numberOfLegs; s++)
		{
			Sigma[s] = IloBoolVarArray(env, numberOfLegs);
			for (int i = 0; i < numberOfLegs; i++) {
				Sigma[s][i] = IloBoolVar(env);
			}
		}
		for (int s = 0; s < numberOfPaths; s++)
		{
			P[s] = IloNumVarArray(env, numberOfFareClasses, 0, IloInfinity);
		}
#pragma endregion
#pragma region Objective Function
		double weight1 = 0.5;
		double weight2 = 0.5;
		IloExpr exp0(env);
		IloExpr exp1(env);

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
		for (int i = 0; i < numberOfLegs; i++) {
			exp1 += (Legs[i].getPE() * Alpha[i] + Legs[i].getPL() * Byta[i]);
		}
		IloObjective combineObj = IloMaximize(env, weight1 * exp0 - weight2 * exp1);
		Model.add(combineObj);

#pragma endregion
#pragma region Constraints
		//Constrain1
		string Constrain = readDataFromFile();
		int countConstrain = countLines(Constrain);
		char operation = '+';
		int i = 0;
		string Condition1;
		IloExpr* exp2 = new IloExpr[countConstrain];
		IloExpr* exp3 = new IloExpr[countConstrain];
		// Initialize each IloExpr in the array
		for (int i = 0; i < countConstrain; ++i) {
			exp2[i] = IloExpr(env);
			exp3[i] = IloExpr(env);
		}
		while (i < countConstrain) {
			string MinMax = Constrain.substr(0, 3);
			switch (operation)
			{
			case '+':
			{
				if (Condition1.empty() ) {
					if (isInteger(Constrain[0])) {
						string number = extractNumber(Constrain);
						int value = stringToInt(number);
						exp2[i] += value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain.erase(0, 4);
						exp2[i] += MinMaxValue(Constrain.substr(0, Constrain.find('}') + 1), MinMax);
						Constrain.erase(0, Constrain.find_first_of('}') + 1);
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
									exp2[i] += Paths[x].getLegCount();

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
										exp2[i] += AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "X")
						{
							string From, To, AirCraftName;
							extractX(Expression, AirCraftName, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberofAirCrafts; y++) {
									if (AirCrafts[y].getType() == AirCraftName && Legs[x].getFrom() == From && Legs[x].getTo() == To) {
										exp2[i] += X[y][x];
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
									exp2[i] += Z[x];
									x = numberOfPaths;
								}
							}
						}
						else if (operationType == "P") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2[i] += P[x][y];
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2[i] += Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "Sigma") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
										exp2[i] += Sigma[x][y];
										y = numberOfLegs;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}
						else if (operationType == "ATL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if(Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] += ATL[x];
							}
						}
						else if (operationType == "TE") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val = Legs[x].getTE();
							}
							if (Constrain[0] == '*') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp2[i] += (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp2[i] += val;
							}

						}
						else if (operationType == "TT") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val = Legs[x].getTT();
							}
							if (Constrain[0] == '*') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp2[i] += (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp2[i] += val;
							}

						}
						else if (operationType == "TL") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val = Legs[x].getTL();
							}
							if (Constrain[0] == '*') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp2[i] += (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp2[i] += val;
							}

						}
						else if (operationType == "Alpha") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] += Alpha[x];
							}
						}
						else if (operationType == "Byta") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] += Byta[x];
							}
						}
						else if (operationType == "PL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] += Legs[x].getPL();
							}
							}
						else if (operationType == "PE") {
								string From, To;
								extract1D(Expression, From, To);
								for (int x = 0; x < numberOfLegs; x++) {
									if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
										exp2[i] += Legs[x].getPE();
								}
								}
						else if (operationType == "S") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							double val = 0;
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[x].getFrom() == From2 && Legs[x].getTo() == To2) {
										val = S[x][y];
										x = numberOfLegs;
										y = numberOfLegs;
									}
									else if (Legs[y].getFrom() == From1 && Legs[y].getTo() == To1 && Legs[x].getFrom() == From2 && Legs[x].getTo() == To2) {
										val += S[y][x];
										x = numberOfLegs;
										y = numberOfLegs;
									}
								}
							}
							if (Constrain[0] == '*') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp2[i] += (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
										else if (Legs[y].getFrom() == From1 && Legs[y].getTo() == To1 && Legs[x].getFrom() == From2 && Legs[x].getTo() == To2) {
											exp2[i] += (val * Sigma[y][x]);
											x = numberOfLegs;
											y = numberOfLegs;
										}

									}
								}
							}
							else {
								exp2[i] += val;
							}
						}
					}
				}
				else
				{
					if (isInteger(Constrain[0])) {
						string number = extractNumber(Constrain);
						int value = stringToInt(number);
						exp3[i] += value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain.erase(0, 4);
						exp3[i] += MinMaxValue(Constrain.substr(0, Constrain.find('}') + 1), MinMax);
						Constrain.erase(0, Constrain.find_first_of('}') + 1);
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
									exp3[i] += Paths[x].getLegCount();
									Constrain.erase(0, 1);
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
										exp3[i] += AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "X")
						{
							string From, To, AirCraftName;
							extractX(Expression, AirCraftName, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberofAirCrafts; y++) {
									if (AirCrafts[y].getType() == AirCraftName && Legs[x].getFrom() == From && Legs[x].getTo() == To) {
										exp3[i] += X[y][x];
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
									exp3[i] += Z[x];
									x = numberOfPaths;
								}
							}
						}
						else if (operationType == "P") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp3[i] += P[x][y];
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp3[i] += Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "Sigma") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
										exp3[i] += Sigma[x][y];
										y = numberOfLegs;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}
						else if (operationType == "ATL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] += ATL[x];
							}
						}
						else if (operationType == "TE") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val = Legs[x].getTE();
							}
							if (Constrain[0] == '*') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp3[i] += (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp3[i] += val;
							}

						}
						else if (operationType == "TT") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val = Legs[x].getTT();
							}
							if (Constrain[0] == '*') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp3[i] += (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp3[i] += val;
							}

						}
						else if (operationType == "TL") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val= Legs[x].getTL();
							}
							if (Constrain[0] == '*') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp3[i] += (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp3[i] += val;
							}

						}
						else if (operationType == "Alpha") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] += Alpha[x];
							}
						}
						else if (operationType == "Byta") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] += Byta[x];
							}
						}
						else if (operationType == "PL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] += Legs[x].getPL();
							}
						}
						else if (operationType == "PE") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] += Legs[x].getPE();
							}
						}
						else if (operationType == "S") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							double val = 0;
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[x].getFrom() == From2 && Legs[x].getTo() == To2) {
										val = S[x][y];
										x = numberOfLegs;
										y = numberOfLegs;
									}
									else if (Legs[y].getFrom() == From1 && Legs[y].getTo() == To1 && Legs[x].getFrom() == From2 && Legs[x].getTo() == To2) {
										val = S[y][x];
										x = numberOfLegs;
										y = numberOfLegs;
									}
								}
							}
							if (Constrain[0] == '*') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp3[i] += (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
										else if (Legs[y].getFrom() == From1 && Legs[y].getTo() == To1 && Legs[x].getFrom() == From2 && Legs[x].getTo() == To2) {
											exp3[i] += (val * Sigma[y][x]);
											x = numberOfLegs;
											y = numberOfLegs;
										}
									}
								}
							}
							else {
								exp3[i] += val;
							}
						}
					}
				}
				//Check If end line
				processConstraintsToModel(Constrain, Model, exp2, exp3, operation, countConstrain, Condition1, i);
				break;
			}
			case '-':
			{
				if (Condition1.empty() ) {
					if (isInteger(Constrain[0])) {
						string number = extractNumber(Constrain);
						int value = stringToInt(number);
						exp2[i] -= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain.erase(0, 4);
						exp2[i] -= MinMaxValue(Constrain.substr(0, Constrain.find('}') + 1), MinMax);
						Constrain.erase(0, Constrain.find_first_of('}') + 1);
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
									exp2[i] -= Paths[x].getLegCount();

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
										exp2[i] -= AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "X")
						{
							string From, To, AirCraftName;
							extractX(Expression, AirCraftName, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberofAirCrafts; y++) {
									if (AirCrafts[y].getType() == AirCraftName && Legs[x].getFrom() == From && Legs[x].getTo() == To) {
										exp2[i] -= X[y][x];
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
									exp2[i] -= Z[x];
									x = numberOfPaths;
								}
							}
						}
						else if (operationType == "P") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2[i] -= P[x][y];
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2[i] -= Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "Sigma") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
										exp2[i] -= Sigma[x][y];
										y = numberOfLegs;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}
						else if (operationType == "ATL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] -= ATL[x];
							}
						}
						else if (operationType == "TE") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val = Legs[x].getTE();
							}
							if (Constrain[0] != '\n') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp2[i] -= (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp2[i] -= val;
							}

						}
						else if (operationType == "TT") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val = Legs[x].getTT();
							}
							if (Constrain[0] != '\n') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp2[i] -= (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp2[i] -= val;
							}

						}
						else if (operationType == "TL") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val = Legs[x].getTL();
							}
							if (Constrain[0] != '\n') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp2[i] -= (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp2[i] -= val;
							}

						}
						else if (operationType == "Alpha") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] -= Alpha[x];
							}
						}
						else if (operationType == "Byta") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] -= Byta[x];
							}
						}
						else if (operationType == "PL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] -= Legs[x].getPL();
							}
						}
						else if (operationType == "PE") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] -= Legs[x].getPE();
							}
						}
						else if (operationType == "S") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							double val = 0;
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[x].getFrom() == From2 && Legs[x].getTo() == To2) {
										val = S[x][y];
										x = numberOfLegs;
										y = numberOfLegs;
									}
									else if (Legs[y].getFrom() == From1 && Legs[y].getTo() == To1 && Legs[x].getFrom() == From2 && Legs[x].getTo() == To2) {
										exp2[i] -= S[y][x];
										x = numberOfLegs;
										y = numberOfLegs;
									}
								}
							}
							if (Constrain[0] != '\n') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp2[i] -= (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp2[i] -= val;
							}
							}
					}
				}
				else
				{
					if (isInteger(Constrain[0])) {
						string number = extractNumber(Constrain);
						int value = stringToInt(number);
						exp3[i] -= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain.erase(0, 4);
						exp3[i] -= MinMaxValue(Constrain.substr(0, Constrain.find('}') + 1), MinMax);
						Constrain.erase(0, Constrain.find_first_of('}') + 1);
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
									exp3[i] -= Paths[x].getLegCount();
									Constrain.erase(0, 1);
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
										exp3[i] -= AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "X")
						{
							string From, To, AirCraftName;
							extractX(Expression, AirCraftName, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberofAirCrafts; y++) {
									if (AirCrafts[y].getType() == AirCraftName && Legs[x].getFrom() == From && Legs[x].getTo() == To) {
										exp3[i] -= X[y][x];
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
									exp3[i] -= Z[x];
									x = numberOfPaths;
								}
							}
						}
						else if (operationType == "P") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp3[i] -= P[x][y];
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp3[i] -= Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "Sigma") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
										exp3[i] -= Sigma[x][y];
										y = numberOfLegs;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}
						else if (operationType == "ATL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] -= ATL[x];
							}
						}
						else if (operationType == "TE") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val = Legs[x].getTE();
							}
							if (Constrain[0] != '\n') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp3[i] -= (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp3[i] -= val;
							}

						}
						else if (operationType == "TT") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val = Legs[x].getTT();
							}
							if (Constrain[0] != '\n') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp3[i] -= (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp3[i] -= val;
							}

						}
						else if (operationType == "TL") {
							string From, To;
							double val;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									val = Legs[x].getTL();
							}
							if (Constrain[0] == '*') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								string From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp3[i] -= (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp3[i] -= val;
							}

						}
						else if (operationType == "Alpha") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] -= Alpha[x];
							}
						}
						else if (operationType == "Byta") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] -= Byta[x];
							}
						}
						else if (operationType == "PL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] -= Legs[x].getPL();
							}
						}
						else if (operationType == "PE") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] -= Legs[x].getPE();
							}
						}
						else if (operationType == "S") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							double val = 0;
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[x].getFrom() == From2 && Legs[x].getTo() == To2) {
										val = S[x][y];
										x = numberOfLegs;
										y = numberOfLegs;
									}
									else if (Legs[y].getFrom() == From1 && Legs[y].getTo() == To1 && Legs[x].getFrom() == From2 && Legs[x].getTo() == To2) {
										exp3[i] -= S[y][x];
										x = numberOfLegs;
										y = numberOfLegs;
									}
								}
							}
							if (Constrain[0] != '\n') {
								Constrain.erase(0, 1);
								Expression = extractExpression(Constrain);
								From1, To1, From2, To2;
								extractSigma_S(Expression, From1, To1, From2, To2);
								for (int x = 0; x < numberOfLegs; x++) {
									for (int y = 0; y < numberOfLegs; y++) {
										if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
											exp3[i] -= (val * Sigma[x][y]);
											y = numberOfLegs;
											x = numOfLegsOfTypeM;
										}
									}
								}
							}
							else {
								exp3[i] -= val;
							}
							}
					}
				}
				//Check If end line
				processConstraintsToModel(Constrain, Model, exp2, exp3, operation, countConstrain, Condition1, i);
				break;
			}
			case '*':
			{
				if (Condition1.empty() ) {
					if (isInteger(Constrain[0])) {
						string number = extractNumber(Constrain);
						int value = stringToInt(number);
						exp2[i] *= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain.erase(0, 4);
						exp2[i] *= MinMaxValue(Constrain.substr(0, Constrain.find('}') + 1), MinMax);
						Constrain.erase(0, Constrain.find_first_of('}') + 1);
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
									exp2[i] *= Paths[x].getLegCount();

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
										exp2[i] *= AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2[i] *= Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "TE") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] *= Legs[x].getTE();
							}
						}
						else if (operationType == "TT") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] *= Legs[x].getTT();
							}
						}
						else if (operationType == "TL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] *= Legs[x].getTL();
							}
						}
						else if (operationType == "PL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] *= Legs[x].getPL();
							}
						}
						else if (operationType == "PE") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] *= Legs[x].getPE();
							}
						}
						else if (operationType == "S") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
										exp2[i] *= S[x][y];
										y = numberOfLegs;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}
					}
				}
				else
				{
					if (isInteger(Constrain[0])) {
						string number = extractNumber(Constrain);
						int value = stringToInt(number);
						exp3[i] *= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain.erase(0, 4);
						exp3[i] *= MinMaxValue(Constrain.substr(0, Constrain.find('}') + 1), MinMax);
						Constrain.erase(0, Constrain.find_first_of('}') + 1);
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
									exp3[i] *= Paths[x].getLegCount();
									Constrain.erase(0, 1);
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
										exp3[i] *= AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp3[i] *= Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "TE") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] *= Legs[x].getTE();
							}
						}
						else if (operationType == "TT") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] *= Legs[x].getTT();
							}
						}
						else if (operationType == "TL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] *= Legs[x].getTL();
							}
						}
						else if (operationType == "PL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] *= Legs[x].getPL();
							}
						}
						else if (operationType == "PE") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] *= Legs[x].getPE();
							}
						}
						else if (operationType == "S") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
										exp3[i] *= S[x][y];
										y = numberOfLegs;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}

					}
				}
				//Check If end line
				processConstraintsToModel(Constrain, Model, exp2, exp3, operation, countConstrain, Condition1, i);
				break;
			}
			case '/':
			{
				if (Condition1.empty() ) {
					if (isInteger(Constrain[0])) {
						string number = extractNumber(Constrain);
						int value = stringToInt(number);
						exp2[i] /= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain.erase(0, 4);
						exp2[i] /= MinMaxValue(Constrain.substr(0, Constrain.find('}') + 1), MinMax);
						Constrain.erase(0, Constrain.find_first_of('}') + 1);
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
									exp2[i] /= Paths[x].getLegCount();

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
										exp2[i] /= AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp2[i] /= Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "TE") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] /= Legs[x].getTE();
							}
						}
						else if (operationType == "TT") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] /= Legs[x].getTT();
							}
						}
						else if (operationType == "TL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] /= Legs[x].getTL();
							}
						}
						else if (operationType == "PL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] /= Legs[x].getPL();
							}
						}
						else if (operationType == "PE") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp2[i] /= Legs[x].getPE();
							}
						}
						else if (operationType == "S") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
										exp2[i] /= S[x][y];
										y = numberOfLegs;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}
					}
				}
				else
				{
					if (isInteger(Constrain[0])) {
						string number = extractNumber(Constrain);
						int value = stringToInt(number);
						exp3[i] /= value;
					}
					else if (MinMax == "Min" || MinMax == "Max") {
						Constrain.erase(0, 4);
						exp3[i] /= MinMaxValue(Constrain.substr(0, Constrain.find('}') + 1), MinMax);
						Constrain.erase(0, Constrain.find_first_of('}') + 1);
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
									exp3[i] /= Paths[x].getLegCount();
									Constrain.erase(0, 1);
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
										exp3[i] += AirCrafts[x].getFareClasses()[y].getCapah();
										x = numberofAirCrafts;
										y = numberOfFareClasses;
									}
								}
							}

						}
						else if (operationType == "M") {
							string From, To, FareClassName;
							extractP_M(Expression, From, To, FareClassName);
							for (int x = 0; x < numberOfPaths; x++) {
								for (int y = 0; y < numberOfFareClasses; y++) {
									if (FareClasses[y] == FareClassName && Paths[x].getFrom() == From && Paths[x].getTo() == To) {
										exp3[i] /= Paths[x].getFareClasses()[y].getMph();
										y = numberOfFareClasses;
										x = numberOfPaths;
									}
								}
							}
						}
						else if (operationType == "TE") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] /= Legs[x].getTE();
							}
						}
						else if (operationType == "TT") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] /= Legs[x].getTT();
							}
						}
						else if (operationType == "TL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] /= Legs[x].getTL();
							}
						}
						else if (operationType == "PL") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] /= Legs[x].getPL();
							}
						}
						else if (operationType == "PE") {
							string From, To;
							extract1D(Expression, From, To);
							for (int x = 0; x < numberOfLegs; x++) {
								if (Legs[x].getFrom() == From && Legs[x].getTo() == To)
									exp3[i] /= Legs[x].getPE();
							}
						}
						else if (operationType == "S") {
							string From1, To1, From2, To2;
							extractSigma_S(Expression, From1, To1, From2, To2);
							for (int x = 0; x < numberOfLegs; x++) {
								for (int y = 0; y < numberOfLegs; y++) {
									if (Legs[x].getFrom() == From1 && Legs[x].getTo() == To1 && Legs[y].getFrom() == From2 && Legs[y].getTo() == To2) {
										exp3[i] /= S[x][y];
										y = numberOfLegs;
										x = numOfLegsOfTypeM;
									}
								}
							}
						}
					}
				}
				//Check If end line
				processConstraintsToModel(Constrain, Model, exp2, exp3, operation, countConstrain, Condition1, i);
				break;
			}
			}
		}
#pragma endregion
#pragma region solveModel
		IloCplex cplex(Model);
		cplex.exportModel("model.lp");
		cplex.setOut(env.getNullStream());
		if (!cplex.isExtracted(Model)) {
			std::cerr << "Model not extracted properly" << std::endl;
		}
		else {
			std::cout << "Model extracted successfully" << std::endl;
		}		
		if (!cplex.solve()) {
			env.error() << "Failed to optimize the Master Problem!!!" << endl;
			return false;
		}

		long long obj = cplex.getObjValue();

		auto end = chrono::high_resolution_clock::now();
		auto Elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
		cout << "\t Elapsed Time(ms): " << Elapsed.count() << endl;
		cout << "Status: " << cplex.getStatus() << endl;
		cout << "\nThe objective value is: " << obj << endl;
		for (int i = 0; i < numberOfPaths; i++)
		{
			for (int j = 0; j < numberOfFareClasses; j++) {
				double Pval = cplex.getValue(P[i][j]);
					cout << "\t\t\t P[" << Paths[i].getFrom() << "," << Paths[i].getTo() << "][" << FareClasses[j] << "] = " << Pval << endl;
			}
		}
		for (int i = 0; i < numberofAirCrafts; i++)
		{
			for (int x = 0; x < numberOfLegs; x++)
			{
				double Xval = cplex.getValue(X[i][x]);

				cout << "\t\t\t X[" << AirCrafts[i].getType() << "][" << Legs[x].getFrom() << "," << Legs[x].getTo() << "] = " << Xval << endl;
			}
		}
		for (int i = 0; i < numberOfLegs; i++) {
			double ATLval = cplex.getValue(ATL[i]);
			cout << "\t\t\t ATL[" << Legs[i].getFrom() << "," << Legs[i].getTo() << "]="<<ATLval<< endl;
		}
		for (int i = 0; i < numberOfLegs; i++) {
			double Alphaval = cplex.getValue(Alpha[i]);
			cout << "\t\t\t Alpha[" << Legs[i].getFrom() << "," << Legs[i].getTo() << "]=" << Alphaval << endl;
		}
		for (int i = 0; i < numberOfLegs; i++) {
			double Bytaval = cplex.getValue(Byta[i]);
			cout << "\t\t\t Byta[" << Legs[i].getFrom() << "," << Legs[i].getTo() << "]=" << Bytaval << endl;
		}
		cplex.writeSolution("solution.lp");
#pragma endregion
		return true;
	}
	string readDataFromFile() {
		ifstream file("./Data/Output.txt");
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
		size_t plusPos = min({ index1, index2, index3, index4,index5, index6, index7, index8, index9, index10, index11, index12, index13 }) + 1;

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
	void extractX(const string& text, string& Aircraft, string& From, string& To) {
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
	void extractP_M(const string& inputString, string& from, string& to, string& fareClassName) {
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
	bool isInteger(char& expression) {
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
				if (isInteger(Constrain[0])) {
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
						extractP_M(Expression, From, To, FareClassName);
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
				if (isInteger(Constrain[0])) {
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
						extractP_M(Expression, From, To, FareClassName);
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
	void extractSigma_S(const string& text, string& From1, string& To1, string& From2, string& To2) {
		// Find the positions of '[' and ']' to extract the substrings
		size_t StartPos = text.find('[') + 1;
		size_t EndPos = text.find(']', StartPos);
		size_t commaPos1 = text.find(',', StartPos);
		From1 = text.substr(StartPos, commaPos1 - StartPos);
		size_t city1StartPos1 = commaPos1 + 1;
		size_t city1EndPos1 = text.find(']', city1StartPos1);

		// Extract the second city
		To1 = text.substr(city1StartPos1, city1EndPos1 - city1StartPos1);

		// Find the positions of '[' and ',' to extract the substrings
		size_t city1StartPos2 = EndPos + 2; // Move past "][" to the next character
		size_t commaPos2 = text.find(',', city1StartPos2);

		// Extract the first city
		From2 = text.substr(city1StartPos2, commaPos2 - city1StartPos2);

		// Find the positions of ',' and ']' to extract the substrings
		size_t city2StartPos2 = commaPos2 + 1;
		size_t city2EndPos2 = text.find(']', city2StartPos2);

		// Extract the second city
		To2 = text.substr(city2StartPos2, city2EndPos2 - city2StartPos2);
	}
	void processConstraintsToModel(string &constrain1Data, IloModel& model, IloExpr* exp2, IloExpr* exp3, char &operation, int countConstrain1, string &Condition1, int &i) {
		if (constrain1Data[0] == '\n') {
			if (Condition1 == "=") {
				model.add(exp2[i] == exp3[i]);
			}
			else if (Condition1 == "<=") {
				model.add(exp2[i] <= exp3[i]);
			}
			else if (Condition1 == ">=") {
				model.add(exp2[i] >= exp3[i]);
			}
			else if (Condition1 == "<") {
				model.add(exp2[i] < exp3[i]);
			}
			else if (Condition1 == ">") {
				model.add(exp2[i] > exp3[i]);
			}
			operation = '+';
			Condition1.clear();
			i++;
		}
		else if (constrain1Data[0] != '<' && constrain1Data[0] != '=' && constrain1Data[0] != '>' &&
			(constrain1Data[0] + constrain1Data[1] != '<=' && constrain1Data[0] + constrain1Data[1] != '>=')) {
			operation = constrain1Data[0];
		}
		else if (constrain1Data.substr(0, 2) == "<=" || constrain1Data.substr(0, 2) == ">=") {
			operation = '+';
			Condition1 = constrain1Data.substr(0, 2);
			constrain1Data.erase(0, 1);
		}
		else {
			operation = '+';
			Condition1 = constrain1Data[0];
		}
		constrain1Data.erase(0, 1);
	}
private:
	typedef IloArray<IloNumVarArray> NumVar2D;
	int numberofAirCrafts, numberOfPaths, numberOfLegs, numberOfFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM, numOfU, numOfV;
	int* numPathsthatContainLeg;
	double** S;
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
	Leg** U;
	Leg** V;
};
