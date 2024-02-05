#pragma once
#include <iostream>
#include "Path.h"
#include "Leg.h"
#include "FareClass.h"

using namespace std;

class AirCraft
{
public:
	string Type;
	int NumberAirCraft;
    int numNodes;
	string* nodes;
    FareClass* fareClasses;
    int fareClassCount;
    // Default ructor
    AirCraft() : Type(""), NumberAirCraft(0), nodes(nullptr), fareClasses(nullptr) {}

    // ructor
    AirCraft(string type, int numberAirCraft, int numNodes)
        : Type(type), NumberAirCraft(numberAirCraft),numNodes(numNodes) {
        // Allocate memory for the nodes array
        nodes = new string[numNodes];
    }
    AirCraft(string type, int numberAirCraft, int numNodes, int fareClassCount, string* nodes)
        : Type(type), NumberAirCraft(numberAirCraft), numNodes(numNodes), fareClassCount(fareClassCount), nodes(nodes) {
        // Allocate memory for the nodes array
        fareClasses = new FareClass[fareClassCount];
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

    // Function to set values for Fare Class
    void setFareClassValues(FareClass* newFareClass) {
        for (int i = 0; i < fareClassCount; ++i) {
            fareClasses[i] = newFareClass[i];
        }
    }
    void displayDetails(){
        cout << "AirCraft Details:" << endl;
        cout << "Type: " << Type << endl;
        cout << "NumberAirCraft: " << NumberAirCraft << endl;
        cout << "NumNodes: " << numNodes << endl;

        cout << "Nodes: ";
        for (int i = 0; i < numNodes; ++i) {
            cout << nodes[i] << " ";
        }
        cout << endl;

        cout << "Fare Classes:" << endl;
        for (int i = 0; i < fareClassCount; ++i) {
            fareClasses[i].displayDetailToAirCraft();
        }
    }


    void DisplayAirCraftToConstraints(int i) {
        cout << "AirCraft Id:" << i + 1 << endl;
        cout << "Type: " << Type << endl;
        cout << "NumberAirCraft: " << NumberAirCraft << endl;
        cout << "NumNodes: " << numNodes << endl;

        cout << "Nodes: ";
        for (int i = 0; i < numNodes; ++i) {
            cout << nodes[i] << " ";
        }
        cout << endl;
    }
    // Getter function for Type
    string getType() const {
        return Type;
    }

    // Getter function for NumberAirCraft
    int getNumberAirCraft() const {
        return NumberAirCraft;
    }

    // Getter function for numNodes
    int getNumNodes() const {
        return numNodes;
    }

    // Getter function for nodes
    string* getNodes() const {
        return nodes;
    }

};

