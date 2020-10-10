#ifndef CL_DATABASE
#define CL_DATABASE
#include<algorithm>
#include <iomanip>
#include <vector>
using std::vector;

#include"Chance_of_Level.h"

// define how to compare by level
bool compareByLevel(const ChanceOfLevel &lhs, const ChanceOfLevel &rhs);

class CL_database // contain data of every levels
{
public:
	CL_database();
	~CL_database();
	ChanceOfLevel getFile(unsigned int i)const;
	vector< ChanceOfLevel > getdata()const;
	bool Chance_Overload(float rchance);
	bool duplicateLevel(int tLevel);
	void resetChance(unsigned int i, float &rChance);
	int Judge(int userNum);   // judge level
	int findLevel(int &rLV);  // search level
	int getSize()const;
	void save_Edited_CLFile();// when object data has been edited, save it immediately
	void showData(int pos);   // show the level data
	void pushBack(ChanceOfLevel &newLevel);
	void erase(int i);

	float sum_of_chance;    // all chance of level occupied percentage

protected:
	void JudgeLevelWinRange();// define win range of level
	void loadFromCLFile();
	void saveToCLFile();

	float *RangeOfLevel_F = nullptr; // first of range
	float *RangeOfLevel_L = nullptr; // last of range
	vector< ChanceOfLevel > Ldata;
};
// constructor	
CL_database::CL_database()	
{
	loadFromCLFile();

	//sort cl_data from low lv to high lv
	sort(Ldata.begin(), Ldata.end(), compareByLevel);

	JudgeLevelWinRange();
}
// destructor
CL_database::~CL_database()	
{
	if (RangeOfLevel_F != nullptr)
		delete[] RangeOfLevel_F;
	if (RangeOfLevel_L != nullptr)
		delete[] RangeOfLevel_L;
	saveToCLFile();
}

ChanceOfLevel CL_database::getFile(unsigned int i)const
{
	return Ldata[i];
}

vector< ChanceOfLevel > CL_database::getdata()const
{
	return Ldata;
}
// function to judge if overload
bool CL_database::Chance_Overload(float rchance)
{// if sum of all chance of level over 100%, return true.
	if ((sum_of_chance + 100*rchance) > 10000)
		return true;
	return false;
}
// check if have same level
bool CL_database::duplicateLevel(int tLevel) 
{
	for (unsigned i = 0; i < Ldata.size(); i++)
	{
		if (tLevel == Ldata[i].getLevel())
			return true;
	}
	return false;
}
//reset chance of level by assign i
void CL_database::resetChance(unsigned int i, float &rChance)
{
	Ldata[i].setChance(rChance);
}
// this function can save level data immediately
void CL_database::save_Edited_CLFile()
{
	saveToCLFile();
}
// get size of level data
int CL_database::getSize()const
{
	return Ldata.size();
}
// judge the level which user got
int CL_database::Judge(int userNum)
{
	for (unsigned int i = 0; i < Ldata.size(); ++i)
	{
		if (RangeOfLevel_F[i] <= userNum && userNum <= RangeOfLevel_L[i])
			return Ldata[i].getLevel();
	}
	return Ldata[0].getLevel();// if user don't get the higher level(>1), return lowest level: 1
}
// find level if exist
int CL_database::findLevel(int &rLV)
{
	for (unsigned int i = 0; i < Ldata.size(); ++i)
	{
		if (Ldata[i].getLevel() == rLV)
			return i;//return position in vector 
	}
	return -1;// if no data be found return minus number
}
// show the data of level
void CL_database::showData(int pos)
{
	if (pos < 0) 
	{// if i is no defided position, show the all data
		for (unsigned int i = 0; i < Ldata.size(); i++)
			showData(i);//recursive
	}
	else
	{//print out data
		cout << "Level: " << Ldata[pos].getLevel() << "| "
			<< "Chance: " << setw(5) << Ldata[pos].getChance()
			<< "%" << endl;
	}
}

void CL_database::pushBack(ChanceOfLevel &newLevel)
{
	Ldata.push_back(newLevel);
}

void CL_database::erase(int i)
{
	vector<ChanceOfLevel>::iterator iter = Ldata.begin();
	Ldata.erase(iter + i);
}
// every level's win range should be defined
void CL_database::JudgeLevelWinRange()
{
	RangeOfLevel_F = new float[Ldata.size()](); // first of range of each level		  
	RangeOfLevel_L = new float[Ldata.size()](); // last of range of each level
	for (unsigned int i = 1; i < Ldata.size(); ++i)
	{// from second low level to define
		if (i == 1)// not including lowest level 0, because it is the level user guarantee to get
		{// define first range of level
			RangeOfLevel_F[i] = 1;
			RangeOfLevel_L[i] = 100 * Ldata[i].getChance();
			sum_of_chance = 100 * Ldata[i].getChance();// caculate the sum of first chance of level, meanwhile
		}//Ex:if first is 0.5%, we multiply by 100. 
		 //range of first level is (0.5*100=50)1~50.
		else
		{// define after range of level
			RangeOfLevel_F[i] = RangeOfLevel_L[i - 1] + 1;
			RangeOfLevel_L[i] = RangeOfLevel_F[i] + 100 * Ldata[i].getChance();
			sum_of_chance += 100 * Ldata[i].getChance();// caculate the sum of another chance of level, meanwhile
		}//Ex:if range of first is 1~50, chance of next level is 1%, 
		 //then range of second level is 50+1+1*100 = 151(51~151).
	}
}
//load level data
void CL_database::loadFromCLFile()
{
	ChanceOfLevel CL_load;	//contain data from file
	fstream  CoL_load("CL.dat", ios::in | ios::binary);	//file which has the info of chance of level
	if (!CoL_load)
	{
		CoL_load.close();
		cout << "File CL.dat could not be opened!" << endl;
		ofstream inFile("CL.dat", ios::out | ios::binary);
		inFile.close();
		CoL_load.open("CL.dat", ios::in | ios::binary);
		system("pause");
	}
	CoL_load.seekg(0, ios::beg);
	CoL_load.read(reinterpret_cast<char*>(&CL_load), sizeof(ChanceOfLevel));
	while (!CoL_load.eof())
	{
		Ldata.push_back(CL_load);
		CoL_load.read(reinterpret_cast<char*>(&CL_load), sizeof(ChanceOfLevel));
	}
	CoL_load.close();
}
//save level data
void CL_database::saveToCLFile()
{
	fstream CL_write("CL.dat", ios::out | ios::binary);
	if (!CL_write)
	{
		CL_write.close();
		cout << "File CL.dat could not be opened." << endl;
		ofstream inFile("CL.dat", ios::out | ios::binary);
		inFile.close();
		CL_write.open("CL.dat", ios::in | ios::binary);
		system("pause");
	}
	CL_write.seekp(0, ios::beg);
	for (unsigned i = 0; i < Ldata.size(); i++)
	{
		CL_write.write(reinterpret_cast<char*>(&Ldata[i]), sizeof(ChanceOfLevel));
	}
	CL_write.close();
}
// define how to compare level
bool compareByLevel(const ChanceOfLevel &lhs, const ChanceOfLevel &rhs)
{// from low to high
	return lhs.getLevel() < rhs.getLevel();
}
#endif // !CL_DATABASE
/* A_A  Let's defraud all consumer by Disposable Games!!!!!! / 咱們來用免洗遊戲坑錢吧!!!!!!  A_A */