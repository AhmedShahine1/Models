#include <iostream>
#include "Path.h"
#include "AirCraft.h"
using namespace std;
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

    // Display information about each aircraft in the array
    for (int i = 0; i < numOfAircraft; ++i) {
        cout << "\nAircraft " << i + 1 << " Details:" << endl;
        aircraftArray[i].displayInfo();
    }


	// Flight Legs
	int numOfFlightLegs;

	cout << "Input Flight Legs Number : ";
	cin >> numOfFlightLegs;
	cout << endl;
	Leg* legs = new Leg[numOfFlightLegs];
	string to, from, type;
	for (int i = 0; i < numOfFlightLegs; i++) {
		cout << "Input Flight Leg [" << i + 1 << "] to: ";
		cin >> to;
		cout << endl<<"Input Flight Leg [" << i + 1 << "] From : ";
		cin >> from;
		cout << endl << "Input Type Of Legs(Mandatory or Optional) [" << i + 1 << "] : ";
		cin >> type;
		cout << endl;
		legs[i]=Leg(to,from,type);
	}

    // Display information about each aircraft in the array
    for (int i = 0; i < numOfFlightLegs; ++i) {
        cout << "\nFlight Legs " << i + 1 << " Details:" << endl;
        legs[i].displayLeg();
    }

    // Paths
    int numOfPaths;
    int numOfLegsBelongsToPath;
    int numOfPathHaveOptionalLegs;
    
    cout << "Input All Paths Number : ";
    cin >> numOfPaths;
    cout << endl;


    #pragma endregion
	return 0;
}