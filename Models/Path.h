#pragma once
#include <iostream>
#include "AirCraft.h"
#include "Leg.h"
#include "FareClass.h"

using namespace std;

class Path
{
public:
	Leg* Legs;
	string to;
	string from;
	int legCount;
	FareClass* fareClasses;
	int fareClassCount;
	Path() : Legs(nullptr), to(""), from(""), fareClasses(nullptr){}

	Path(string to, string from, int legCount, int fareClassCount)
	{
		this->legCount = legCount;
		this->to = to;
		this->from = from;
		this->fareClassCount = fareClassCount;
		fareClasses = new FareClass[fareClassCount];
		Legs = new Leg[legCount];
	}

	~Path() {
		delete[] Legs;
	}

	// Function to add a leg to the path
	void addLeg( Leg* newLeg) {
		for (int i = 0; i < legCount; ++i) {
			Legs[i] = newLeg[i];
		}
	}

	// Function to add a Fare Class to the path
	void addFareClass(FareClass* newFareClass) {
		for (int i = 0; i < fareClassCount; ++i) {
			fareClasses[i] = newFareClass[i];
		}
	}

	// Function to display path details
	void displayPath(){
		cout << "Path Details:" << endl;
		cout << "To: " << to << endl;
		cout << "From: " << from << endl;
		cout << "Leg Count: " << legCount << endl;

		// Display details of each leg in the path
		for (int i = 0; i < legCount; ++i) {
			Legs[i].displayLeg();
		}

		cout << "Fare Classes:" << endl;
		for (int i = 0; i < fareClassCount; ++i) {
			fareClasses[i].displayDetailToPath();
		}
	}

	void DisplayPathToConstraints(int i) {
		cout << "Path Id:" << i+1 << endl;
		cout << "To: " << to << endl;
		cout << "From: " << from << endl;
	}
	// Function to get the value of 'to'
	string getTo() const {
		return to;
	}

	// Function to get the value of 'from'
	string getFrom() const {
		return from;
	}

	// Function to checK if the path has at least one leg of type "m"
	bool hasLegOfTypeM() const {
		for (int i = 0; i < legCount; ++i) {
			if (Legs[i].type == "Mandatory") {
				return true;
			}
		}
		return false;
	}
	// Function to checK if the path has at least one leg of type "O"
	bool hasLegOfTypeO() const {
		for (int i = 0; i < legCount; ++i) {
			if (Legs[i].type == "Optional") {
				return true;
			}
		}
		return false;
	}

	// Function to checK if a specific to and from pair exists in the legs of the path
	bool hasLeg(string checKTo, string checKFrom) {
		for (int i = 0; i < legCount; ++i) {
			if (Legs[i].hasToFrom(checKTo, checKFrom)) {
				return true;  // Found a matching leg
			}
		}
		return false;  // No matching leg found
	}
};

