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
void System(string** PathsthatContainLeg, int* numPathsthatContainLeg, int numAirCrafts, int numPaths, int numLegs, int numFareClasses, int numOfLegsOfTypeO, int numOfLegsOfTypeM, int numOfPathsWithLegO, int numOfPathsWithLegM, Leg* Legs, Path* Paths, AirCraft* AirCrafts, string* FareClasses, Path* pathsWithTypeM, Path* pathsWithTypeO, Leg* legsOfTypeM, Leg* legsOfTypeO) {
#pragma region System
    system("cls");
    while (true)
    {
        int Decision;
        cout << "Enter 1 to write model or 2 to solve model or 3 to clear console or 4 to Exit :";
        cin >> Decision;
        if (Decision == 1) {
            try
            {
                Constraints constrain = Constraints(PathsthatContainLeg, numPathsthatContainLeg, numAirCrafts, numPaths, numLegs, numFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM, Legs, Paths, AirCrafts, FareClasses, pathsWithTypeM, pathsWithTypeO, legsOfTypeM, legsOfTypeO);
                cout << endl << "Constrain1:" << endl;
                constrain.Consrtaint_1();
                cout << endl << "Constrain2:" << endl;
                constrain.Consrtaint_2();
                cout << endl << "Constrain3:" << endl;
                constrain.Consrtaint_5();
                cout << endl << "Constrain4:" << endl;
                constrain.Consrtaint_6();
                cout << endl << "Constrain5:" << endl;
                constrain.Consrtaint_7();
                cout << endl << "Constrain7:" << endl;
                constrain.Constraint_8();
                cout << endl << "Constrain7:" << endl;
                constrain.Consrtaint_9();
                cout << endl << "Constrain9:" << endl;
                constrain.Consrtaint_10();
            }
            catch (const exception&)
            {
                cout << endl << "!\tError in write model\t!" << endl;
            }
        }
        else if (Decision == 2)
        {
            try
            {
                Cplex cplex = Cplex(PathsthatContainLeg, numPathsthatContainLeg, numAirCrafts, numPaths, numLegs, numFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM, Legs, Paths, AirCrafts, FareClasses, pathsWithTypeM, pathsWithTypeO, legsOfTypeM, legsOfTypeO);
                cplex.System();

            }
            catch (const exception&)
            {
                cout << endl << "!\tError in solve model\t!" << endl;
            }
        }
        else if (Decision == 3) {
            system("cls");
        }
        else
        {
            break;
        }
    }
#pragma endregion

}
#pragma endregion

int main() {
#pragma region EnterData
    int readData = 0;
    cout << "Enter 1- To read data from file 2- To enter data: ";
    cin >> readData;
    if (readData == 1) {
        ifstream airCraftData("AirCraft.txt");
        if (!airCraftData.is_open()) {
            cout << "Error opening file." << endl;
            return 1;
        }
        ifstream PathData("Path.txt");
        if (!PathData.is_open()) {
            cout << "Error opening file." << endl;
            return 1;
        }
        ifstream LegData("Leg.txt");
        if (!LegData.is_open()) {
            cout << "Error opening file." << endl;
            return 1;
        }
        ifstream FareClassData("FareClass.txt");
        if (!FareClassData.is_open()) {
            cout << "Error opening file." << endl;
            return 1;
        }
        ifstream MeanDemandData("MeanDemand.txt");
        if (!MeanDemandData.is_open()) {
            cout << "Error opening file." << endl;
            return 1;
        }
        ifstream CapacityData("Capacity.txt");
        if (!CapacityData.is_open()) {
            cout << "Error opening file." << endl;
            return 1;
        }
        ifstream CostData("cost.txt");
        if (!CostData.is_open()) {
            cout << "Error opening file." << endl;
            return 1;
        }
        ifstream PriceData("price.txt");
        if (!PriceData.is_open()) {
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
        for (int i = 0; i < numOfFlightLegs; i++) {
            LegData >> fromLeg;
            LegData >> toLeg;
            LegData >> typeLeg;
            legs[i] = Leg(toLeg, fromLeg, typeLeg);
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
            Paths[i].setLegs(LegsToPath,numOfLegsBelongsToPath);
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
            Paths[i].setFareClasses(newFareClass,numOfFareClasses);
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
            aircraftArray[i].setFareClasses(newFareClass,numOfFareClasses);
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
            Paths[i].setFareClasses(newFareClass,Paths[i].getFareClassCount());
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

        System(PathsthatContainLeg, numPathsthatContainLeg, numOfAircraft, numOfPaths, numOfFlightLegs, numOfFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM, legs, Paths, aircraftArray, NameFareClasses, pathsWithTypeM, pathsWithTypeO, legsOfTypeM, legsOfTypeO);
    }
    else
    {
        // Aircraft Types
        int numOfAircraft;

        // TaKe input for the number of aircraft
        cout << "Enter the number of Aircraft Type: ";
        cin >> numOfAircraft;

        // Create a dynamic array of AirCraft objects
        AirCraft* aircraftArray = new AirCraft[numOfAircraft];

        // Initialize each AirCraft object in the array
        for (int i = 0; i < numOfAircraft; ++i) {
            string type;
            int numberAirCraft;
            int numNodes;

            // TaKe input for each aircraft
            cout << "Aircraft [" << i + 1 << "]\nEnter Aircraft Type : ";
            cin >> type;

            cout << "Enter Number of All Aircraft: ";
            cin >> numberAirCraft;

            cout << "Enter Number of Nodes: ";
            cin >> numNodes;

            // Create AirCraft object and set values
            aircraftArray[i] = AirCraft(type, numberAirCraft, numNodes);

            // Input nodes
            string* nodeValues = new string[numNodes];
            for (int j = 0; j < numNodes; ++j) {
                cout << "Enter Node " << j + 1 << ": ";
                cin >> nodeValues[j];
            }

            // Set nodes values
            aircraftArray[i].setNodes(nodeValues, numNodes);

        }

        // Flight Legs
        int numOfFlightLegs;

        cout << "Input Flight Legs Number : ";
        cin >> numOfFlightLegs;
        cout << endl;
        Leg* legs = new Leg[numOfFlightLegs];
        string toLeg, fromLeg, typeLeg, toPath, fromPath;
        for (int i = 0; i < numOfFlightLegs; i++) {
            cout << "Input Flight Leg [" << i + 1 << "] From : ";
            cin >> fromLeg;
            cout << endl << "Input Flight Leg [" << i + 1 << "] to: ";
            cin >> toLeg;
            cout << endl << "Input Type Of Legs(Mandatory or Optional) [" << i + 1 << "] : ";
            cin >> typeLeg;
            cout << endl;
            legs[i] = Leg(toLeg, fromLeg, typeLeg);
        }

        // Fare Classes
        int numOfFareClasses;
        cout << "Input Fare Class Number : ";
        cin >> numOfFareClasses;
        cout << endl;

        string* NameFareClasses = new string[numOfFareClasses]; //indexed by (h)
        for (int h = 0; h < numOfFareClasses; h++)
        {
            cout << "Input Fare Class [" << h + 1 << "] Name : ";
            cin >> NameFareClasses[h];
            cout << endl;
        }

        // Paths
        int numOfPaths;
        int numOfLegsBelongsToPath;
        cout << "Input All Paths Number : ";
        cin >> numOfPaths;
        cout << endl;
        Path* Paths = new Path[numOfPaths];
        for (int i = 0; i < numOfPaths; i++) {
            cout << "Input Path [" << i + 1 << "] From : ";
            cin >> fromPath;
            cout << endl << "Input Path [" << i + 1 << "] to: ";
            cin >> toPath;
            cout << "Input Num Of Legs Belongs To Path [" << i << "] : ";
            cin >> numOfLegsBelongsToPath;
            cout << endl;
            Paths[i] = Path(toPath, fromPath, numOfLegsBelongsToPath, numOfFareClasses);
            cout << "Legs Belongs To Path [" << i << "] : " << endl;
            Leg* LegsToPath = new Leg[numOfLegsBelongsToPath];
            for (int j = 0; j < numOfLegsBelongsToPath; j++) {
                cout << "Available Legs:";
                displayLegsDetails(legs, numOfFlightLegs);
                char LegsBelongsToPath;
                cout << "Enter (Y) When do add new Legs or (N) When use old Legs: ";
                cin >> LegsBelongsToPath;
                if (toupper(LegsBelongsToPath) == 'Y') {
                    cout << "Input Flight Leg From : ";
                    cin >> fromLeg;
                    cout << endl << "Input Flight Leg to: ";
                    cin >> toLeg;
                    cout << endl << "Input Type Of Legs(Mandatory or Optional) : ";
                    cin >> typeLeg;
                    cout << endl;
                    LegsToPath[j] = Leg(toLeg, fromLeg, typeLeg);
                }
                else {
                    int idleg;
                    cout << "Enter Id Leg to add to this path:";
                    cin >> idleg;
                    cout << endl;
                    LegsToPath[j] = legs[idleg - 1];
                }
            }
            Paths[i].setLegs(LegsToPath,numOfLegsBelongsToPath);
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
                        cout << PathsthatContainLeg[j][k];
                    }
                }
            }
        }
        //Parameters
        //set Mean Demand to FareClass in Path
        for (int i = 0; i < numOfPaths; i++) {
            cout << "Enter Mean demand to Path:" << Paths[i].getFrom() << "," << Paths[i].getTo() << " When Fare Class:" << endl;
            FareClass* newFareClass = new FareClass[numOfFareClasses];
            for (int j = 0; j < numOfFareClasses; j++) {
                int Mph = 0;
                cout << "fare Class:" << NameFareClasses[j] << endl << "Enter Mean demand :";
                cin >> Mph;
                newFareClass[j] = FareClass(NameFareClasses[j], 0, Mph, 0);
            }
            Paths[i].setFareClasses(newFareClass, numOfFareClasses);
        }
        //set Capacity to fareClass in AirCraft
        for (int i = 0; i < numOfAircraft; i++) {
            FareClass* newFareClass = new FareClass[numOfFareClasses];
            cout << "Enter Capacity to AirCraft:" << aircraftArray[i].getType() << " When Fare Class:" << endl;
            for (int j = 0; j < numOfFareClasses; j++) {
                int Capah = 0;
                cout << "fare Class:" << NameFareClasses[j] << endl << "Enter Capacity :";
                cin >> Capah;
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
                cout << "Input cost of assigning fleet type [" << aircraftArray[i].getType() << "] to leg [" << legs[j].getFrom() << "," << legs[j].getTo() << "] : ";
                cin >> Cost;
                newlegs[j].setCost(Cost);
            }
            aircraftArray[i].setLegs(newlegs, numOfFlightLegs);
        }
        //set estimated price for fare class h on path p
        for (int i = 0; i < numOfPaths; i++) {
            FareClass* newFareClass = Paths[i].getFareClasses();
            for (int j = 0; j < Paths[i].getFareClassCount(); j++) {
                int Price = 0;
                cout << "Input estimated price for fare class [" << NameFareClasses[j] << "] on path [" << Paths[i].getFrom() << "," << Paths[i].getTo() << "] : ";
                cin >> Price;
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
        System(PathsthatContainLeg, numPathsthatContainLeg, numOfAircraft, numOfPaths, numOfFlightLegs, numOfFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM, legs, Paths, aircraftArray, NameFareClasses, pathsWithTypeM, pathsWithTypeO, legsOfTypeM, legsOfTypeO);

    }
#pragma endregion
}