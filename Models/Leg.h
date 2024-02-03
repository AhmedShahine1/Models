#pragma once
#include <iostream>
#include "Path.h"
#include "AirCraft.h"

using namespace std;

class Leg
{
public:
	string to;
	string from;
	string type;
	Leg() : to(""), from(""), type(""){}
	Leg(string to,string from,string type)
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
	// Function to display leg details
	void displayLeg(){
		cout << "Leg Details:" << endl;
		cout << "To: " << to << endl;
		cout << "From: " << from << endl;
		cout << "Type: " << type << endl;
	}

	// Function to get the value of 'to'
	string getTo() const {
		return to;
	}

	// Function to get the value of 'from'
	string getFrom() const {
		return from;
	}

	// Function to get the value of 'type'
	string getType() const {
		return type;
	}

};

