#ifndef DEBUG_CENSOR
#define DEBUG_CENSOR
#include<iostream>
#include <conio.h>
#include<string>
using namespace std;
using std::string;
class Censor // check the password
{
public:
	Censor();
	~Censor();
	bool checkPassWord();

private:
	char Password[9] = "defraudu";

};
Censor::Censor()
{
}
Censor::~Censor()
{
}
bool Censor::checkPassWord()
{
	cout << "Please input the password (8 keys): \n";
	char key;
	char inputpass[30];
	int countx = 0;
	while ((key = _getwch()) != '\r') //enter==13
	{
		inputpass[countx] = (char)key;
		if (key == '\b')
		{//if input key == backspace, deleate one *
			cout << '\b' << ' ' << '\b';
			if (countx - 1 > 0)
				countx--;
			else
				countx = 0;
		}
		else if (countx < 8)
		{
			cout << '*';
			countx++;
		}
		else {}
	}
	inputpass[countx] = '\0';

	for (int i = 0; i < 8; i++)
	{
		if (inputpass[i] != Password[i])
			return false;
	}
	return true;
}
#endif

/* A_A  Let's defraud all consumer by Disposable Games!!!!!! / 咱們來用免洗遊戲坑錢吧!!!!!!  A_A */