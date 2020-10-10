#ifndef CAPSULE_TOYS
#define CAPSULE_TOYS
#include "Objdatabase.h"
#include"CL_database.h"
#include"IntOfChose.h"

#include <windows.h>
#include <iomanip>
#include <ctime>
using std::vector;

class CapsuleToys // class to defraud user AvA
{
public:
	CapsuleToys(Objdatabase &theObjdatabase, CL_database &theCL);
	~CapsuleToys();
	void excute();
	void PlayTimes(int times, int mode);
private:
	void show_award_for_user(Obj raward,int rLevel);// show the award what user got
	void displayOption();// display game option
	int Judge_of_Level();// determine level
	Obj Get_Award();

	int RangeOfnNum = 10000; // 1~10000
	int usernum;
	Objdatabase &objDatabase;// contain object data of capsule toys
	CL_database &CLdatabase ;// contain level data
};

CapsuleToys::CapsuleToys(Objdatabase &theObjdatabase, CL_database &theCL)
	:objDatabase(theObjdatabase), CLdatabase(theCL)
{
}

CapsuleToys::~CapsuleToys()
{
}

//play capsule toys game AoA
void CapsuleToys::excute()
{
	// enumeration constants represent game options
	enum FuntionOption { CANCEL = 0, ONE, TEN, N };

	bool Cancel = false; // user has not chosen to cancel
	while (!Cancel)
	{ // loop while user has not chosen option to cancel the game 
		int times = 0; // user'play times, default is zero	

		displayOption(); // show the option of the game
		Choose inputInteger;// get user selection by class of Choose
		int FuntionSelection = inputInteger.excute(0, 3);
		switch (FuntionSelection)
		{// decide how to proceed based on user's menu selection
		case ONE:   // play one time
			PlayTimes(1,0);
			break;
		case TEN:   // play ten times
			PlayTimes(10,0);
			break;
		case N:     // custom how many times user want to play
			cout << "Input times: ";
			times = inputInteger.input_censor(); // prevent user input worng type
			PlayTimes(times,1);
			break;
		case CANCEL:// stop play the game 
			Cancel = true;
			break;
		default:    // user did not enter an integer from 0-3
			cout << "\nIncorrect choice! You ASS! " << endl;
			break;
		}
	}
}

// get award's level by the winning rate A3A
int CapsuleToys::Judge_of_Level()
{
	usernum = 0;
	usernum = rand() % RangeOfnNum + 1;	// generate user's number randomly
	return CLdatabase.Judge(usernum);	// judge level by the winning rate
}

// return random award by assigned level
Obj CapsuleToys::Get_Award()
{
	int Jlevel = Judge_of_Level(); // judge the random level
	int range = objDatabase.getSize_ofEachBucket(Jlevel); // get the range by size of level

	//int w = usernum % range;
	int w = rand() % range; // get the random position of data in judged level bucket

    // return the data which be chosen at randomly position [row][col]
	return objDatabase.getFile(Jlevel, w);
}

// play times by user decision
void CapsuleToys::PlayTimes(int times,int mode)
{
	try 
	{// if no data contain, tell user.
		for (int i = 1; i < objDatabase.level_bucket_size; ++i)
		{// bucket [0] have no data
			if (objDatabase.getSize_ofEachBucket(i) == 0)
				throw 0;
		}
	}
	catch (int err)
	{// catch error, tell user
		cout << " Incomplete data! please report or redownload! Moron!" << endl;
		return;
	}

	if (times == 0)
	{
		cout << "error!";
		return;
	}

	int a = objDatabase.level_bucket_size;
	int sum_inLevel[7] = {};//contain sum of object in each level
	int t = 0;

	double duration;
	clock_t start = clock();
	while (t < times)
	{// loop to run n play times
		Obj award;
		
		award = Get_Award();

		if (mode == 0)// show award directly
			show_award_for_user(award, award.getObjLevel());
		
		// ++award quantity which user got in level bucket of object
		else if (mode == 1)
		{
			objDatabase.get_LevelBucket()[award.getObjLevel()][award.getPosition_inVec()].plusObjQuantity();
			sum_inLevel[award.getObjLevel()]++;
		}
		t++;
	}
	duration = ((double)(clock() - start)) / CLOCKS_PER_SEC;
	cout << "\ntimes: " << duration << endl;

	if (mode == 1)
	{//in this mode, user can choose level of object to show 
		int level_to_show;
		Choose inputInteger;
		while (true)
		{//if level_to_show is zero, break. else show that level which corresponded with

			cout << "\nWhich level you want to show?(press 0 to cancel):";

			level_to_show = inputInteger.excute(0,6);
			for (int i = 0; i < objDatabase.get_LevelBucket()[level_to_show].size(); ++i)
			{
				if (objDatabase.get_LevelBucket()[level_to_show][i].getObjQuantity() > 0)
					show_award_for_user(objDatabase.get_LevelBucket()[level_to_show][i], level_to_show);
			}
			if (level_to_show == 0)
				break;
		}

		for (int i = 1; i <= 6; ++i)
			cout << "Level " << i <<": "<< sum_inLevel[i] << endl;
	
		for (int i = 0; i < objDatabase.level_bucket_size; ++i)
		{//reset Quantity of every object
			for (int j = 0; j < objDatabase.getSize_ofEachBucket(i); ++j)
			{
				if (objDatabase.get_LevelBucket()[i][j].getObjQuantity() > 0)
					objDatabase.get_LevelBucket()[i][j].setObjQuantity(0);
			}
		}
	}
}

// show the award what user got
void CapsuleToys::show_award_for_user(Obj raward,int rLevel)
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	cout << " " << setw(10) << " \nLevel:";

	if (rLevel == 6)
	{//if level equal to 6 change color to Yellow
		SetConsoleTextAttribute(hOut,
			FOREGROUND_RED |
			FOREGROUND_GREEN |
			FOREGROUND_INTENSITY);
		cout << "******";// six star
		cout << setw(20) << raward.getName() << " << Legendary!!! >>";
		if (raward.getObjQuantity() > 1)
			cout << " | Quantity:" << raward.getObjQuantity();
		cout << endl;
	}
	else if (rLevel == 5)
	{//if level equal to 5 change color to Magenta
		SetConsoleTextAttribute(hOut,
			FOREGROUND_BLUE |
			FOREGROUND_RED |
			FOREGROUND_INTENSITY);
		cout << "*****";// five star
		cout << setw(21) << raward.getName() << " << Epic!! >>";
		if (raward.getObjQuantity() > 1)
			cout << " | Quantity:" << raward.getObjQuantity();
		cout << endl;
	}
	else if (rLevel == 4)
	{//if level equal to 4 change color to Red
		SetConsoleTextAttribute(hOut,
			FOREGROUND_RED |
			FOREGROUND_INTENSITY);
		cout << "****";// four star
		cout << setw(22) << raward.getName() << " << Rare! >>";
		if (raward.getObjQuantity() > 1)
			cout << " | Quantity:" << raward.getObjQuantity();
		cout << endl;
	}
	else if (rLevel == 3)
	{//if level equal to 3 change color to Cyan
		SetConsoleTextAttribute(hOut,
			FOREGROUND_GREEN |
			FOREGROUND_BLUE |
			FOREGROUND_INTENSITY);
		cout << "***";// three star
		cout << setw(23) << raward.getName();
		if (raward.getObjQuantity() > 1)
			cout << " | Quantity:" << raward.getObjQuantity();
		cout << endl;
	}
	else if (rLevel == 2)
	{//if level equal to 2 change color to Green
		SetConsoleTextAttribute(hOut,
			FOREGROUND_GREEN |
			FOREGROUND_INTENSITY);
		cout << "**";// two star
		cout << setw(24) << raward.getName();
		if (raward.getObjQuantity() > 1)
			cout << " | Quantity:" << raward.getObjQuantity();
		cout << endl;
	}
	else
	{//if level equal to 1 is White color
		SetConsoleTextAttribute(hOut,
			FOREGROUND_RED |
			FOREGROUND_GREEN |
			FOREGROUND_BLUE |
			FOREGROUND_INTENSITY);
		cout << "*";// one star
		cout << setw(25) << raward.getName();
		if (raward.getObjQuantity() > 1)
			cout << " | Quantity:" << raward.getObjQuantity();
		cout << endl;
	}
	SetConsoleTextAttribute(hOut,//return to defaut color
		FOREGROUND_RED |
		FOREGROUND_GREEN |
		FOREGROUND_BLUE);
}

// display the option of the game
void CapsuleToys::displayOption()
{
	cout << endl
		<< "Press < 1 > to play one time;\n"
		<< "Press < 2 > to play ten times;\n"
		<< "Press < 3 > to play n times;\n"
		<< "Press < 0 > to cancel.\n" << endl;
	cout << "Make a choice: ";
}

#endif // !1
/* A_A  Let's defraud all consumer by Disposable Games!!!!!! / 咱們來用免洗遊戲坑錢吧!!!!!!  A_A */

//BUG: 玩n次顯示等級如果沒有東西不會通知