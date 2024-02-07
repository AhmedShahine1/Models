#pragma once
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
			for (int j = 0; j < numberofAirCrafts; ++j)
			{
				cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeM[i].getFrom() << "-" << legsOfTypeM[i].getTo() << "]" << " + ";
				if (j == numberofAirCrafts - 1)
					cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeM[i].getFrom() << "-" << legsOfTypeM[i].getTo() << "]";
			}
			cout << endl << "----------------------------------------" << endl;
		}
	}

	void Consrtaint_2()
	{
		for (int i = 0; i < numOfLegsOfTypeO; ++i)
		{
			for (int j = 0; j < numberofAirCrafts; ++j)
			{
				cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeO[i].getFrom() << "-" << legsOfTypeO[i].getTo() << "]" << " + ";
				if (j == numberofAirCrafts - 1)
					cout << "X[" << AirCrafts[j].getType() << "]" << "[" << legsOfTypeO[i].getFrom() << "-" << legsOfTypeO[i].getTo() << "]";
			}
			cout << endl << "----------------------------------------" << endl;
		}
	}

	void Consrtaint_5() {
		for (int i = 0; i < numOfPathsWithLegO; i++)
		{
			for (int j = 0; j < numberofAirCrafts; j++) {

				for (int x = 0; x < numOfLegsOfTypeO; x++)
				{
					cout << "Z (From:" << pathsWithTypeO[i].getFrom() << " ,To: " << pathsWithTypeO[i].getTo() << "),- ";

					cout << "X (" << AirCrafts[j].getType() << "),";

					if (x == numOfLegsOfTypeO - 1) {
						cout << legsOfTypeO[x].getType();
						cout << "]}" << endl;
					}
					else {
						cout << legsOfTypeO[x].getType() << "+";
						cout << "]}" << endl;
					}
				}
			}
			cout << endl << "----------------------------------------" << endl;
		}
	}

	void Consrtaint_6()
	{

		for (int optional_path_repeater = 0; optional_path_repeater < numOfPathsWithLegO; optional_path_repeater++)
		{
			cout << "Z ( From: " << pathsWithTypeO[optional_path_repeater].getFrom()<<" To: "<< pathsWithTypeO[optional_path_repeater].getTo() << ")" << "-";

			for (int aircrafts = 0; aircrafts < numberofAirCrafts; aircrafts++)
			{
				for (int optional_legs = 0; optional_legs < numOfLegsOfTypeO; optional_legs++)
				{
					cout << " X ( " << AirCrafts[aircrafts].getType() << " , From: " << legsOfTypeO[optional_legs].getFrom()<<" To: "<< legsOfTypeO[optional_legs].getTo() << " )";

					if (aircrafts != (numberofAirCrafts - 1))
						cout << " + ";

				}//end last loop 

			}// end second loop 


			cout << " >= " << " 1 - | L ( From: " << pathsWithTypeO[optional_path_repeater].getFrom()<<" To: "<< pathsWithTypeO[optional_path_repeater].getTo() << " ) | " << endl;
			cout << endl << "----------------------------------------" << endl;
		}//end first loop 

	}//end of function

	void Consrtaint_7()
	{
		for (int optional_legs_repeater = 0; optional_legs_repeater < numOfLegsOfTypeO; optional_legs_repeater++)
		{
			for (int aircraft = 0; aircraft < numberofAirCrafts; aircraft++)
			{
				cout << "X ( " << AirCrafts[aircraft].getType() << " , Form: " << legsOfTypeO[optional_legs_repeater].getFrom()<<" To: "<< legsOfTypeO[optional_legs_repeater].getTo()
					<< " ) ";

				// print the + for the summation equation 
				if (aircraft != (numberofAirCrafts - 1))
					cout << " + ";
			}


			cout << " <= ";

				for (int optional_paths_repeater = 0; optional_paths_repeater < numOfPathsWithLegO; ++optional_paths_repeater)
				{
					cout << "Z ( From: " << pathsWithTypeO[optional_paths_repeater].getFrom() << " To: " << pathsWithTypeO[optional_paths_repeater].getTo() << ")";
					if (optional_paths_repeater != (numOfPathsWithLegO - 1))
						cout << "+";

				}
				cout << endl << "----------------------------------------" << endl;
		}
	}

	void Consrtaint_8() {
		for (int i = 0; i < numberOfLegs; i++)
		{
			for (int j = 0; j < numberOfFareClasses; j++)
			{
				for (int K = 0; K < numPathsthatContainLeg[i]; K++)
				{
					if (K == 0) {
						cout << "[";
					}
					cout << "{Passengers( in path: (" << PathsthatContainLeg[i][K] << ") ,in Fareclass(" << FareClasses[j] << ")}";
					if (K != numPathsthatContainLeg[i] - 1)
					{
						cout << " + ";
					}
					else {
						cout << "] <= ";
					}
				}

				for (int s = 0; s < numberofAirCrafts; s++)
				{
					if (s == 0) {
						cout << "[";
					}
					cout << "[X (Assign Aircrafts Type: " << AirCrafts[s].getType() << ", To Leg: " << Legs[i].getFrom() << ", " << Legs[i].getTo() << ") * ( Min{ (Capacity in Fare Class: " << FareClasses[j] << ", in Aircraft Type: " << AirCrafts[s].getType() << ") , ";
					for (int K = 0; K < numPathsthatContainLeg[i]; K++)
					{
						if (K == 0) {
							cout << "(";
						}
						cout << "Mean Demand( in path: (" << PathsthatContainLeg[i][K] << ") ,in Fareclass(" << FareClasses[j] << ")";
						if (K != numPathsthatContainLeg[i] - 1)
						{
							cout << " + ";
						}
						else {
							cout << ") }]" << endl;
						}
					}
					if (s != numberofAirCrafts - 1)
					{
						cout << " + ";
					}
					else {
						cout << endl;
					}
				}
			}
			cout << endl << "----------------------------------------" << endl;
		}
	}

	void Consrtaint_9() {
		for (int i = 0; i < numOfPathsWithLegO; i++)
		{
			for (int j = 0; j < numberOfFareClasses; j++) {
				cout << "passengers(From: "<< pathsWithTypeO[i].getFrom()<<" ,To: "<< pathsWithTypeO[i].getTo()<<" ,Fare Class: "<<FareClasses[j]<<") <= Min{Mean Demand(From: " << pathsWithTypeO[i].getFrom() << " ,To: " << pathsWithTypeO[i].getTo() << " ,Fare Class: " << FareClasses[j]<<"),Max Cap[";
				for (int x = 0; x < numberofAirCrafts; x++)
				{
					cout << "Cap(" << AirCrafts[x].getType() << ","<<FareClasses[j]<<"), ";
				}
				cout << "]} * Z (From: " << pathsWithTypeO[i].getFrom() << " ,To: " << pathsWithTypeO[i].getTo() << ")" << endl;
			}
			cout << endl << "----------------------------------------" << endl;
		}
	}

	void Consrtaint_10() {
		for (int i = 0; i < numberOfPaths; i++)
		{
			for (int j = 0; j < numberOfFareClasses; j++) {
				cout << "passengers(From: " << Paths[i].getFrom() << " ,To: " << Paths[i].getTo() << " ,Fare Class: " << FareClasses[j] << ") <=  Demand " << Paths[i].getFrom() << " ,To: " << Paths[i].getTo() << " ,Fare Class: " << FareClasses[j] << ")";
				cout << "]}" << endl;
			}
			cout << endl << "----------------------------------------" << endl;
		}
	}
};

