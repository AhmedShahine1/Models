#pragma once
class FareClass
{
private:
	string name;
	int Capah;
	int Mph;
public:
	// Constructor with default parameter values
	FareClass(string name = "", int Capah = 0, int Mph = 0)
		: name(name), Capah(Capah), Mph(Mph) {}

	void displayDetailToPath(){
		cout << "FareClass Details:" << endl;
		cout << "Name: " << name << endl;
		cout << "Mph: " << Mph << endl;
	}
	void displayDetailToAirCraft() {
		cout << "FareClass Details:" << endl;
		cout << "Name: " << name << endl;
		cout << "Capah: " << Capah << endl;
	}

	// Getter function for Capah
	int getCapah() {
		return Capah;
	}

	// Getter function for Mph
	int getMph() {
		return Mph;
	}
};

