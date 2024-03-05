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

    Constraints(string** PathsthatContainLeg,int* numPathsthatContainLeg, int numAirCrafts, int numPaths, int numLegs, int numFareClasses, int numOfLegsOfTypeO, int numOfLegsOfTypeM, int numOfPathsWithLegO, int numOfPathsWithLegM, Leg* Legs, Path* Paths, AirCraft* AirCrafts, string* FareClasses, Path* pathsWithTypeM, Path* pathsWithTypeO, Leg* legsOfTypeM, Leg* legsOfTypeO)
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
		legsOfTypeO(legsOfTypeO) {
    }
	~Constraints() {}

	void Consrtaint_1()
	{
		for (int i = 0; i < numOfLegsOfTypeM; ++i)
		{
			string Output;
			for (int j = 0; j < numberofAirCrafts; ++j)
			{
				if (j == numberofAirCrafts - 1)
				{
					cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeM[i].getFrom() << "," << legsOfTypeM[i].getTo() << "] = 1"<<endl;
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeM[i].getFrom() + "," + legsOfTypeM[i].getTo() + "] = 1";
				}
				else
				{
					cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeM[i].getFrom() << "," << legsOfTypeM[i].getTo() << "]" << " + ";
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeM[i].getFrom() + "," + legsOfTypeM[i].getTo() + "]" + " + ";
				}
			}
			WriteData(Output);
		}
		cout << endl << "----------------------------------------" << endl;
	}

	void Consrtaint_2()
	{
		for (int i = 0; i < numOfLegsOfTypeO; ++i)
		{
			string Output;
			for (int j = 0; j < numberofAirCrafts; ++j)
			{
				if (j == numberofAirCrafts - 1)
				{
					cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeO[i].getFrom() << "," << legsOfTypeO[i].getTo() << "] <= 1";
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeO[i].getFrom() + "," + legsOfTypeO[i].getTo() + "] <= 1";
				}
				else
				{
					cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeO[i].getFrom() << "," << legsOfTypeO[i].getTo() << "]" << " + ";
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeO[i].getFrom() + "," + legsOfTypeO[i].getTo() + "]" + " + ";
				}
			}
			WriteData(Output);
		}
		cout << endl << "----------------------------------------" << endl;
	}

	void Consrtaint_5() {
		for (int i = 0; i < numOfPathsWithLegO; i++)
		{
			string Output;
			cout << "Z[" << pathsWithTypeO[i].getFrom() << "," << pathsWithTypeO[i].getTo() << "] - {" ;
			Output += "Z[" + pathsWithTypeO[i].getFrom() + "," + pathsWithTypeO[i].getTo() + "] - {";
			for (int j = 0; j < numOfLegsOfTypeO; j++) {

				for (int x = 0; x < numberofAirCrafts; x++)
				{
					if (x == numberofAirCrafts - 1) {
						cout << "X[" << AirCrafts[x].getType() << "][" << legsOfTypeO[j].getFrom() << "," << legsOfTypeO[j].getTo();
						cout << "]} <= 0" << endl;
						Output += "X[" + AirCrafts[x].getType() + "][" +legsOfTypeO[j].getFrom() + "," + legsOfTypeO[j].getTo() + "]} <= 0";
					}
					else {
						cout << "X[" << AirCrafts[x].getType() << "][" << legsOfTypeO[j].getFrom() << "," << legsOfTypeO[j].getTo() << "] + ";
						Output += "X[" + AirCrafts[x].getType() + "][" + legsOfTypeO[j].getFrom() + "," + legsOfTypeO[j].getTo() + "] + ";
					}
				}
			}
			WriteData(Output);
		}
		cout << endl << "----------------------------------------" << endl;
	}

	void Consrtaint_6()
	{

		for (int optional_path_repeater = 0; optional_path_repeater < numOfPathsWithLegO; optional_path_repeater++)
		{
			string Output;
			cout << "Z[" << pathsWithTypeO[optional_path_repeater].getFrom() << "," << pathsWithTypeO[optional_path_repeater].getTo() << "]" << " - ";
			Output += "Z[" + pathsWithTypeO[optional_path_repeater].getFrom() + "," + pathsWithTypeO[optional_path_repeater].getTo() + "]" + " - ";
			for (int aircrafts = 0; aircrafts < numberofAirCrafts; aircrafts++)
			{
				for (int optional_legs = 0; optional_legs < numOfLegsOfTypeO; optional_legs++)
				{
					cout << " X[" << AirCrafts[aircrafts].getType() << "][" << legsOfTypeO[optional_legs].getFrom() << "," << legsOfTypeO[optional_legs].getTo() << "]";
					Output += " X[" + AirCrafts[aircrafts].getType() + "][" + legsOfTypeO[optional_legs].getFrom() + "," + legsOfTypeO[optional_legs].getTo() + "]";
					if (aircrafts != (numberofAirCrafts - 1))
					{
						cout << " + ";
						Output += " + ";
					}
				}//end last loop 

			}// end second loop 


			cout << " >= " << " 1 - | L[" << pathsWithTypeO[optional_path_repeater].getFrom() << "," << pathsWithTypeO[optional_path_repeater].getTo() << "] | " << endl;
			Output += " >= 1 - | L[" + pathsWithTypeO[optional_path_repeater].getFrom() + "," + pathsWithTypeO[optional_path_repeater].getTo() + "] | ";
			WriteData(Output);
		}//end first loop 
		cout << endl << "----------------------------------------" << endl;
	}//end of function

	void Consrtaint_7()
	{
		for (int optional_legs_repeater = 0; optional_legs_repeater < numOfLegsOfTypeO; optional_legs_repeater++)
		{
			string Output;
			for (int aircraft = 0; aircraft < numberofAirCrafts; aircraft++)
			{
				cout << "X[" << AirCrafts[aircraft].getType() << "][" << legsOfTypeO[optional_legs_repeater].getFrom() << "," << legsOfTypeO[optional_legs_repeater].getTo()
					<< "]";
				Output += "X[" + AirCrafts[aircraft].getType() + "][" + legsOfTypeO[optional_legs_repeater].getFrom() + "," + legsOfTypeO[optional_legs_repeater].getTo() + "]";
				// print the + for the summation equation 
				if (aircraft != (numberofAirCrafts - 1))
				{
					cout << " + ";
					Output += " + ";
				}
			}


			cout << " <= ";
			Output += " <= ";
			for (int optional_paths_repeater = 0; optional_paths_repeater < numOfPathsWithLegO; ++optional_paths_repeater)
			{
				cout << "Z[" << pathsWithTypeO[optional_paths_repeater].getFrom() << "," << pathsWithTypeO[optional_paths_repeater].getTo() << "]";
				Output += "Z[" + pathsWithTypeO[optional_paths_repeater].getFrom() + "," + pathsWithTypeO[optional_paths_repeater].getTo() + "]";
				if (optional_paths_repeater != (numOfPathsWithLegO - 1)) {
					cout << "+";
					Output += "+";
				}

			}
			WriteData(Output);
		}
		cout << endl << "----------------------------------------" << endl;
	}

	void Constraint_8() {
		for (int i = 0; i < numberOfLegs; i++) {

			for (int j = 0; j < numberOfFareClasses; j++)
			{
				string Output ;
				int Count = 0;
				for (int p = 0; p < numberOfPaths; p++)
				{
					if (Paths[p].hasLeg(Legs[i].getTo(), Legs[i].getFrom()))
					{
						Output += "P[" + Paths[p].getFrom() + "," + Paths[p].getTo() + "][" + Paths[p].getFareClasses()[j].getName() + "]";
						if (p != numberOfPaths - 1 && Paths[p+1].hasLeg(Legs[i].getTo(), Legs[i].getFrom()) )
						{
							Output += " + ";
						}
						else if (p == numberOfPaths - 1){
							Output += " <= ";
						}
						Count++;
					}
					else {
						if (p != 0 && p != numberOfPaths - 1 && Paths[p + 1].hasLeg(Legs[i].getTo(), Legs[i].getFrom()) && Count != 0)
						{
							Output += " + ";
						}
						else if (p == numberOfPaths - 1) {
							Output += " <= ";
						}
					}
				}


				for (int s = 0; s < numberofAirCrafts; s++)
				{
					for (int p = 0; p < numberOfPaths; p++)
					{
						if (p == 0){
							Output += "X[" + AirCrafts[s].getType() + "][" + Legs[i].getFrom() + "," + Legs[i].getTo() + "] * Min{Cap[" + Paths[p].getFareClasses()[j].getName() + "][" + AirCrafts[s].getType() + "], ";
						}
						if (Paths[p].hasLeg(Legs[i].getTo(), Legs[i].getFrom()))
						{
							Output += "M[" + Paths[p].getFrom() + "," + Paths[p].getTo() + "][" + Paths[p].getFareClasses()[j].getName() + "] + ";
							if (p == numberOfPaths - 1) {
								Output = Output.substr(0, Output.size()-2);
								Output += "} ";
							}
						}
						else
						{
							if (p == numberOfPaths - 1) {
								Output = Output.substr(0, Output.size() - 2);
								Output += "}";
							}
						}
					}
					if (s != numberofAirCrafts - 1)
					{
						
						Output += " + ";
					}
					else {
						Output += "";
					}
				}
				WriteData(Output);
				cout << Output << endl;
			}
		}
		cout << "----------------------------------------------------------------------------------------" << endl;
	}

	void Consrtaint_9() {
		for (int i = 0; i < numOfPathsWithLegO; i++)
		{
			string Output;
			for (int j = 0; j < numberOfFareClasses; j++) {
				cout << "P["<< pathsWithTypeO[i].getFrom()<<","<< pathsWithTypeO[i].getTo()<<"]["<<FareClasses[j]<<"] <= Min{M[" << pathsWithTypeO[i].getFrom() << "," << pathsWithTypeO[i].getTo() << "][" << FareClasses[j]<<"],OCap[";
				Output = "P[" + pathsWithTypeO[i].getFrom() + "," + pathsWithTypeO[i].getTo() + "][" + FareClasses[j] + "] <= Min{M[" + pathsWithTypeO[i].getFrom() + "," + pathsWithTypeO[i].getTo() + "][" + FareClasses[j] + "],OCap[";
				for (int x = 0; x < numberofAirCrafts; x++)
				{
					cout << "Cap[" << AirCrafts[x].getType() << ","<<FareClasses[j]<<"],";
					Output += "Cap[" + AirCrafts[x].getType() + "," + FareClasses[j] + "],";
				}
				cout << "]} * Z[" << pathsWithTypeO[i].getFrom() << "," << pathsWithTypeO[i].getTo() << "]" << endl;
				Output += "]} * Z[" + pathsWithTypeO[i].getFrom() + "," + pathsWithTypeO[i].getTo() + "]";
				WriteData(Output);
			}
		}
		cout << endl << "----------------------------------------" << endl;
	}

	void Consrtaint_10() {
		for (int i = 0; i < numberOfPaths; i++)
		{
			string Output;
			for (int j = 0; j < numberOfFareClasses; j++) {
				cout << "P[" << Paths[i].getFrom() << "," << Paths[i].getTo() << "][" << FareClasses[j] << "] <=  D[" << Paths[i].getFrom() << "," << Paths[i].getTo() << "][" << FareClasses[j] << "]" << endl;
				Output = "P[" + Paths[i].getFrom() + "," + Paths[i].getTo() + "][" + FareClasses[j] + "] <=  D[" + Paths[i].getFrom() + "," + Paths[i].getTo() + "][" + FareClasses[j] + "]";
				WriteData(Output);
			}
		}
		cout << endl << "----------------------------------------" << endl;
	}

	void WriteData(string data) {
		ofstream outputFile("Output.txt", ios::app);

		// Check if the file is successfully opened
		if (!outputFile.is_open()) {
			cout << "Error opening the file!" << endl;
		}
		outputFile << data << endl;
		outputFile.close();
	}
};

