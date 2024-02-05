#pragma once
class Constraints
{
public:
	int numberofAirCrafts, numberOfPaths, numberOfLegs, numberOfFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM;
	//Sets
	Leg* Legs;
	Path* Paths;
	AirCraft* AirCrafts;
	string* FareClasses;
	Path* pathsWithTypeM;
	Path* pathsWithTypeO;
	Leg* legsOfTypeM;
	Leg* legsOfTypeO;
	//Parameters
	int** Caj;
	int** Fph;

    Constraints(int numAirCrafts, int numPaths, int numLegs, int numFareClasses, int numOfLegsOfTypeO, int numOfLegsOfTypeM, int numOfPathsWithLegO, int numOfPathsWithLegM, Leg* Legs, Path* Paths, AirCraft* AirCrafts, string* FareClasses, int** Caj, int** Fph, Path* pathsWithTypeM, Path* pathsWithTypeO, Leg* legsOfTypeM, Leg* legsOfTypeO)
        : numberofAirCrafts(numAirCrafts),
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

	void Consrtaint_9() {
		for (int i = 0; i < numOfPathsWithLegO; i++)
		{
			for (int j = 0; j < numberOfFareClasses; j++) {
				cout << "passengers(From: "<< pathsWithTypeO[i].getFrom()<<" ,To: "<< pathsWithTypeO[i].getTo()<<" ,Fare Class: "<<FareClasses[j]<<") <= Min{Mean Demand(From: " << pathsWithTypeO[i].getFrom() << " ,To: " << pathsWithTypeO[i].getTo() << " ,Fare Class: " << FareClasses[j]<<"),Max Cap[";
				for (int x = 0; x < numberofAirCrafts; x++)
				{
					cout << "Cap(" << AirCrafts[x].getType() << ","<<FareClasses[j]<<"), ";
				}
				cout << "]}" << endl;
			}
		}
	}
};

