#pragma once
#include <iostream>
#include "Path.h"
#include "Leg.h"

using namespace std;

class AirCraft
{
public:
	string Type;
	int NumberAirCraft;
    int numNodes;
	string* nodes;
    // Default ructor
    AirCraft() : Type(""), NumberAirCraft(0), nodes(nullptr){}

    // ructor
    AirCraft(string type, int numberAirCraft, int numNodes)
        : Type(type), NumberAirCraft(numberAirCraft),numNodes(numNodes) {
        // Allocate memory for the nodes array
        nodes = new string[numNodes];
    }

    // Destructor
    ~AirCraft() {
        // Deallocate memory for the nodes array
        delete[] nodes;
    }

    // Function to set values for nodes
    void setNodesValues(string* nodeValues, int numNodes) {
        for (int i = 0; i < numNodes; ++i) {
            nodes[i] = nodeValues[i];
        }
    }

    // Display information about the aircraft
    void displayInfo() {
        cout << "Aircraft Type: " << Type << endl;
        cout << "Number of Aircraft: " << NumberAirCraft << endl;
        cout << "Airports (nodes): ";
        for (int i = 0; i < numNodes; ++i) {
            cout << nodes[i] << ", ";
        }
        cout << endl;
    }

    // Function to get the value of Type
    string getType() {
        return Type;
    }

};

