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
	//Parameters
	int** Caj;
	int** Fph;

    Constraints(string** PathsthatContainLeg,int* numPathsthatContainLeg, int numAirCrafts, int numPaths, int numLegs, int numFareClasses, int numOfLegsOfTypeO, int numOfLegsOfTypeM, int numOfPathsWithLegO, int numOfPathsWithLegM, Leg* Legs, Path* Paths, AirCraft* AirCrafts, string* FareClasses, int** Caj, int** Fph, Path* pathsWithTypeM, Path* pathsWithTypeO, Leg* legsOfTypeM, Leg* legsOfTypeO)
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
        Caj(Caj),
        Fph(Fph),
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
					cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeM[i].getFrom() << "-" << legsOfTypeM[i].getTo() << "] = 1";
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeM[i].getFrom() + "-" + legsOfTypeM[i].getTo() + "] = 1";
				}
				else
				{
					cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeM[i].getFrom() << "-" << legsOfTypeM[i].getTo() << "]" << " + ";
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeM[i].getFrom() + "-" + legsOfTypeM[i].getTo() + "]" + " + ";
				}
			}
			cout << endl << "----------------------------------------" << endl;
			WriteData(Output);
		}
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
					cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeO[i].getFrom() << "-" << legsOfTypeO[i].getTo() << "] <= 1";
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeO[i].getFrom() + "-" + legsOfTypeO[i].getTo() + "] <= 1";
				}
				else
				{
					cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeO[i].getFrom() << "-" << legsOfTypeO[i].getTo() << "]" << " + ";
					Output += "X[" + AirCrafts[j].getType() + "]" + "[" + legsOfTypeO[i].getFrom() + "-" + legsOfTypeO[i].getTo() + "]" + " + ";
				}
			}
			WriteData(Output);
			cout << endl << "----------------------------------------" << endl;
		}
	}


	void Consrtaint_5() {
		for (int i = 0; i < numOfPathsWithLegO; i++)
		{
			string Output;
			for (int j = 0; j < numOfLegsOfTypeO; j++) {

				for (int x = 0; x < numberofAirCrafts; x++)
				{
					cout << "Z (From:" << pathsWithTypeO[i].getFrom() << " ,To: " << pathsWithTypeO[i].getTo() << ") - " << "X (" << AirCrafts[x].getType() << "),";
					Output += "Z (From:" + pathsWithTypeO[i].getFrom() + " ,To: " + pathsWithTypeO[i].getTo() + ") - " + "X (" + AirCrafts[x].getType() + "),";
					if (x == numberofAirCrafts - 1) {
						cout << legsOfTypeO[j].getFrom() << ", " << legsOfTypeO[j].getTo();
						cout << "]} <= 0" << endl;
						Output += legsOfTypeO[j].getFrom() + ", " + legsOfTypeO[j].getTo() + "]} <= 0";
					}
					else {
						cout << legsOfTypeO[j].getFrom() << ", " << legsOfTypeO[j].getTo() << "+";
						cout << "]} <= 0" << endl;
						Output += legsOfTypeO[j].getFrom() + ", " + legsOfTypeO[j].getTo() + "+" + "]} <= 0";
					}
				}
			}
			WriteData(Output);
			cout << endl << "----------------------------------------" << endl;
		}
	}

	void Consrtaint_6()
	{

		for (int optional_path_repeater = 0; optional_path_repeater < numOfPathsWithLegO; optional_path_repeater++)
		{
			string Output;
			cout << "Z ( From: " << pathsWithTypeO[optional_path_repeater].getFrom() << " To: " << pathsWithTypeO[optional_path_repeater].getTo() << ")" << "-";
			Output += "Z ( From: " + pathsWithTypeO[optional_path_repeater].getFrom() + " To: " + pathsWithTypeO[optional_path_repeater].getTo() + ")" + "-";
			for (int aircrafts = 0; aircrafts < numberofAirCrafts; aircrafts++)
			{
				for (int optional_legs = 0; optional_legs < numOfLegsOfTypeO; optional_legs++)
				{
					cout << " X ( " << AirCrafts[aircrafts].getType() << " , From: " << legsOfTypeO[optional_legs].getFrom() << " To: " << legsOfTypeO[optional_legs].getTo() << " )";
					Output += " X ( " + AirCrafts[aircrafts].getType() + " , From: " + legsOfTypeO[optional_legs].getFrom() + " To: " + legsOfTypeO[optional_legs].getTo() + " )";
					if (aircrafts != (numberofAirCrafts - 1))
					{
						cout << " + ";
						Output += " + ";
					}
				}//end last loop 

			}// end second loop 


			cout << " >= " << " 1 - | L ( From: " << pathsWithTypeO[optional_path_repeater].getFrom() << " To: " << pathsWithTypeO[optional_path_repeater].getTo() << " ) | " << endl;
			Output += " >= 1 - | L ( From: " + pathsWithTypeO[optional_path_repeater].getFrom() + " To: " + pathsWithTypeO[optional_path_repeater].getTo() + " ) | ";
			cout << endl << "----------------------------------------" << endl;
			WriteData(Output);
		}//end first loop 

	}//end of function

	void Consrtaint_7()
	{
		for (int optional_legs_repeater = 0; optional_legs_repeater < numOfLegsOfTypeO; optional_legs_repeater++)
		{
			string Output;
			for (int aircraft = 0; aircraft < numberofAirCrafts; aircraft++)
			{
				cout << "X ( " << AirCrafts[aircraft].getType() << " , Form: " << legsOfTypeO[optional_legs_repeater].getFrom() << " To: " << legsOfTypeO[optional_legs_repeater].getTo()
					<< " ) ";
				Output += "X ( " + AirCrafts[aircraft].getType() + " , Form: " + legsOfTypeO[optional_legs_repeater].getFrom() + " To: " + legsOfTypeO[optional_legs_repeater].getTo() + " ) ";
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
				cout << "Z ( From: " << pathsWithTypeO[optional_paths_repeater].getFrom() << " To: " << pathsWithTypeO[optional_paths_repeater].getTo() << ")";
				Output += "Z ( From: " + pathsWithTypeO[optional_paths_repeater].getFrom() + " To: " + pathsWithTypeO[optional_paths_repeater].getTo() + ")";
				if (optional_paths_repeater != (numOfPathsWithLegO - 1)) {
					cout << "+";
					Output += "+";
				}

			}
			WriteData(Output);
			cout << endl << "----------------------------------------" << endl;
		}
	}


	void Constraint_8() {
		for (int i = 0; i < numberOfLegs; i++) {

			for (int j = 0; j < numberOfFareClasses; j++)
			{
				string Output = "[";
				for (int p = 0; p < numberOfPaths; p++)
				{
					if (Paths[p].hasLeg(Legs[i].getTo(), Legs[i].getFrom()))
					{
						if (p == 0) {
							Output = "[";
						}
						Output += "{Passengers( in path: (" + Paths[p].getTo() + ", " + Paths[p].getFrom() + ") ,in Fareclass(" + Paths[p].fareClasses[j].name + ")}";
						if (p != numberOfPaths - 1 && Paths[p+1].hasLeg(Legs[i].getTo(), Legs[i].getFrom()))
						{
							Output += " + ";
						}
						else if (p == numberOfPaths - 1){
							Output += "] <= ";
						}
					}
					else {
						if (p != 0 && p != numberOfPaths - 1 && Paths[p + 1].hasLeg(Legs[i].getTo(), Legs[i].getFrom()) && Output != "[")
						{
							Output += " + ";
						}
						else if (p == numberOfPaths - 1) {
							Output += "] <= ";
						}
					}
				}


				for (int s = 0; s < numberofAirCrafts; s++)
				{
					if (s == 0) {
						Output += "[";
					}
					for (int p = 0; p < numberOfPaths; p++)
					{
						if (p == 0){
							Output += "[X (Assign Aircrafts Type: " + AirCrafts[s].getType() + ", To Leg: " + Legs[i].getFrom() + ", " + Legs[i].getTo() + ") * ( Min{ (Capacity in Fare Class: " + Paths[p].fareClasses[j].name + ", in Aircraft Type: " + AirCrafts[s].getType() + ") , (";
						}
						if (Paths[p].hasLeg(Legs[i].getTo(), Legs[i].getFrom()))
						{
							Output += "Mean Demand( in path: (" + Paths[p].getTo()+ ", "+ Paths[p].getFrom() + ") ,in Fareclass(" + Paths[p].fareClasses[j].name + ") +";
							if (p == numberOfPaths - 1) {
								Output = Output.substr(0, Output.size()-2);
								Output += "] ";
							}
						}
						else
						{
							if (p == numberOfPaths - 1) {
								Output = Output.substr(0, Output.size() - 2);
								Output += ") }]";
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
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
			
		}
	}
	void Consrtaint_9() {
		for (int i = 0; i < numOfPathsWithLegO; i++)
		{
			string Output;
			for (int j = 0; j < numberOfFareClasses; j++) {
				cout << "passengers(From: "<< pathsWithTypeO[i].getFrom()<<" ,To: "<< pathsWithTypeO[i].getTo()<<" ,Fare Class: "<<FareClasses[j]<<") <= Min{Mean Demand(From: " << pathsWithTypeO[i].getFrom() << " ,To: " << pathsWithTypeO[i].getTo() << " ,Fare Class: " << FareClasses[j]<<"),Max Cap[";
				Output = "passengers(From: " + pathsWithTypeO[i].getFrom() + " ,To: " + pathsWithTypeO[i].getTo() + " ,Fare Class: " + FareClasses[j] + ") <= Min{Mean Demand(From: " + pathsWithTypeO[i].getFrom() + " ,To: " + pathsWithTypeO[i].getTo() + " ,Fare Class: " + FareClasses[j] + "),Max Cap[";
				for (int x = 0; x < numberofAirCrafts; x++)
				{
					cout << "Cap(" << AirCrafts[x].getType() << ","<<FareClasses[j]<<"), ";
					Output += "Cap(" + AirCrafts[x].getType() + "," + FareClasses[j] + "), ";
				}
				cout << "]} * Z (From: " << pathsWithTypeO[i].getFrom() << " ,To: " << pathsWithTypeO[i].getTo() << ")" << endl;
				Output += "]} * Z (From: " + pathsWithTypeO[i].getFrom() + " ,To: " + pathsWithTypeO[i].getTo() + ")";
				WriteData(Output);
			}
			cout << endl << "----------------------------------------" << endl;
		}
	}

	void Consrtaint_10() {
		for (int i = 0; i < numberOfPaths; i++)
		{
			string Output;
			for (int j = 0; j < numberOfFareClasses; j++) {
				cout << "passengers(From: " << Paths[i].getFrom() << " ,To: " << Paths[i].getTo() << " ,Fare Class: " << FareClasses[j] << ") <=  [Demand (" << Paths[i].getFrom() << " ,To: " << Paths[i].getTo() << " ,Fare Class: " << FareClasses[j] << ")]}" << endl;
				Output = "passengers(From: " + Paths[i].getFrom() + " ,To: " + Paths[i].getTo() + " ,Fare Class: " + FareClasses[j] + ") <=  [Demand (" + Paths[i].getFrom() + " ,To: " + Paths[i].getTo() + " ,Fare Class: " + FareClasses[j] + ")]}";
				WriteData(Output);
			}
			cout << endl << "----------------------------------------" << endl;
		}
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

