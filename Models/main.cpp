#pragma once
#include <iostream>
#include "Path.h"
#include "AirCraft.h"
#include "Leg.h"
#include "Constraints.h"
#include "FareClass.h"

using namespace std;
void displayAircraftDetails(AirCraft* aircraftArray, int numOfAircraft) {
    for (int i = 0; i < numOfAircraft; ++i) {
        cout << "\nAircraft " << i + 1 << " Details:" << endl;
        aircraftArray[i].displayDetails();
    }
}

// Function to display information about each flight leg in the array
void displayLegsDetails(Leg* legs, int numOfFlightLegs) {
    for (int k = 0; k < numOfFlightLegs; ++k) {
        cout << "\nFlight Legs Id: " << k + 1 << ", Details:" << endl;
        legs[k].displayLeg();
    }
}

// Function to display information about each path in the array
void displayPathsDetails(Path* paths, int numOfPaths) {
    for (int i = 0; i < numOfPaths; i++) {
        cout << "\nPath " << i + 1 << " Details:" << endl;
        paths[i].displayPath();
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

// Function to get all legs with type "m" from an array of paths
Leg* getAllLegsOfTypeM(const Path* paths, int numOfPaths, int& numOfLegsOfTypeM) {
    // Count the number of legs with type "m"
    for (int i = 0; i < numOfPaths; ++i) {
        numOfLegsOfTypeM += paths[i].hasLegOfTypeM() ? paths[i].legCount : 0;
    }

    // Create a dynamic array to store legs with type "m"
    Leg* legsOfTypeM = new Leg[numOfLegsOfTypeM];
    int currentIndex = 0;

    // Populate the dynamic array with legs of type "m"
    for (int i = 0; i < numOfPaths; ++i) {
        for (int j = 0; j < paths[i].legCount; ++j) {
            if (paths[i].Legs[j].type == "Mandatory") {
                legsOfTypeM[currentIndex++] = paths[i].Legs[j];
            }
        }
    }

    return legsOfTypeM;
}

// Function to get all legs with type "o" from an array of paths
Leg* getAllLegsOfTypeO(const Path* paths, int numOfPaths, int& numOfLegsOfTypeO) {
    // Count the number of legs with type "o"
    for (int i = 0; i < numOfPaths; ++i) {
        numOfLegsOfTypeO += paths[i].hasLegOfTypeO() ? paths[i].legCount : 0;
    }

    // Create a dynamic array to store legs with type "o"
    Leg* legsOfTypeO = new Leg[numOfLegsOfTypeO];
    int currentIndex = 0;

    // Populate the dynamic array with legs of type "o"
    for (int i = 0; i < numOfPaths; ++i) {
        for (int j = 0; j < paths[i].legCount; ++j) {
            if (paths[i].Legs[j].type == "Optional") {
                legsOfTypeO[currentIndex++] = paths[i].Legs[j];
            }
        }
    }

    return legsOfTypeO;
}

int main() {
    #pragma region Problem Data

	// Aircraft Types
    int numOfAircraft;

    // Take input for the number of aircraft
    cout << "Enter the number of Aircraft Type: ";
    cin >> numOfAircraft;

    // Create a dynamic array of AirCraft objects
    AirCraft* aircraftArray = new AirCraft[numOfAircraft];

    // Initialize each AirCraft object in the array
    for (int i = 0; i < numOfAircraft; ++i) {
        string type;
        int numberAirCraft;
        int numNodes;

        // Take input for each aircraft
        cout << "Aircraft ["<<i+1<<"]\nEnter Aircraft Type : ";
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
        aircraftArray[i].setNodesValues(nodeValues, numNodes);

    }

	// Flight Legs
	int numOfFlightLegs;

	cout << "Input Flight Legs Number : ";
	cin >> numOfFlightLegs;
	cout << endl;
	Leg* legs = new Leg[numOfFlightLegs];
	string toLeg, fromLeg, typeLeg, toPath,fromPath;
	for (int i = 0; i < numOfFlightLegs; i++) {
        cout << "Input Flight Leg [" << i + 1 << "] From : ";
        cin >> fromLeg;
		cout << endl << "Input Flight Leg [" << i + 1 << "] to: ";
		cin >> toLeg;
		cout << endl << "Input Type Of Legs(Mandatory or Optional) [" << i + 1 << "] : ";
		cin >> typeLeg;
		cout << endl;
		legs[i]=Leg(toLeg,fromLeg,typeLeg);
	}

    // Fare Classes
    int numOfFareClasses;
    int numOfFareClassesInPath;
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
                LegsToPath[j] = legs[idleg-1];
            }
        }
        Paths[i].addLeg(LegsToPath);
    }

    for (int i = 0; i < numOfPaths; i++) {
        cout << "Enter Mean demand to Path:" << Paths[i].getFrom() << "," << Paths[i].getTo() << " When Fare Class:"<<endl;
        FareClass* newFareClass = new FareClass[numOfFareClasses];
        for (int j = 0; j < numOfFareClasses; j++) {
            cout << "fare Class:" << NameFareClasses[j]<<endl<<"Enter Mean demand :";
            int Mph = 0;
            cin >> Mph;
            newFareClass[j] = FareClass(NameFareClasses[j],0,Mph);
        }
        Paths[i].addFareClass(newFareClass);
    }

    for (int i = 0; i < numOfAircraft; i++) {
        aircraftArray[i] = AirCraft(aircraftArray[i].getType(), aircraftArray[i].getNumberAirCraft(), aircraftArray[i].getNumNodes(), numOfFareClasses, aircraftArray[i].getNodes());
        cout << "Enter Capacity to AirCraft:" << aircraftArray[i].getType() << " When Fare Class:" << endl;
        FareClass* newFareClass = new FareClass[numOfFareClasses];
        for (int j = 0; j < numOfFareClasses; j++) {
            cout << "fare Class:" << NameFareClasses[j] << endl << "Enter Capacity :";
            int Capah = 0;
            cin >> Capah;
            Capah = Capah * aircraftArray[i].getNumberAirCraft();
            newFareClass[j] = FareClass(NameFareClasses[j], Capah, 0);
        }
    }

    //Parameters
    int** Caj = new int* [numOfAircraft]; //cost of assigning fleet type a to leg j

    for (int a = 0; a < numOfAircraft; a++) {
        Caj[a] = new int[numOfFlightLegs];
        for (int j = 0; j < numOfFlightLegs; j++) {
            cout << "Input cost of assigning fleet type [" << aircraftArray[a].getType() << "] to leg [" << legs[j].getFrom()<<","<<legs[j].getTo() << "] : ";
            cin >> Caj[a][j];
            cout << endl;
        }
    }

    int** Fph = new int* [numOfPaths]; //estimated price for fare class h on path p

    for (int p = 0; p < numOfPaths; p++) {
        Fph[p] = new int[numOfFareClasses];
        for (int h = 0; h < numOfFareClasses; h++) {
            cout << "Input estimated price for fare class [" << NameFareClasses[h] << "] on path [" << Paths[p].getFrom() << "," << Paths[p].getTo() << "] : ";
            cin >> Fph[p][h];
            cout << endl;
        }
    }

    int numOfPathsWithLegM = 0;
    // Get all paths with at least one leg of type "m"
    Path* pathsWithTypeM = getAllPathsWithLegOfTypeM(Paths, numOfPaths, numOfPathsWithLegM);

    int numOfPathsWithLegO = 0;
    // Get all paths with at least one leg of type "o"
    Path* pathsWithTypeO = getAllPathsWithLegOfTypeM(Paths, numOfPaths, numOfPathsWithLegO);

    int numOfLegsOfTypeM = 0;
    // Get all legs with type "m"
    Leg* legsOfTypeM = getAllLegsOfTypeM(Paths, numOfPaths, numOfLegsOfTypeM);

    int numOfLegsOfTypeO = 0;
    // Get all legs with type "O"
    Leg* legsOfTypeO = getAllLegsOfTypeO(Paths, numOfPaths, numOfLegsOfTypeO);

    Constraints constrain = Constraints(numOfAircraft, numOfPaths, numOfFlightLegs, numOfFareClasses, numOfLegsOfTypeO, numOfLegsOfTypeM, numOfPathsWithLegO, numOfPathsWithLegM, legs, Paths, aircraftArray, NameFareClasses, Caj, Fph, pathsWithTypeM, pathsWithTypeO, legsOfTypeM, legsOfTypeO);
    #pragma endregion
	return 0;
}


