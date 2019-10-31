/* -----------------------------------------------------------------------------

FILE:           proj3.h
DESCRIPTION:    Header file for Project 3 for Fall 2019 CS 202-1001 at UNR;
					Assists users in renting a car from one of three different companies
COMPILER:       MinGW C++ compiler
NOTES:          

MODIFICATION HISTORY:
Author				Date			Version
---------------		----------		--------------
Version 1			YYYY-MM-DD		x.x Description
Nick Learner		2019-09-19		1.0	Created header file
----------------------------------------------------------------------------- */

#ifndef PROJ3_H
#define PROJ3_H

#define MAX_CHAR_ARRAY_SIZE 256

#define NUM_OF_CARS 5

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

struct RentalAgency
{
	char name[MAX_CHAR_ARRAY_SIZE];
	int zipcode[5];
	RentalCar inventory[NUM_OF_CARS];
};

#endif