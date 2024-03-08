#pragma once
#include <iostream>
#include "AirCraft.h"
#include "Leg.h"
#include "FareClass.h"

using namespace std;
class Path {
public:
	// Default constructor
	Path() : Legs(nullptr), to(""), from(""), legCount(0), fareClasses(nullptr), fareClassCount(0) {}
	Path(string to, string from, int legCount, int fareClassCount)
	{
		this->legCount = legCount;
		this->to = to;
		this->from = from;
		this->fareClassCount = fareClassCount;
		fareClasses = new FareClass[fareClassCount];
		Legs = new Leg[legCount];
	}

	// Destructor to free allocated memory
	~Path() {
		delete[] Legs;
		delete[] fareClasses;
	}

	// Setter functions
	void setTo(const string& t) {
		to = t;
	}

	void setFrom(const string& f) {
		from = f;
	}

	void setLegs(const Leg* legsArray, int count) {
		// Assume ownership of the array
		Legs = new Leg[count];
		for (int i = 0; i < count; ++i) {
			Legs[i] = legsArray[i];
		}
		legCount = count;
	}

	void setFareClasses(const FareClass* fareArray, int count) {
		// Assume ownership of the array
		fareClasses = new FareClass[count];
		for (int i = 0; i < count; ++i) {
			fareClasses[i] = fareArray[i];
		}
		fareClassCount = count;
	}

	// Getter functions
	string getTo() const {
		return to;
	}

	string getFrom() const {
		return from;
	}

	const Leg* getLegs() const {
		return Legs;
	}

	int getLegCount() const {
		return legCount;
	}

	 FareClass* getFareClasses() {
		return fareClasses;
	}

	int getFareClassCount() const {
		return fareClassCount;
	}

	// Function to checK if the path has at least one leg of type "m"
	bool hasLegOfTypeM() const {
		for (int i = 0; i < legCount; ++i) {
			if (Legs[i].getType() == "Mandatory") {
				return true;
			}
		}
		return false;
	}
	// Function to checK if the path has at least one leg of type "O"
	bool hasLegOfTypeO() const {
		for (int i = 0; i < legCount; ++i) {
			if (Legs[i].getType() == "Optional") {
				return true;
			}
		}
		return false;
	}

	// Function to checK if a specific to and from pair exists in the legs of the path
	bool hasLeg(string checKTo, string checKFrom) {
		for (int i = 0; i < legCount; i++) {
			if (Legs[i].hasToFrom(checKTo, checKFrom)) {
				return true;  // Found a matching leg
			}
		}
		return false;  // No matching leg found
	}

	int getPriceFromFareClass(string fareClassName) {
		for (int i = 0; i < fareClassCount; i++) {
			if (fareClasses[i].getName() == fareClassName)
				return fareClasses[i].getPrice();
		}
	}

	void display() {
		cout << "Leg Details:\n";
		cout << "To: " << to << "\n";
		cout << "From: " << from << "\n";
		cout << "Number of Leg: " << legCount << "\n";
		cout << "Number of Fare Class: " << fareClassCount << "\n";
		for (int i = 0; i < legCount; i++)
		{
			Legs[i].display();
		}
		for (int i = 0; i < fareClassCount; i++)
		{
			fareClasses[i].display();
		}
	}
private:
	Leg* Legs;
	string to;
	string from;
	int legCount;
	FareClass* fareClasses;
	int fareClassCount;
};
