/* -----------------------------------------------------------------------------

FILE:           proj3.cpp
DESCRIPTION:    Project 3 for Fall 2019 CS 202-1001 at UNR; Assists users
					in renting a car from one of three different companies
COMPILER:       MinGW C++ compiler
NOTES:          

MODIFICATION HISTORY:
Author				Date			Version
---------------		----------		--------------
Version 1			YYYY-MM-DD		x.x Description
Nick Learner		2019-09-15		1.0	Created program+created RentalCar
Nick Learner		2019-09-17		1.1	Created func prototypes
Nick Learner		2019-09-18		1.2	Wrote rest of program
----------------------------------------------------------------------------- */


#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

// Global constants
const int NUM_OF_AGENCIES = 3;
const int NUM_OF_CARS = 5;
const int MAX_CHAR_ARRAY_SIZE = 256;

// Important function prototypes
size_t myStringLength(const char * str);
int myStringCompare(const char * str1, const char * str2);
void myStringCopy(char * destination, const char * source);
char * myStringCat(char * destination, const char * source);

class RentalCar
{
	private:
		int m_year;
		char m_make[MAX_CHAR_ARRAY_SIZE];
		char m_model[MAX_CHAR_ARRAY_SIZE];
		float m_price;
		bool m_available;
	public:
		void SetYear(int y);
		int GetYear();
		void SetMake(const char* ma);
		char* GetMake();
		void SetModel(const char* mo);
		char* GetModel();
		void SetPrice(float p);
		float GetPrice();
		void SetAvailable(bool a);
		bool GetAvailable();
		void PrintData() const;
		float EstimateCost(int numdays);
		RentalCar();
		RentalCar(int y, char* ma, char* mo,
			float p, bool a);
};

void RentalCar::SetYear(int y)
{
	m_year = y;
}

int RentalCar::GetYear()
{
	return m_year;
}

void RentalCar::SetMake(const char* ma)
{
	myStringCopy(m_make, ma);
}

char* RentalCar::GetMake()
{
	return m_make;
}

void RentalCar::SetModel(const char* mo)
{
	myStringCopy(m_model, mo);
}

char* RentalCar::GetModel()
{
	return m_model;
}

void RentalCar::SetPrice(float p)
{
	m_price = p;
}

float RentalCar::GetPrice()
{
	return m_price;
}

void RentalCar::SetAvailable(bool a)
{
	m_available = a;
}

bool RentalCar::GetAvailable()
{
	return m_available;
}

float RentalCar::EstimateCost(int numdays)
{
	return (m_price * numdays);
}

void RentalCar::PrintData() const
{
	cout << m_year << ' ' << m_make << ' ' << m_model << ' '
			<< fixed << setprecision(2) << m_price
			<< ' ' << boolalpha << m_available << endl;
}

RentalCar::RentalCar()
{
	char* nlptr {0};
	SetYear(-1);
	SetMake(nlptr);
	SetModel(nlptr);
	SetPrice(-1.0);
	SetAvailable(false);
}

RentalCar::RentalCar(int y, char* ma, char* mo,
			float p=-1.0, bool a=false)
{
	SetYear(y);
	SetMake(ma);
	SetModel(mo);
	SetPrice(p);
	SetAvailable(a);
}

struct RentalAgency
{
	char name[MAX_CHAR_ARRAY_SIZE];
	int zipcode[5];
	RentalCar inventory[NUM_OF_CARS];
	RentalAgency();
};

RentalAgency::RentalAgency()
{
	RentalCar DFLT_CAR;
	RentalCar* inventoryptr = inventory;
	char DFLT_NAME = ' ';
	char* nameptr = name;
	int DFLT_ZIP = -1;
	int* zipcodeptr = zipcode;
	
	for (int i = 0; i < MAX_CHAR_ARRAY_SIZE; i++)
	{
		*nameptr = DFLT_NAME;
		nameptr++;
	}
	
	for (int i = 0; i < 5; i++)
	{
		*zipcodeptr = DFLT_NAME;
		zipcodeptr++;
	}
	
	for (int i = 0; i < NUM_OF_CARS; i++)
	{
		*inventoryptr = DFLT_CAR;
		inventoryptr++;
	}
}


// More function prototypes
void UserMenu();
void DisplayUserMenu();
void UserInputFileName(char*, char*, int);
bool OpenInputFile(char*, fstream&);
void FileInput(RentalAgency*, int*, fstream&);
bool OpenOutputFile(char*, fstream&);
void Error();


int main()
{
	//cout << "Nothing to see here..." << endl;
	
	char make[MAX_CHAR_ARRAY_SIZE] = "Ford";
	char model[MAX_CHAR_ARRAY_SIZE] = "Model T";
	
	RentalCar car1(1908, make, model, 260.00, true);
	
	car1.PrintData();
	
	cout << endl;
	
	UserMenu();
	
	return 0;
}


/* -----------------------------------------------------------------------------
FUNCTION:          myStringCopy()
DESCRIPTION:       Copies a char array to a destination
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------- */

void myStringCopy(char * destination, const char * source)
{
	while (*destination != * source)
	{
		*destination = *source;
		destination++;
		source++;
	}
}


/* -----------------------------------------------------------------------------
FUNCTION:          UserMenu()
DESCRIPTION:       Displays a menu the user picks an option from
RETURNS:           void
NOTES:             Based on Nick Learner's UserMenu() function in proj2.cpp
------------------------------------------------------------------------------- */

void UserMenu()
{
	char inputfilename[16] = {' ', ' '};
	char* inputfileptr = inputfilename;
	char outputfilename[16] = {' ', ' '};
	char* outputfileptr = outputfilename;
	char inputfiletype[16] = "Input";
	char* inputfiletypeptr = inputfiletype;
	char outputfiletype[16] = "Output";
	char* outputfiletypeptr = outputfiletype;
	fstream inputfile;
	fstream outputfile;
	
	bool hasfilebeenopened = false;
	bool continuemenu = true;
	int choice;
	
	RentalAgency agencydata[NUM_OF_AGENCIES];
	
	while (continuemenu)
	{
		DisplayUserMenu();
		
		cin >> choice;
		
		switch (choice)
		{
		case 1:
			UserInputFileName(inputfiletypeptr, inputfileptr, 16);
			hasfilebeenopened = OpenInputFile(inputfilename, inputfile);
			if (hasfilebeenopened)
			{
				cout << "File " << inputfilename << " has been opened." << "\n";
				//FileInput(cardata, order, inputfile);
				cout << "Car data has been inputted." << "\n";
				inputfile.close();
				cout << "File " << inputfilename << " has been closed." << "\n\n";
			}
			break;
		case 2:
			/*if (!hasfilebeenopened)
			{
				cout << endl;
				cout << "Data has not yet been inputted. Please choose Option 1).\n\n";
			}else
			{
				cout << "\n\n";
				PrintCarData(cardata, order);
				cout << "\n\n";
			}*/
			break;
		case 3:
			/*UserInputFileName(outputfiletype, outputfilename, 16);
			if (OpenOutputFile(outputfilename, outputfile))
			{
				cout << "File " << outputfilename << " has been opened." << "\n";
				PrintCarDataToFile(cardata, order, outputfile);
				cout << "Data has been entered into the file." << "\n";
				outputfile.close();
				cout << "File " << outputfilename << " has been closed." << "\n\n";
			}*/
			break;
		case 4:
			/*SelectionSort(cardata, order);
			cout << endl;
			cout << "The list has been sorted by ascending price." << "\n\n";*/
			break;
		case 5:
			//FifthMenuOption(cardata, order);
			break;
		case 6:
			cout << endl;
			cout << "Thank you for using Let's Rent a Car, Version 2" << "\n\n";
			continuemenu = false;
			break;
		default:
			cout << endl;
			Error();
			break;
		}
	}
}


/* -----------------------------------------------------------------------------
FUNCTION:          DisplayUserMenu()
DESCRIPTION:       Displays a menu the user picks an option from
RETURNS:           void
NOTES:             Based on Nick Learner's almost identical function in proj2.cpp
------------------------------------------------------------------------------- */

void DisplayUserMenu()
{
	// Menu prompt as per Proj3 instructions
	cout << "Let's Rent a Car, Version 2" << endl;
	cout << "1) Input the name of the rental car and rental agency datafile" << endl;
	cout << "2) Print all rental car and rental agency data to console" << endl;
	cout << "3) Estimate car rental cost" << endl;
	cout << "4) Show most expensive rental car, per day" << endl;
	cout << "5) Print data for available cars" << endl;
	cout << "6) Quit program" << endl;
	
	cout << "Enter your choice [ 1 2 3 4 5 6 ]: ";
}


/* -----------------------------------------------------------------------------
FUNCTION:          UserInputFileName()
DESCRIPTION:       Gets a filename from user
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------*/

void UserInputFileName(char* type, char* filename, int length)
{
	char inputstr[length];
	
	cout << endl;
	cout << type << " filename: ";
	
	cin >> inputstr;
	
	myStringCopy(filename, inputstr);
}


/* -----------------------------------------------------------------------------
FUNCTION:          OpenInputFile()
DESCRIPTION:       Opens a file from which the names are inputted
RETURNS:           bool (true if file is opened, false if file is not found)
NOTES:             
------------------------------------------------------------------------------*/

bool OpenInputFile(char* filename, fstream& inputfile)
{
	inputfile.open(filename, ios::in);
	if (inputfile.fail())
	{
		cout << "File " << filename << " failed to open." << "\n\n";
		return false;
	}
	else
		return true;
}


/* -----------------------------------------------------------------------------
FUNCTION:          Error()
DESCRIPTION:       Gives error message
RETURNS:           void
NOTES:             Used in proj2.cpp of PA2_Learner_Nicholas
------------------------------------------------------------------------------- */

void Error()
{
	cout << "Incorrect input! Please enter a correct input." << "\n\n";
}
