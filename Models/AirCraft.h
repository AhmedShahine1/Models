#pragma once
class AirCraft
{
public:
	string Type;
	int NumberAirCraft;
    int numNodes;
	string* nodes;

    // Default constructor
    AirCraft() : Type(""), NumberAirCraft(0), nodes(nullptr) {}

    // Constructor
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
    void setNodesValues(const string* nodeValues, int numNodes) {
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
};

