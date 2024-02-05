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

	string Consrtaint_9() {

	}
};

