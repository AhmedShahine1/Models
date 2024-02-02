#pragma once
#include <string>
using namespace std;
class Leg
{
public:
	string to;
	string from;
	string type;
	Leg() : to(""), from(""), type("") {}
	Leg(string to,string from,string type)
	{
		this->to = to;
		this->from = from;
		this->type = type;
	}
	// Function to display leg details
	void displayLeg() const {
		cout << "Leg Details:" << endl;
		cout << "To: " << to << endl;
		cout << "From: " << from << endl;
		cout << "Type: " << type << endl;
	}
};

