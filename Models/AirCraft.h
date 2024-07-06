#pragma once
#include <iostream>
#include "Path.h"
#include "Leg.h"
#include "FareClass.h"

using namespace std;
class AirCraft {
public:
    // Default constructor
    AirCraft() : Type(""), NumberAirCraft(0), numNodes(0), nodes(nullptr),
        fareClasses(nullptr), fareClassCount(0), legs(nullptr), numofLegs(0) {}

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
    // Setter functions
    void setType(const string& type) {
        Type = type;
    }

    void setNumberAirCraft(int number) {
        NumberAirCraft = number;
    }

    void setNumNodes(int num) {
        numNodes = num;
    }

    void setNodes(const string* n, int count) {
        // Assume ownership of the array
        nodes = new string[count];
        for (int i = 0; i < count; ++i) {
            nodes[i] = n[i];
        }
        numNodes = count;
    }

    void setFareClasses(const FareClass* fareArray, int count) {
        // Assume ownership of the array
        fareClasses = new FareClass[count];
        for (int i = 0; i < count; ++i) {
            fareClasses[i] = fareArray[i];
        }
        fareClassCount = count;
    }

    void setLegs(const Leg* legsArray, int count) {
        // Assume ownership of the array
        legs = new Leg[count];
        for (int i = 0; i < count; ++i) {
            legs[i] = legsArray[i];
        }
        numofLegs = count;
    }

    // Getter functions
    string getType() const {
        return Type;
    }

    int getNumberAirCraft() const {
        return NumberAirCraft;
    }

    int getNumNodes() const {
        return numNodes;
    }

    const string* getNodes() const {
        return nodes;
    }

    const FareClass* getFareClasses() const {
        return fareClasses;
    }

    int getFareClassCount() const {
        return fareClassCount;
    }

    const Leg* getLegs() const {
        return legs;
    }

    int getNumofLegs() const {
        return numofLegs;
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
        cout << endl;
        cout << "Legs:" << endl;
        for (int i = 0; i < numofLegs; i++) {
            legs[i].display();
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

    int getCostFromLeg(string FromLeg, string ToLeg) {
        for (int i = 0; i < numofLegs; i++) {
            if (FromLeg == legs[i].getFrom() && ToLeg == legs[i].getTo()) {
                return legs[i].getCost();
            }
        }
    }
private:
    string Type;
    int NumberAirCraft;
    int numNodes;
    string* nodes;
    FareClass* fareClasses;
    int fareClassCount;
    Leg* legs;
    int numofLegs;
};
