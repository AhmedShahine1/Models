#pragma once
#include "Leg.h"
class Path
{
public:
	Leg* Legs;
	string to;
	string from;
	string type;
	Path(string toPath, string fromPath, string typePath, int legCount)
	{
		this->to = toPath;
		this->from = fromPath;
		this->type = typePath;
		Legs = new Leg[legCount];
	}
	~Path() {
		delete[] Legs;
	}
};

