#pragma once
#include <iostream>
#include "AirCraft.h"
#include "Leg.h"

using namespace std;

class Path
{
public:
	Leg* Legs;
	string to;
	string from;
	//FareClass* fareClasses;
	int legCount;
	Path() : Legs(nullptr), to(""), from(""){}
	Path(string to, string from, int legCount)
	{
		this->legCount = legCount;
		this->to = to;
		this->from = from;
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

	// Function to display path details
	void displayPath()  {
		cout << "To: " << to << endl;
		cout << "From: " << from << endl;

		// Display details of each leg in the path
		for (int i = 0; i < legCount; ++i) {
			Legs[i].displayLeg();
		}
	}

	// Function to get the value of 'to'
	string getTo() const {
		return to;
	}

	// Function to get the value of 'from'
	string getFrom() const {
		return from;
	}

	// Function to check if the path has at least one leg of type "m"
	bool hasLegOfTypeM() const {
		for (int i = 0; i < legCount; ++i) {
			if (Legs[i].type == "Mandatory") {
				return true;
			}
		}
		return false;
	}
	// Function to check if the path has at least one leg of type "O"
	bool hasLegOfTypeO() const {
		for (int i = 0; i < legCount; ++i) {
			if (Legs[i].type == "Optional") {
				return true;
			}
		}
		return false;
	}
};

