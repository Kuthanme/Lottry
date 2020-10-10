#ifndef CHANCE_OF_LEVEL
#define CHANCE_OF_LEVEL
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;
using std::string;

class ChanceOfLevel	// define each level's chance 
{
public:
	void setLevel(int thelevel);
	int getLevel()const;
	void setChance(float thechance);
	float getChance();
private:
	int Level;
	float Chance;
};

void ChanceOfLevel::setLevel(int thelevel)
{
	Level = thelevel;
}

int ChanceOfLevel::getLevel()const
{
	return Level;
}

void ChanceOfLevel::setChance(float thechance)
{
	Chance = thechance;
}

float ChanceOfLevel::getChance()
{
	return Chance;
}
#endif // !CHANCE_OF_LEVEL
/* A_A  Let's defraud all consumer by Disposable Games!!!!!! / 咱們來用免洗遊戲坑錢吧!!!!!!  A_A */