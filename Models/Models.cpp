//#include <iostream> //cout , cin 
//using namespace std;
//
//int main()
//{
//#pragma region Problem Data
//
//	// Aircraft Types
//	int numOfAircraft;
//
//	cout << "Input Aircrafts Number : ";
//	cin >> numOfAircraft;
//	cout << endl;
//
//	string* AircraftsType = new string[numOfAircraft]; //indexed by (a)
//
//	for (int a = 0; a < numOfAircraft; a++)
//	{
//		cout << "Input Aircrafts Type [" << a + 1 << "] : ";
//		cin >> AircraftsType[a];
//		cout << endl;
//	}
//
//
//	// Flight Legs
//	int numOfFlightLegs;
//	int numMandatoryLegs = 0;
//	int numOptionalLegs = 0;
//
//	cout << "Input Flight Legs Number : ";
//	cin >> numOfFlightLegs;
//	cout << endl;
//
//	string* FlightLegs = new string[numOfFlightLegs]; //indexed by (j)
//	string* MandatoryOrOptional = new string[numOfFlightLegs];
//
//	for (int j = 0; j < numOfFlightLegs; j++)
//	{
//		cout << "Input Flight Leg [" << j + 1 << "] : ";
//		cin >> FlightLegs[j];
//		cout << endl;
//
//		cout << "Input Type Of Legs(Mandatory or Optional) [" << j + 1 << "] : ";
//		cin >> MandatoryOrOptional[j];
//		cout << endl;
//
//		if (MandatoryOrOptional[j] == "Mandatory") {
//			numMandatoryLegs++;
//		}
//		else {
//			numOptionalLegs++;
//		}
//	}
//
//	string* MandatoryLegs = new string[numMandatoryLegs];
//	string* OptionalLegs = new string[numOptionalLegs];
//
//	for (int j = 0; j < numOfFlightLegs; j++) {
//		if (MandatoryOrOptional[j] == "M") {
//			MandatoryLegs[j] = FlightLegs[j];
//		}
//		else if (MandatoryOrOptional[j] == "O") {
//			OptionalLegs[j] = FlightLegs[j];
//		}
//	}
//
//
//
//	//Nodes : city in networK of Aircraft type a 
//	int numOfNodes;
//
//	string** NodesOfAircraft = new string * [numOfAircraft]; //indexed by (n) and a ==> AT
//
//	for (int a = 0; a < numOfAircraft; a++) {
//		cout << "Input Nodes in aircraft type [" << AircraftsType[a] << "] networK Number : ";
//		cin >> numOfNodes;
//		cout << endl;
//		NodesOfAircraft[a] = new string[numOfNodes];
//	}
//
//
//
//
//	for (int a = 0; a < numOfAircraft; a++) {
//		for (int n = 0; n < numOfNodes; n++)
//		{
//			cout << "Input nodes in netwoK of type [" << AircraftsType[a] << "][" << n + 1 << "] : ";
//			cin >> NodesOfAircraft[a][n];
//			cout << endl;
//		}
//	}
//
//
//
//	// Paths
//	int numOfPaths;
//	int numOfLegsBelongsToPath;
//	int numOfPathHaveOptionalLegs;
//
//	cout << "Input All Paths Number : ";
//	cin >> numOfPaths;
//	cout << endl;
//
//	string* AllPaths = new string[numOfPaths]; //indexed by (p)
//
//	for (int p = 0; p < numOfPaths; p++)
//	{
//		cout << "Input Path [" << p + 1 << "] : ";
//		cin >> AllPaths[p];
//		cout << endl;
//	}
//
//
//	string** LegsBelongsToPath = new string * [numOfPaths];
//
//	for (int p = 0; p < numOfPaths; p++)
//	{
//		cout << "Input Num Of Legs Belongs To Path [" << AllPaths[p] << "] : ";
//		cin >> numOfLegsBelongsToPath;
//		cout << endl;
//		LegsBelongsToPath[p] = new string[numOfLegsBelongsToPath];
//
//		for (int n = 0; n < numOfLegsBelongsToPath; n++)
//		{
//			cout << "Input Leg [" << n + 1 << "] Belong To Path [" << AllPaths[p] << "] : ";
//			cin >> LegsBelongsToPath[p][n];
//			cout << endl;
//		}
//	}
//
//	cout << "Input number of path have optional legs : ";
//	cin >> numOfPathHaveOptionalLegs;
//	cout << endl;
//
//	/*
//	string* PathContainigOptional = new string[numOfPaths]; //indexed by (p)
//
//	string** OptionalLegsBelongsToPath = new string * [numOfPaths];
//	*/
//
//
//	// Fare Classes
//	int numOfFareClasses;
//	int numOfFareClassesInPath;
//
//	cout << "Input Fare Class Number : ";
//	cin >> numOfFareClasses;
//	cout << endl;
//
//	string* FareClasses = new string[numOfPaths]; //indexed by (h)
//
//	for (int h = 0; h < numOfFareClasses; h++)
//	{
//		cout << "Input Fare Class [" << h + 1 << "] Name : ";
//		cin >> FareClasses[h];
//		cout << endl;
//	}
//
//	string** ClassesBelongsToPath = new string * [numOfPaths];
//
//	for (int p = 0; p < numOfPaths; p++)
//	{
//		cout << "Input Num Of Fare Classes Belongs To Path [" << AllPaths[p] << "] : ";
//		cin >> numOfFareClassesInPath;
//		cout << endl;
//		ClassesBelongsToPath[p] = new string[numOfFareClassesInPath];
//
//		for (int n = 0; n < numOfLegsBelongsToPath; n++)
//		{
//			cout << "Input Class [" << n + 1 << "] Belong To Path [" << AllPaths[p] << "] : ";
//			cin >> ClassesBelongsToPath[p][n];
//			cout << endl;
//		}
//	}
//
//	//Parameters
//	int** Caj = new int* [numOfAircraft]; //cost of assigning fleet type a to leg j
//
//	for (int a = 0; a < numOfAircraft; a++) {
//		Caj[a] = new int[numOfFlightLegs];
//		for (int j = 0; j < numOfFlightLegs; j++) {
//			cout << "Input cost of assigning fleet type [" << AircraftsType[a] << "] to leg [" << FlightLegs[j] << "] : ";
//			cin >> Caj[a][j];
//			cout << endl;
//		}
//	}
//
//	int* NAa = new int[numOfAircraft]; //number of available aircraft for fleet type a
//
//	for (int a = 0; a < numOfAircraft; a++) {
//		cout << "Input number of available aircraft for fleet type [" << AircraftsType[a] << "] : ";
//		cin >> NAa[a];
//		cout << endl;
//	}
//
//	int** Capah = new int* [numOfAircraft]; //capacity of aircraft type a to accommodate passengers for fare class h
//
//	for (int a = 0; a < numOfAircraft; a++) {
//		Capah[a] = new int[numOfFareClasses];
//		for (int h = 0; h < numOfFareClasses; h++) {
//			cout << "Input capacity of aircraft type [" << AircraftsType[a] << "] to accommodate passengers for fare class [" << FareClasses[h] << "] : ";
//			cin >> Capah[a][h];
//			cout << endl;
//		}
//	}
//
//	int** Mph = new int* [numOfPaths]; //mean demand for fare class h onpath (or itinerary) p
//
//	for (int p = 0; p < numOfPaths; p++) {
//		Mph[p] = new int[numOfFareClasses];
//		for (int h = 0; h < numOfFareClasses; h++) {
//			cout << "Input mean demand for fare class [" << FareClasses[h] << "] on path (or itinerary) [" << AllPaths[p] << "] : ";
//			cin >> Mph[p][h];
//			cout << endl;
//		}
//	}
//
//	int** Fph = new int* [numOfPaths]; //estimated price for fare class h on path p
//
//	for (int p = 0; p < numOfPaths; p++) {
//		Fph[p] = new int[numOfFareClasses];
//		for (int h = 0; h < numOfFareClasses; h++) {
//			cout << "Input estimated price for fare class [" << FareClasses[h] << "] on path [" << AllPaths[p] << "] : ";
//			cin >> Fph[p][h];
//			cout << endl;
//		}
//	}
//
//#pragma endregion
//
//	return 0;
//}

