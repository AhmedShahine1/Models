#pragma once
#include <iostream>
#include <fstream>
#include "Path.h"
#include "AirCraft.h"
#include "Leg.h"
#include "Constraints.h"
#include "FareClass.h"
#include "Cplex.h"
#include"ilcplex/ilocplex.h";
#include<chrono>;
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;
//M=Mean demand,P=Passanger,Cap=capacity,F=estimated price,Caj=Cost
#pragma region Function
void displayAircraftDetails(AirCraft* aircraftArray, int numOfAircraft) {
    for (int i = 0; i < numOfAircraft; ++i) {
        cout << "\nAircraft " << i + 1 << " Details:" << endl;
        aircraftArray[i].displayDetails();
    }
}

// Function to display information about each flight leg in the array
void displayLegsDetails(Leg* legs, int numOfFlightLegs) {
    for (int K = 0; K < numOfFlightLegs; ++K) {
        cout << "\nFlight Legs Id: " << K + 1 << ", Details:" << endl;
        legs[K].display();
    }
}

// Function to display information about each path in the array
void displayPathsDetails(Path* paths, int numOfPaths) {
    for (int i = 0; i < numOfPaths; i++) {
        cout << "\nPath " << i + 1 << " Details:" << endl;
        paths[i].display();
    }
}

// Function to get all paths with at least one leg of type "m"
Path* getAllPathsWithLegOfTypeM(const Path* paths, int numOfPaths, int& numOfPathsWithLegM) {
    // Count the number of paths with at least one leg of type "m"
    for (int i = 0; i < numOfPaths; ++i) {
        if (paths[i].hasLegOfTypeM()) {
            ++numOfPathsWithLegM;
        }
    }

    // Create a dynamic array to store paths with leg type "m"
    Path* pathsWithTypeM = new Path[numOfPathsWithLegM];
    int currentIndex = 0;

    // Populate the dynamic array with paths having leg type "m"
    for (int i = 0; i < numOfPaths; ++i) {
        if (paths[i].hasLegOfTypeM()) {
            pathsWithTypeM[currentIndex++] = paths[i];
        }
    }

    return pathsWithTypeM;
}

// Function to get all paths with at least one leg of type "o"
Path* getAllPathsWithLegOfTypeO(const Path* paths, int numOfPaths, int& numOfPathsWithLegO) {
    // Count the number of paths with at least one leg of type "o"
    for (int i = 0; i < numOfPaths; ++i) {
        if (paths[i].hasLegOfTypeO()) {
            ++numOfPathsWithLegO;
        }
    }

    // Create a dynamic array to store paths with leg type "o"
    Path* pathsWithTypeM = new Path[numOfPathsWithLegO];
    int currentIndex = 0;

    // Populate the dynamic array with paths having leg type "o"
    for (int i = 0; i < numOfPaths; ++i) {
        if (paths[i].hasLegOfTypeO()) {
            pathsWithTypeM[currentIndex++] = paths[i];
        }
    }

    return pathsWithTypeM;
}

// Function to get all legs with a specific type from an array of legs
Leg* getLegsByType(Leg* legs, int legCount, const string& targetType, int& numOfLegCount) {
    Leg* legsOfType = new Leg[legCount];
    for (int i = 0; i < legCount; ++i) {
        if (legs[i].hasType(targetType)) {
            legsOfType[numOfLegCount++] = legs[i];
        }
    }
    return legsOfType;
}

// Function to start system
void System(string** PathsthatContainLeg, int* numPathsthatContainLeg, int numAirCrafts, int numPaths, int numLegs, int numFareClasses, int numOfLegsOfTypeO, int numOfLegsOfTypeM, int numOfPathsWithLegO, int numOfPathsWithLegM, int CountU, int CountV, Leg* Legs, Path* Paths, AirCraft* AirCrafts, string* FareClasses, Path* pathsWithTypeM, Path* pathsWithTypeO, Leg* legsOfTypeM, Leg* legsOfTypeO, Leg** U, Leg** V, double** S) {
#pragma region System
    try
    {
        Constraints constrain = Constraints(PathsthatContainLeg, numPathsthatContainLeg, numAirCrafts, numPaths, numLegs, numFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM, CountU, CountV, Legs, Paths, AirCrafts, FareClasses, pathsWithTypeM, pathsWithTypeO, legsOfTypeM, legsOfTypeO, U, V, S);
        constrain.ClearData();
        constrain.Constrain_1();
        constrain.Constrain_2();
        constrain.Constrain_3();
        constrain.Constrain_4();
        constrain.Constrain_5();
        constrain.Constrain_6();
        constrain.Constrain_7();
        constrain.Constrain_8();
        constrain.Constrain_9();
        constrain.Constrain_10();
        constrain.Constrain_11();
        constrain.Constrain_12();
        constrain.Constrain_13();
        constrain.Constrain_14();
        constrain.Constrain_15();
        constrain.Constrain_16();
        constrain.Constrain_17();
        constrain.Constrain_18();
        constrain.Constrain_19();
        constrain.Constrain_20();
    }
    catch (const exception&)
    {
        cout << endl << "!\tError in write model\t!" << endl;
    }
    try
    {

        Cplex cplex = Cplex(PathsthatContainLeg, numPathsthatContainLeg, numAirCrafts, numPaths, numLegs, numFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM, CountU, CountV, Legs, Paths, AirCrafts, FareClasses, pathsWithTypeM, pathsWithTypeO, legsOfTypeM, legsOfTypeO, U, V, S);
        cplex.System();

    }
    catch (const exception&)
    {
        cout << endl << "!\tError in solve model\t!" << endl;
    }
#pragma endregion

}
#pragma endregion

int main() {
#pragma region EnterData
    ifstream airCraftData("./Data/AirCraft.txt");
    if (!airCraftData.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream PathData("./Data/Path.txt");
    if (!PathData.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream LegData("./Data/Leg.txt");
    if (!LegData.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream FareClassData("./Data/FareClass.txt");
    if (!FareClassData.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream MeanDemandData("./Data/MeanDemand.txt");
    if (!MeanDemandData.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream CapacityData("./Data/Capacity.txt");
    if (!CapacityData.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream CostData("./Data/cost.txt");
    if (!CostData.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream PriceData("./Data/price.txt");
    if (!PriceData.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream TargetTime("./Data/TargetTime.txt");
    if (!TargetTime.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream EarlyTime("./Data/EarlyTime.txt");
    if (!EarlyTime.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream LateTime("./Data/LateTime.txt");
    if (!LateTime.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream EarlyPrice("./Data/EarlyPrice.txt");
    if (!EarlyPrice.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream LatePrice("./Data/LatePrice.txt");
    if (!LatePrice.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream RequiredTime("./Data/RequiredTime.txt");
    if (!LatePrice.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream UData("./Data/UData.txt");
    if (!LatePrice.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    ifstream VData("./Data/VData.txt");
    if (!LatePrice.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }
    // Aircraft Types
    int numOfAircraft;

    // TaKe input for the number of aircraft
    airCraftData >> numOfAircraft;
    // Create a dynamic array of AirCraft objects
    AirCraft* aircraftArray = new AirCraft[numOfAircraft];

    // Initialize each AirCraft object in the array
    for (int i = 0; i < numOfAircraft; ++i) {
        string type;
        int numberAirCraft;
        int numNodes;

        // TaKe input for each aircraft
        airCraftData >> type;

        airCraftData >> numberAirCraft;

        airCraftData >> numNodes;

        // Create AirCraft object and set values
        aircraftArray[i] = AirCraft(type, numberAirCraft, numNodes);

        // Input nodes
        string* nodeValues = new string[numNodes];
        for (int j = 0; j < numNodes; ++j) {
            airCraftData >> nodeValues[j];
        }

        // Set nodes values
        aircraftArray[i].setNodes(nodeValues, numNodes);
    }

    // Flight Legs
    int numOfFlightLegs;

    LegData >> numOfFlightLegs;
    Leg* legs = new Leg[numOfFlightLegs];
    string toLeg, fromLeg, typeLeg, toPath, fromPath;
    tm TL, TE, TT;
    string TimeL, TimeE, TimeT;
    int PE, PL;
    for (int i = 0; i < numOfFlightLegs; i++) {
        LegData >> fromLeg;
        LegData >> toLeg;
        LegData >> typeLeg;
        EarlyTime >> TimeE;
        TargetTime >> TimeT;
        LateTime >> TimeL;
        LatePrice >> PL;
        EarlyPrice >> PE;
        istringstream ss(TimeE);
        ss >> get_time(&TE, "%H:%M");
        istringstream sT(TimeT);
        sT >> get_time(&TT, "%H:%M");
        istringstream sL(TimeL);
        sL >> get_time(&TL, "%H:%M");
        legs[i] = Leg(toLeg, fromLeg, typeLeg);
        legs[i].setTE(TE);
        legs[i].setTT(TT);
        legs[i].setTL(TL);
        legs[i].setPE(PE);
        legs[i].setPL(PL);
    }

    // Fare Classes
    int numOfFareClasses;
    FareClassData >> numOfFareClasses;

    string* NameFareClasses = new string[numOfFareClasses]; //indexed by (h)
    for (int h = 0; h < numOfFareClasses; h++)
    {
        FareClassData >> NameFareClasses[h];
    }

    // Paths
    int numOfPaths;
    int numOfLegsBelongsToPath;
    PathData >> numOfPaths;
    Path* Paths = new Path[numOfPaths];
    for (int i = 0; i < numOfPaths; i++) {
        PathData >> fromPath;
        PathData >> toPath;
        PathData >> numOfLegsBelongsToPath;
        Paths[i] = Path(toPath, fromPath, numOfLegsBelongsToPath, numOfFareClasses);
        Leg* LegsToPath = new Leg[numOfLegsBelongsToPath];
        for (int j = 0; j < numOfLegsBelongsToPath; j++) {
            char LegsBelongsToPath;
            PathData >> LegsBelongsToPath;
            if (toupper(LegsBelongsToPath) == 'Y') {
                PathData >> fromLeg;
                PathData >> toLeg;
                PathData >> typeLeg;
                LegsToPath[j] = Leg(toLeg, fromLeg, typeLeg);
            }
            else {
                int idleg;
                PathData >> idleg;
                LegsToPath[j] = legs[idleg - 1];
            }
        }
        Paths[i].setLegs(LegsToPath, numOfLegsBelongsToPath);
    }

    string** PathsthatContainLeg = new string * [numOfFlightLegs];
    int* numPathsthatContainLeg = new int[numOfFlightLegs];

    for (int j = 0; j < numOfFlightLegs; j++)
    {
        numPathsthatContainLeg[j] = 0;
        for (int i = 0; i < numOfPaths; i++)
        {
            if (Paths[i].hasLeg(legs[j].getTo(), legs[j].getFrom()))
            {
                numPathsthatContainLeg[j]++;
            }
        }

        PathsthatContainLeg[j] = new string[numPathsthatContainLeg[j]];
        for (int k = 0; k < numPathsthatContainLeg[j]; k++) {
            for (int i = 0; i < numOfPaths; i++)
            {
                if (Paths[i].hasLeg(legs[j].getTo(), legs[j].getFrom()))
                {
                    PathsthatContainLeg[j][k] = Paths[i].getFrom() + " ," + Paths[i].getTo();
                }
            }
        }
    }
    //Parameters
    //set Mean Demand to FareClass in Path
    for (int i = 0; i < numOfPaths; i++) {
        FareClass* newFareClass = new FareClass[numOfFareClasses];
        for (int j = 0; j < numOfFareClasses; j++) {
            int Mph = 0;
            MeanDemandData >> Mph;
            newFareClass[j] = FareClass(NameFareClasses[j], 0, Mph, 0);
        }
        Paths[i].setFareClasses(newFareClass, numOfFareClasses);
    }
    //set Capacity to fareClass in AirCraft
    for (int i = 0; i < numOfAircraft; i++) {
        FareClass* newFareClass = new FareClass[numOfFareClasses];
        for (int j = 0; j < numOfFareClasses; j++) {
            int Capah = 0;
            CapacityData >> Capah;
            Capah = Capah * aircraftArray[i].getNumberAirCraft();
            newFareClass[j] = FareClass(NameFareClasses[j], Capah, 0, 0);
        }
        aircraftArray[i].setFareClasses(newFareClass, numOfFareClasses);
    }
    //set cost of assigning fleet type a to leg j
    for (int i = 0; i < numOfAircraft; i++) {
        Leg* newlegs = legs;
        for (int j = 0; j < numOfFlightLegs; j++) {
            int Cost = 0;
            CostData >> Cost;
            newlegs[j].setCost(Cost);
        }
        aircraftArray[i].setLegs(newlegs, numOfFlightLegs);
    }
    //set estimated price for fare class h on path p
    for (int i = 0; i < numOfPaths; i++) {
        FareClass* newFareClass = Paths[i].getFareClasses();
        for (int j = 0; j < Paths[i].getFareClassCount(); j++) {
            int Price = 0;
            PriceData >> Price;
            newFareClass[j].setPrice(Price);
        }
        Paths[i].setFareClasses(newFareClass, Paths[i].getFareClassCount());
    }

    int numOfPathsWithLegM = 0;
    // Get all paths with at least one leg of type "m"
    Path* pathsWithTypeM = getAllPathsWithLegOfTypeM(Paths, numOfPaths, numOfPathsWithLegM);

    int numOfPathsWithLegO = 0;
    // Get all paths with at least one leg of type "o"
    Path* pathsWithTypeO = getAllPathsWithLegOfTypeO(Paths, numOfPaths, numOfPathsWithLegO);

    int numOfLegsOfTypeM = 0;
    // Get all legs with type "m"
    Leg* legsOfTypeM = getLegsByType(legs, numOfFlightLegs, "Mandatory", numOfLegsOfTypeM);

    int numOfLegsOfTypeO = 0;
    // Get all legs with type "O"
    Leg* legsOfTypeO = getLegsByType(legs, numOfFlightLegs, "Optional", numOfLegsOfTypeO);

    int countV ;
    VData >> countV;
    Leg** V = new Leg*[countV];
    for (int i = 0; i < countV; i++) {
        int index1, index2;
        VData >> index1 >> index2;
            V[i] = new Leg[2];
            V[i][0] = legs[index1];
            V[i][1] = legs[index2];

    }
    int countU ;
    UData >> countU;
    Leg** U = new Leg * [countU];
    for (int i = 0; i < countU; i++) {
        int index1, index2;
        UData >> index1 >> index2;
        U[i] = new Leg[2];
        U[i][0] = legs[index1];
        U[i][1] = legs[index2];
    }
    double** S = new double*[numOfFlightLegs];
    for (int i = 0; i < numOfFlightLegs; i++) {
        S[i] = new double[numOfFlightLegs];
        for (int j = 0; j < numOfFlightLegs; j++) {
            RequiredTime >> S[i][j];
        }
    }
    System(PathsthatContainLeg, numPathsthatContainLeg, numOfAircraft, numOfPaths, numOfFlightLegs, numOfFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM, countU, countV, legs, Paths, aircraftArray, NameFareClasses, pathsWithTypeM, pathsWithTypeO, legsOfTypeM, legsOfTypeO, U, V, S);
    return 0;
#pragma endregion
/*[[0.25, 0.25],
[0.25, 0.25],
[0.25, 0.25]]
*/ 
}