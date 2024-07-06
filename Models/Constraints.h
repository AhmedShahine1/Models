#pragma once
#include <iostream>
#include "AirCraft.h"
#include "Leg.h"
#include "FareClass.h"
#include "Path.h"

using namespace std;
class Constraints
{
public:
	int numberofAirCrafts, numberOfPaths, numberOfLegs, numberOfFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM, CountU, CountV;
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
	Constraints(string** PathsthatContainLeg, int* numPathsthatContainLeg, int numAirCrafts, int numPaths, int numLegs, int numFareClasses, int numOfLegsOfTypeO, int numOfLegsOfTypeM, int numOfPathsWithLegO, int numOfPathsWithLegM, int CountU, int CountV, Leg* Legs, Path* Paths, AirCraft* AirCrafts, string* FareClasses, Path* pathsWithTypeM, Path* pathsWithTypeO, Leg* legsOfTypeM, Leg* legsOfTypeO, Leg** U, Leg** V, double** S)
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
		CountU(CountU),
		CountV(CountV),
		Legs(Legs),
		Paths(Paths),
		AirCrafts(AirCrafts),
		FareClasses(FareClasses),
		pathsWithTypeM(pathsWithTypeM),
		pathsWithTypeO(pathsWithTypeO),
		legsOfTypeM(legsOfTypeM),
		legsOfTypeO(legsOfTypeO),
		U(U),
		S(S),
		V(V) {
	}
	~Constraints() {}
	void Constrain_1()
	{
		for (int i = 0; i < numOfLegsOfTypeM; ++i)
		{
			string Output;
			for (int j = 0; j < numberofAirCrafts; ++j)
			{
				if (j == numberofAirCrafts - 1)
				{
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeM[i].getFrom() + "," + legsOfTypeM[i].getTo() + "]=1";
				}
				else
				{
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeM[i].getFrom() + "," + legsOfTypeM[i].getTo() + "]" + "+";
				}
			}
			WriteData(Output);
		}
	}

	void Constrain_2()
	{
		for (int i = 0; i < numOfLegsOfTypeO; ++i)
		{
			string Output;
			for (int j = 0; j < numberofAirCrafts; ++j)
			{
				if (j == numberofAirCrafts - 1)
				{
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeO[i].getFrom() + "," + legsOfTypeO[i].getTo() + "]<=1";
				}
				else
				{
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeO[i].getFrom() + "," + legsOfTypeO[i].getTo() + "]" + "+";
				}
			}
			WriteData(Output);
		}
	}

	void Constrain_3() {
		for (int i = 0; i < numOfPathsWithLegO; i++)
		{
			string Output;
			Output += "Z[" + pathsWithTypeO[i].getFrom() + "," + pathsWithTypeO[i].getTo() + "]-";
			for (int j = 0; j < numOfLegsOfTypeO; j++) {

				for (int x = 0; x < numberofAirCrafts; x++)
				{
					if (x == numberofAirCrafts - 1) {
						Output += "X[" + AirCrafts[x].getType() + "][" + legsOfTypeO[j].getFrom() + "," + legsOfTypeO[j].getTo() + "]<=0";
					}
					else {
						Output += "X[" + AirCrafts[x].getType() + "][" + legsOfTypeO[j].getFrom() + "," + legsOfTypeO[j].getTo() + "]-";
					}
				}
			}
			WriteData(Output);
		}
	}

	void Constrain_4()
	{

		for (int optional_path_repeater = 0; optional_path_repeater < numOfPathsWithLegO; optional_path_repeater++)
		{
			string Output;
			Output += "Z[" + pathsWithTypeO[optional_path_repeater].getFrom() + "," + pathsWithTypeO[optional_path_repeater].getTo() + "]" + "-";
			for (int aircrafts = 0; aircrafts < numberofAirCrafts; aircrafts++)
			{
				for (int optional_legs = 0; optional_legs < numOfLegsOfTypeO; optional_legs++)
				{
					Output += "X[" + AirCrafts[aircrafts].getType() + "][" + legsOfTypeO[optional_legs].getFrom() + "," + legsOfTypeO[optional_legs].getTo() + "]";
					if (aircrafts != (numberofAirCrafts - 1))
					{
						Output += "-";
					}
				}//end last loop 

			}// end second loop 


			Output += ">=1-|L[" + pathsWithTypeO[optional_path_repeater].getFrom() + "," + pathsWithTypeO[optional_path_repeater].getTo() + "]|";
			WriteData(Output);
		}//end first loop 
	}//end of function

	void Constrain_5()
	{
		for (int optional_legs_repeater = 0; optional_legs_repeater < numOfLegsOfTypeO; optional_legs_repeater++)
		{
			string Output;
			for (int aircraft = 0; aircraft < numberofAirCrafts; aircraft++)
			{
				Output += "X[" + AirCrafts[aircraft].getType() + "][" + legsOfTypeO[optional_legs_repeater].getFrom() + "," + legsOfTypeO[optional_legs_repeater].getTo() + "]";
				// print the + for the summation equation 
				if (aircraft != (numberofAirCrafts - 1))
				{
					Output += "+";
				}
			}


			Output += "<=";
			for (int optional_paths_repeater = 0; optional_paths_repeater < numOfPathsWithLegO; ++optional_paths_repeater)
			{
				Output += "Z[" + pathsWithTypeO[optional_paths_repeater].getFrom() + "," + pathsWithTypeO[optional_paths_repeater].getTo() + "]";
				if (optional_paths_repeater != (numOfPathsWithLegO - 1)) {
					Output += "+";
				}

			}
			WriteData(Output);
		}
	}

	void Constrain_6() {
		for (int i = 0; i < numberOfLegs; i++) {

			for (int j = 0; j < numberOfFareClasses; j++)
			{
				string Output;
				int Count = 0;
				for (int p = 0; p < numberOfPaths; p++)
				{
					if (Paths[p].hasLeg(Legs[i].getTo(), Legs[i].getFrom()))
					{
						Output += "P[" + Paths[p].getFrom() + "," + Paths[p].getTo() + "][" + Paths[p].getFareClasses()[j].getName() + "]";
						if (p != numberOfPaths - 1 && Paths[p + 1].hasLeg(Legs[i].getTo(), Legs[i].getFrom()))
						{
							Output += "+";
						}
						else if (p == numberOfPaths - 1) {
							Output += "<=";
						}
						Count++;
					}
					else {
						if (p != 0 && p != numberOfPaths - 1 && Paths[p + 1].hasLeg(Legs[i].getTo(), Legs[i].getFrom()) && Count != 0)
						{
							Output += "+";
						}
						else if (p == numberOfPaths - 1) {
							Output += "<=";
						}
					}
				}


				for (int s = 0; s < numberofAirCrafts; s++)
				{
					for (int p = 0; p < numberOfPaths; p++)
					{
						if (p == 0) {
							Output += "X[" + AirCrafts[s].getType() + "][" + Legs[i].getFrom() + "," + Legs[i].getTo() + "]*Min{Cap[" + AirCrafts[s].getType() + "," + Paths[p].getFareClasses()[j].getName() + "],";
						}
						if (Paths[p].hasLeg(Legs[i].getTo(), Legs[i].getFrom()))
						{
							Output += "M[" + Paths[p].getFrom() + "," + Paths[p].getTo() + "][" + Paths[p].getFareClasses()[j].getName() + "]+";
							if (p == numberOfPaths - 1) {
								Output = Output.substr(0, Output.size() - 2);
								Output += "]}";
							}
						}
						else
						{
							if (p == numberOfPaths - 1) {
								Output = Output.substr(0, Output.size() - 2);
								Output += "]}";
							}
						}
					}
					if (s != numberofAirCrafts - 1)
					{

						Output += "+";
					}
					else {
						Output += "";
					}
				}
				WriteData(Output);
			}
		}
	}

	void Constrain_7() {
		for (int i = 0; i < numOfPathsWithLegO; i++)
		{
			string Output;
			for (int j = 0; j < numberOfFareClasses; j++) {
				Output = "P[" + pathsWithTypeO[i].getFrom() + "," + pathsWithTypeO[i].getTo() + "][" + FareClasses[j] + "]<=Min{M[" + pathsWithTypeO[i].getFrom() + "," + pathsWithTypeO[i].getTo() + "][" + FareClasses[j] + "],";
				for (int x = 0; x < numberofAirCrafts; x++)
				{
					Output += "Cap[" + AirCrafts[x].getType() + "," + FareClasses[j] + "]";
					if (x != numberofAirCrafts - 1) {
						cout << ",";
						Output += ",";
					}
				}
				Output += "}*Z[" + pathsWithTypeO[i].getFrom() + "," + pathsWithTypeO[i].getTo() + "]";
				WriteData(Output);
			}
		}
	}

	void Constrain_8() {
		for (int i = 0; i < numberOfPaths; i++)
		{
			string Output;
			for (int j = 0; j < numberOfFareClasses; j++) {
				Output = "P[" + Paths[i].getFrom() + "," + Paths[i].getTo() + "][" + FareClasses[j] + "]<=M[" + Paths[i].getFrom() + "," + Paths[i].getTo() + "][" + FareClasses[j] + "]";
				WriteData(Output);
			}
		}
	}

	void Constrain_9() {
		for (int i = 0; i < numberOfLegs; i++)
		{
			string Output;
			for (int j = i + 1; j < numberOfLegs; j++) {
				Output = "Sigma[" + Legs[i].getFrom() + "," + Legs[i].getTo() + "][" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]+Sigma[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "][" + Legs[i].getFrom() + "," + Legs[i].getTo() + "]=1";
				WriteData(Output);
			}
		}
	}

	void Constrain_10() {
		for (int i = 0; i < CountV; i++)
		{
			string Output;
				Output = "Sigma[" + V[i][0].getFrom() + "," + V[i][0].getTo() + "][" + V[i][1].getFrom() + "," + V[i][1].getTo() + "]=1";
				WriteData(Output);
		}
	}

	void Constrain_11() {
		for (int i = 0; i < CountV; i++)
		{
			string Output;
			Output = "ATL[" + V[i][1].getFrom() + "," + V[i][1].getTo() + "]>=ATL[" + V[i][0].getFrom() + "," + V[i][0].getTo() + "]+S[" + V[i][0].getFrom() + "," + V[i][0].getTo() + "][" + V[i][1].getFrom() + "," + V[i][1].getTo() +"]";
			WriteData(Output);
		}
	}

	void Constrain_12() {
		for (int i = 0; i < CountU; i++)
		{
			string Output;
			Output = "ATL[" + U[i][1].getFrom() + "," + U[i][1].getTo() + "]>=ATL[" + U[i][0].getFrom() + "," + U[i][0].getTo() + "]+S[" + U[i][0].getFrom() + "," + U[i][0].getTo() + "][" + U[i][1].getFrom() + "," + U[i][1].getTo() + "]*Sigma[" + U[i][0].getFrom() + "," + U[i][0].getTo() + "][" + U[i][1].getFrom() + "," + U[i][1].getTo() + "]-TL[" + U[i][0].getFrom() + "," + U[i][0].getTo() + "]*Sigma[" + U[i][1].getFrom() + "," + U[i][1].getTo() + "][" + U[i][0].getFrom() + "," + U[i][0].getTo() + "]+TE[" + U[i][1].getFrom() + "," + U[i][1].getTo() + "]*Sigma[" + U[i][1].getFrom() + "," + U[i][1].getTo() + "][" + U[i][0].getFrom() + "," + U[i][0].getTo() + "]";
			WriteData(Output);
			Output = "ATL[" + U[i][0].getFrom() + "," + U[i][0].getTo() + "]>=ATL[" + U[i][1].getFrom() + "," + U[i][1].getTo() + "]+S[" + U[i][1].getFrom() + "," + U[i][1].getTo() + "][" + U[i][0].getFrom() + "," + U[i][0].getTo() + "]*Sigma[" + U[i][1].getFrom() + "," + U[i][1].getTo() + "][" + U[i][0].getFrom() + "," + U[i][0].getTo() + "]-TL[" + U[i][1].getFrom() + "," + U[i][1].getTo() + "]*Sigma[" + U[i][0].getFrom() + "," + U[i][0].getTo() + "][" + U[i][1].getFrom() + "," + U[i][1].getTo() + "]+TE[" + U[i][0].getFrom() + "," + U[i][0].getTo() + "]*Sigma[" + U[i][0].getFrom() + "," + U[i][0].getTo() + "][" + U[i][1].getFrom() + "," + U[i][1].getTo() + "]";
			WriteData(Output);

		}
	}

	void Constrain_13() {
		for (int j = 0; j < numberOfLegs; j++)
		{
			string Output;
			Output = "Alpha[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]>=TT[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]-ATL[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]";
			WriteData(Output);
		}
	}

	void Constrain_14() {
		for (int j = 0; j < numberOfLegs; j++)
		{
			string Output;
			Output = "0<=Alpha[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]\nAlpha[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]<=TT[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]-TE[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]";
			WriteData(Output);
		}
	}

	void Constrain_15() {
		for (int j = 0; j < numberOfLegs; j++)
		{
			string Output;
			Output = "Byta[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]>=ATL[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]-TT[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]";
			WriteData(Output);
		}
	}

	void Constrain_16() {
		for (int j = 0; j < numberOfLegs; j++)
		{
			string Output;
			Output = "0<=Byta[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]\nByta[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]<=TL[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]-TT[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]";
			WriteData(Output);
		}
	}

	void Constrain_17() {
		for (int j = 0; j < numberOfLegs; j++)
		{
			string Output;
			Output = "ATL[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]=TT[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]-Alpha[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]+Byta[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]";
			WriteData(Output);
		}
	}

	void Constrain_18() {
		for (int j = 0; j < numberOfLegs; j++)
		{
			string Output;
			Output = "TE[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]<=ATL[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]\nATL[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]<=TL[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]";
			WriteData(Output);
		}
	}

	void Constrain_19() {
		for (int j = 0; j < numberOfLegs; j++)
		{
			string Output;
			Output = "Alpha[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]>=0";
			WriteData(Output);
		}
	}

	void Constrain_20() {
		for (int j = 0; j < numberOfLegs; j++)
		{
			string Output;
			Output = "Byta[" + Legs[j].getFrom() + "," + Legs[j].getTo() + "]>=0";
			WriteData(Output);
		}
	}

	void WriteData(string data) {
		ofstream outputFile("./Data/Output.txt", ios::app);

		// Check if the file is successfully opened
		if (!outputFile.is_open()) {
			cout << "Error opening the file!" << endl;
		}
		outputFile << data << endl;
		outputFile.close();
	}
	void ClearData() {
		ofstream outfile("./Data/Output.txt", ios::trunc); // Open file and truncate (delete contents)
		if (!outfile.is_open()) {
			cerr << "Error opening the file!" << endl;
			return;
		}

		cout << "File " << "./Data/Output.txt" << " opened and cleared." << endl;

		outfile.close();
	}
};

