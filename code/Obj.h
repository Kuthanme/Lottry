#ifndef OBJ
#define OBJ
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;
using std::string;

class Obj // define the object
{
public:
	Obj();

	string getName()const;
	void setName(string theName);

	string getIDNumber()const;
	void setIDNumber(string theIDNumber);

	int getObjLevel()const;
	void setObjLevel(int rlevel);

	int getPosition_inVec()const;
	void setPosition_inVec(int rpos);

	int getObjQuantity()const;
	void setObjQuantity(int rQuantity);
	void plusObjQuantity();
	
private:
	char name[30];
	char IDNumber[30];
	int objLevel;
	int position; //return object's position 
	int Quantity; //
};

Obj::Obj()
	:Quantity(0)
{
}

string Obj::getName() const
{
	string buffer(name);
	return buffer;
}

void Obj::setName(string theName)
{
	strcpy_s(name, theName.c_str());
}

string Obj::getIDNumber() const
{
	string buffer(IDNumber);
	return buffer;
}

void Obj::setIDNumber(string theIDNumber)
{ 
	strcpy_s(IDNumber, theIDNumber.c_str());
}

int Obj::getObjLevel()const
{
	return objLevel;
}

void Obj::setObjLevel(int rlevel)
{
	objLevel = rlevel;
}

int Obj::getPosition_inVec()const
{
	return position;
}

void Obj::setPosition_inVec(int rpos)
{
	position = rpos;
}

int Obj::getObjQuantity()const
{
	return Quantity;
}

void Obj::setObjQuantity(int rQuantity)
{
	Quantity = rQuantity;
}

void Obj::plusObjQuantity()
{
	Quantity++;
}
#endif
/* A_A  Let's defraud all consumer by Disposable Games!!!!!! / 咱們來用免洗遊戲坑錢吧!!!!!!  A_A */