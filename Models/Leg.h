#pragma once
#include <iostream>
#include "Path.h"
#include "AirCraft.h"

using namespace std;
class Leg {
public:
	// Default constructor
	Leg() : to(""), from(""), type(""), cost(0) {}

	Leg(string to, string from, string type)
	{
		this->to = to;
		this->from = from;
		if (type == "Mandatory" || type == "M" || type == "m" || type == "mandatory") {
			this->type = "Mandatory";
		}
		else {
			this->type = "Optional";
		}
	}

	// Getter functions
	string getTo() const {
		return to;
	}

	string getFrom() const {
		return from;
	}

	string getType() const {
		return type;
	}

	int getCost() const {
		return cost;
	}

	// Setter functions (encapsulation)
	void setTo(const string& t) {
		to = t;
	}

	void setFrom(const string& f) {
		from = f;
	}

	void setType(const string& ty) {
		type = ty;
	}

	void setCost(int c) {
		cost = c;
	}

	// Display function
	void display() const {
		cout << "Leg Details:\n";
		cout << "To: " << to << "\n";
		cout << "From: " << from << "\n";
		cout << "Type: " << type << "\n";
		cout << "Cost: " << cost << "\n";
	}
	// Function to checK if the given to and from values match the leg
	bool hasToFrom(string checKTo, string checKFrom) {
		return (to == checKTo && from == checKFrom);
	}

	// Function to check if the leg has a specific type
	bool hasType(string checkType) const {
		return (type == checkType);
	}

private:
	string to;
	string from;
	string type;
	int cost;
};