#pragma once
class FareClass{
public:
    // Default constructor
    FareClass() : name(""), Capah(0), Mph(0), Price(0) {}

    // Parameterized constructor
    FareClass(const string & n, int cap, int mph, int Price) : name(n), Capah(cap), Mph(mph), Price(Price) {}

    // Function to display the FareClass information
    void display() const {
        cout << "Name: " << name << "\n";
        cout << "Capah: " << Capah << "\n";
        cout << "Mph: " << Mph << "\n";
        cout << "Price: " << Price << endl;
    }
    void displayDetailToPath() {
        cout << "FareClass Details:" << endl;
        cout << "Name: " << name << endl;
        cout << "Mph: " << Mph << endl;
        cout << "Fph: " << Price << endl;
    }
    void displayDetailToAirCraft() {
        cout << "FareClass Details:" << endl;
        cout << "Name: " << name << endl;
        cout << "Capah: " << Capah << endl;
    }

    string getName() const {
        return name;
    }

    int getCapah() const {
        return Capah;
    }

    int getMph() const {
        return Mph;
    }

    int getPrice() const {
        return Price;
    }
    void setName(const string& n) {
        name = n;
    }

    void setCapah(int cap) {
        Capah = cap;
    }

    void setMph(int mph) {
        Mph = mph;
    }

    void setPrice(int price) {
        Price = price;
    }
private:
    string name;
    int Capah;
    int Mph;
    int Price;
};