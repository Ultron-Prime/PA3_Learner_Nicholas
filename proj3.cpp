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
Nick Learner		2019-09-18		1.2	Got stuck on inputting agency data from file
Nick Learner		2019-09-19		1.3	Finished program
----------------------------------------------------------------------------- */


#include <iostream>
#include <fstream>
#include <iomanip>// Used for setprecision()

#include "proj3.h"

using namespace std;

// Global constants
const int NUM_OF_AGENCIES = 3;

// Important function prototypes
size_t myStringLength(const char * str);
int myStringCompare(const char * str1, const char * str2);
char * myStringCopy(char * destination, const char * source);
char * myStringCat(char * destination, const char * source);

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
	cout << m_year << ' ' << m_make << ' ' << m_model
			<< ", $" << fixed << setprecision(2) << m_price
			<< " per day, Available: " << boolalpha
			<< m_available << endl;
}

RentalCar::RentalCar()
{
	m_year = -1;
	myStringCopy(m_make, "MAKE");
	myStringCopy(m_model, "MODEL");
	m_price = 0.0;
	m_available = false;
}

RentalCar::RentalCar(int y, char* ma, char* mo,
			float p, bool a)
{
	m_year = y;
	myStringCopy(m_make, ma);
	myStringCopy(m_model, mo);
	m_price = p;
	m_available = a;
}

// More function prototypes
void UserMenu();
void DisplayUserMenu();
void UserInputFileName(char*, char*);
bool OpenInputFile(char*, fstream&);
bool FileInput(RentalAgency*);
RentalAgency ReadRentalAgencyData(fstream&);
void ReadZipcode(int*, fstream&);
RentalCar ReadRentalCarData(fstream&);
void PrintAgencyData(RentalAgency*);
void EstimateTotalRentalCost(RentalAgency*);
RentalAgency AgencyChoice(RentalAgency*);
RentalCar CarChoice(RentalCar*);
void ShowMostExpensiveCar(RentalAgency*);
void PrintAvailableCarsToFile(RentalAgency*);
bool OpenOutputFile(char*, fstream&);
void Error();


int main()
{
	UserMenu();
	
	return 0;
}


/* -----------------------------------------------------------------------------
FUNCTION:          myStringCopy()
DESCRIPTION:       Copies a char array to a destination
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------- */

char* myStringCopy(char * destination, const char * source)
{
	while ((*destination++ = *source++));
	
	return destination;
}


/* -----------------------------------------------------------------------------
FUNCTION:          UserMenu()
DESCRIPTION:       Displays a menu the user picks an option from
RETURNS:           void
NOTES:             Based on Nick Learner's UserMenu() function in proj2.cpp
------------------------------------------------------------------------------- */

void UserMenu()
{
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
			hasfilebeenopened = FileInput(agencydata);
			break;
		case 2:
			if (!hasfilebeenopened)
			{
				cout << endl;
				cout << "Data has not yet been inputted. Please choose Option 1).\n\n";
			}else
			{
				cout << "\n\n";
				PrintAgencyData(agencydata);
				cout << "\n\n";
			}
			break;
		case 3:
			EstimateTotalRentalCost(agencydata);
			break;
		case 4:
			ShowMostExpensiveCar(agencydata);
			break;
		case 5:
			PrintAvailableCarsToFile(agencydata);
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
	cout << "5) Print data for available cars to a file" << endl;
	cout << "6) Quit program" << endl;
	
	cout << "Enter your choice [ 1 2 3 4 5 6 ]: ";
}


/* -----------------------------------------------------------------------------
FUNCTION:          FileInput()
DESCRIPTION:       Inputs the car data into an array of RentalAgency structs
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------*/

bool FileInput(RentalAgency* agencydata)
{
	char filetype[MAX_CHAR_ARRAY_SIZE] = "Input";
	char filename[MAX_CHAR_ARRAY_SIZE];
	bool hasfilebeenopened;
	fstream inputfile;
	
	UserInputFileName(filetype, filename);
	hasfilebeenopened = OpenInputFile(filename, inputfile);
	if (hasfilebeenopened)
	{
		cout << "File " << filename << " has been opened." << "\n";
		for (int i = 0; i < NUM_OF_AGENCIES; i++)
		{
			RentalAgency & agency = *agencydata++;
			agency = ReadRentalAgencyData(inputfile);
		} 
		cout << "Agency data has been inputted." << "\n";
		inputfile.close();
		cout << "File " << filename << " has been closed." << "\n\n";
	}else
	{
		cout << "File " << filename << " failed to open." << endl;
	}
	
	return hasfilebeenopened;
}


/* -----------------------------------------------------------------------------
FUNCTION:          UserInputFileName()
DESCRIPTION:       Gets a filename from user
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------*/

void UserInputFileName(char* type, char* filename)
{
	char inputstr[MAX_CHAR_ARRAY_SIZE];
	
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
FUNCTION:          ReadRentalAgencyData()
DESCRIPTION:       Reads in the info for an agency
RETURNS:           A RentalAgency object
NOTES:             
------------------------------------------------------------------------------- */

RentalAgency ReadRentalAgencyData(fstream& inputfile)
{
	RentalAgency agency;
	RentalCar car;
	RentalCar* inventoryptr = agency.inventory;
	
	inputfile >> agency.name;
	ReadZipcode(agency.zipcode, inputfile);
	for (int i = 0; i < NUM_OF_CARS; i++)
	{
		car = ReadRentalCarData(inputfile);
		*inventoryptr++ = car;
	}
	
	return agency;
}


/* -----------------------------------------------------------------------------
FUNCTION:          ReadZipcode()
DESCRIPTION:       Reads just the zipcode from the input file, and parses it
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------- */

void ReadZipcode(int* array, fstream& inputfile)
{
	int fivedigitint;
	int digit;
	int* arrayend = array + 4;
	
	inputfile >> fivedigitint;
	
	while (fivedigitint >= 10)
	{
		digit = fivedigitint % 10;
		*arrayend-- = digit;
		fivedigitint /= 10;
	}
	
	*arrayend = fivedigitint;
}


/* -----------------------------------------------------------------------------
FUNCTION:          ReadRentalCarData()
DESCRIPTION:       Reads the info for a car from the file
RETURNS:           A RentalCar object
NOTES:             
------------------------------------------------------------------------------- */

RentalCar ReadRentalCarData(fstream& inputfile)
{
	int year;
	char make[MAX_CHAR_ARRAY_SIZE], model[MAX_CHAR_ARRAY_SIZE];
	float price;
	bool available;
	
	inputfile >> year >> make >> model >> price >> available;
	RentalCar car(year, make, model, price, available);
	
	return car;
}


/* -----------------------------------------------------------------------------
FUNCTION:          PrintAgencyData()
DESCRIPTION:       Prints the info for each car to the console
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------- */

void PrintAgencyData(RentalAgency* agencydata)
{
	RentalCar* inventoryptr = agencydata->inventory;
	int* zipcodeptr = agencydata->zipcode;
	
	for (int j = 0; j < NUM_OF_AGENCIES; j++)
	{
		cout << agencydata->name << " ";
		for (int i = 0; i < 5; i++)
		{
			cout << *zipcodeptr;
			zipcodeptr++;
		}
		cout << endl;
		
		for (int i = 0; i < NUM_OF_CARS; i++)
		{
			inventoryptr->PrintData();
			inventoryptr++;
		}
		
		agencydata++;
		zipcodeptr = agencydata->zipcode;
		inventoryptr = agencydata->inventory;
	}
}


/* -----------------------------------------------------------------------------
FUNCTION:          EstimateRentalCost()
DESCRIPTION:       Estimates the total cost of a rental
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------- */

void EstimateTotalRentalCost(RentalAgency* agencydata)
{
	RentalAgency agency = AgencyChoice(agencydata);
	RentalCar car = CarChoice(agency.inventory);
	
	int days;
	float cost;
	
	cout << endl << "Enter the number of days you would ike to rent the "
		<< car.GetMake() << " " << car.GetModel() << ": " << endl;
	cin >> days;
	cout << endl;
	cost = car.EstimateCost(days);
	
	cout << "The total cost would be $" << cost << "." << "\n\n";
}


/* -----------------------------------------------------------------------------
FUNCTION:          AgencyChoice()
DESCRIPTION:       Allows the user to pick an agency to rent from
RETURNS:           RentalAgency
NOTES:             
------------------------------------------------------------------------------- */

RentalAgency AgencyChoice(RentalAgency* agencydata)
{
	int choice;
	RentalAgency agency;
	RentalAgency* agencyptrtofirst = agencydata;
	
	cout << endl << "Which Rental Agency?" << endl;
	for (int i = 0; i < NUM_OF_AGENCIES; i++)
	{
		cout << '[' << i << "] " << agencydata->name << endl;
		agencydata++;
	}
	
	agencydata = agencyptrtofirst;
	
	cout << "Enter your choice [ 0 1 2 ]: ";
	cin >> choice;
	
	if (choice > 2 || choice < 0)
	{
		choice = 0;
	}else;
	
	agency = *(agencydata + choice);
	
	cout << endl << "You have chosen the " << agency.name << " Rental Agency." << endl;
	
	return agency;
}


/* -----------------------------------------------------------------------------
FUNCTION:          CarChoice()
DESCRIPTION:       Allows the user to pick a car to rent
RETURNS:           A RentalCar object
NOTES:             
------------------------------------------------------------------------------- */

RentalCar CarChoice(RentalCar* cardata)
{
	int choice;
	RentalCar car = *cardata;
	RentalCar* carptrtofirst = cardata;
	
	cout << endl << "Which Car?" << endl;
	for (int i = 0; i < NUM_OF_CARS; i++)
	{
		car = *cardata++;
		cout << '[' << i << "] ";
		car.PrintData();
	}
	
	cardata = carptrtofirst;
	
	cout << "Enter your choice [ 0 1 2 3 4 ]: ";
	cin >> choice;
	
	if (choice > 4 || choice < 0)
	{
		choice = 0;
	}
	
	car = *(cardata + choice);
	
	return car;
}


/* -----------------------------------------------------------------------------
FUNCTION:          ShowMostExpensiveCar()
DESCRIPTION:       Self explanatory
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------- */

void ShowMostExpensiveCar(RentalAgency* agencydata)
{
	RentalCar car, mostexpensivecar = *(agencydata->inventory);
	RentalCar* inventoryptr;
	RentalAgency* agencyptrtofirst = agencydata;
	
	for (int i = 0; i < NUM_OF_AGENCIES; i++)
	{
		inventoryptr = agencydata->inventory;
		for (int j = 0; j < NUM_OF_CARS; j++)
		{
			car = *inventoryptr;
			if (car.GetPrice() > mostexpensivecar.GetPrice())
			{
				mostexpensivecar = car;
			}
			
			inventoryptr++;
		}
		
		agencydata++;
	}
	
	agencydata = agencyptrtofirst;
	
	cout << endl << "Most expensive car:" << endl;
	mostexpensivecar.PrintData();
	cout << endl;
}


/* -----------------------------------------------------------------------------
FUNCTION:          PrintAvailableCarsToFile()
DESCRIPTION:       Prints data for only the available cars to a file
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------- */

void PrintAvailableCarsToFile(RentalAgency* agencydata)
{
	char filetype[MAX_CHAR_ARRAY_SIZE] = "Output";
	char filename[MAX_CHAR_ARRAY_SIZE];
	fstream outputfile;
	bool hasfilebeenopened;
	
	RentalAgency* agencyptrtofirst = agencydata;
	RentalCar car;
	RentalCar* inventoryptr;
	int * zipcodeptr;
	
	UserInputFileName(filetype, filename);
	hasfilebeenopened = OpenOutputFile(filename, outputfile);
	
	if (hasfilebeenopened)
	{
		cout << "File " << filename << " has been opened." << "\n";
		
		for (int i = 0; i < NUM_OF_AGENCIES; i++)
		{
			inventoryptr = agencydata->inventory;
			zipcodeptr = agencydata->zipcode;
			
			outputfile << agencydata->name << " ";
			
			for (int j = 0; j < 5; j++)
			{
				outputfile << *zipcodeptr++;
			}outputfile << endl;
			
			for (int j = 0; j < NUM_OF_CARS; j++)
			{
				car = *inventoryptr;
				if (car.GetAvailable())
				{
					outputfile << car.GetYear() << " "
						<< car.GetMake() << " "
						<< car.GetModel() << " "
						<< car.GetPrice() << " " << boolalpha
						<< car.GetAvailable() << endl;
				}
				
				inventoryptr++;
			}
			
			agencydata++;
		}
		
		agencydata = agencyptrtofirst;
		
		cout << "Agency data has been outputted." << "\n";
		outputfile.close();
		cout << "File " << filename << " has been closed." << "\n\n";
	}else
	{
		cout << "File " << filename << " failed to open." << endl;
	}
	
	agencyptrtofirst = agencydata;
}


/* -----------------------------------------------------------------------------
FUNCTION:          OpenOutputFile()
DESCRIPTION:       Opens a file to output data into
RETURNS:           bool
NOTES:             
------------------------------------------------------------------------------- */
 
bool OpenOutputFile(char* filename, fstream& outputfile)
{
	outputfile.open(filename, ios::out);
	if (outputfile.fail())
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
