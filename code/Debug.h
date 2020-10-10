#ifndef DEBUG
#define DEBUG
#include"Objdatabase.h"
#include"CL_database.h"
#include"Debug_Censor.h"
#include"IntOfChose.h"

class Debug	// admin have many function about capsule toys.
{
public:
	Debug::Debug(Objdatabase&theObj, CL_database&theLevel);
	void execute();
	void creatNewObj();	  //creat new object 
	void creatNewLevel(); //creat new level of objects
	void editObject();	  //edit the object which admin want
	void editLevel();     //edit the level which admin want
	void eraseLevel();    //erase level
	void eraseObject(int rpos,int cpos); //erase the object
private:
	void modifyObj(int rdataPos, int cdataPos, int option);//function to modify object data
	void modifyLevel();		//function to modify level data
	void displayFunction(); //show the fuction menu of debug mode
	void showObjEditMenu(); //show the menu of edit object mode
	void showObjModSelet(int rdataPos,int cdataPos);// funtion to ask admin which data will be modified
	Obj newObject;
	Objdatabase &objdatabase;
	ChanceOfLevel newLevel;
	CL_database &cl_database;
};

Debug::Debug(Objdatabase&theObj, CL_database&theLevel)
	:objdatabase(theObj), cl_database(theLevel)
{
}

void Debug::execute()
{
	Censor Password_Test; // check the password, 
	if (Password_Test.checkPassWord())
	{//when admin input the wrong password if will false
		cout << "\n------------Debug Mode-------------\n";
		enum FuntionOption { CANCEL = 0, ADDOBJ, ADDLEVEL, EDITO, EDITL};
		bool Cancel = false;
		while (!Cancel)
		{
			displayFunction();  //show the fuction of debug mode
			Choose inputInteger;// get admin selection by class of Choose
			int FuntionSelection = inputInteger.excute(0, 4);

			switch (FuntionSelection)// decide how to proceed based on admin's menu selection
			{
			case ADDOBJ:  // add the new object of capsule toys
				creatNewObj();
				break;
			case ADDLEVEL:// add the new level of capsule toys
				creatNewLevel();
				break;
			case EDITO:   // admin want to edit specific object  
				editObject();
				break;
			case EDITL:   // admin want to edit level 
				editLevel();
				break;
			case CANCEL: // exit debug mode
				Cancel = true;
				break;
			default:	// user did not enter an integer from 0-3
				cout << "\nIncorrect choice! You are an idiot! " << endl;
				break;
			}//end switch
		}//end while
	}//end if
	else { cout << "\nwrong password!" << endl; }
}
//function to add new object
void Debug::creatNewObj()
{
	char name[30];
	cout << "Input the name of newobject: ";
	cin.ignore();
	cin.get(name, 30, '\n');
	newObject.setName(name);

	if (objdatabase.duplicateName(newObject.getName()))
	{//if system has same object, return
		cout << "You are already have a object of this system! \n" << endl;
		return;
	}
	char ID[30];
	cout << "Input your ID Number of object: ";
	cin.ignore();
	cin.get(ID, 30, '\n');
	newObject.setIDNumber(ID);

	if (objdatabase.duplicateID(newObject.getIDNumber()))
	{//if system has same ID, return
		cout << "You are already have a object of this system!\n" << endl;
		return;
	}
	int level;
	cout << "Input your level of object: ";
	cin >> level;
	if (!cl_database.duplicateLevel(level))
	{//if adnim input the level does not exist in data, return
		cout << "No level exist in the system!\n";
		return;
	}
	newObject.setObjLevel(level);
	cout << endl << "Successful!!" << endl;
	objdatabase.pushBack(newObject);
	objdatabase.save_Edited_OBJFile(level);// save new data into OBJfile
}
//function to add new chance of level
void Debug::creatNewLevel()
{
	int level;
	cout << "Input new level: ";
	cin >> level;
	newLevel.setLevel(level);
	if (cl_database.duplicateLevel(newLevel.getLevel()))
	{//if here has same level, return
		cout << "You are already have the same level of this system! \n" << endl;
		return;
	}
	float Chance;
	cout << "Input the chance of level (Round off to second decimal places): ";
	cin >> Chance;

    // ensure round off to second decimal places
	Chance = (float)((int)(Chance * 100)*0.01);

	if (cl_database.Chance_Overload(Chance))
	{// if sum of all chance of level over 100, return.
		cout << "\nPercent of chance used: "
			<< cl_database.sum_of_chance / 100 << "%" << endl
			<< "overloading!!" << endl;
		return;
	}
	newLevel.setChance(Chance);
	cout << endl << "Successful!!" << endl;
	cl_database.pushBack(newLevel);
	cl_database.save_Edited_CLFile();// save new data into CLfile
}
//function to edit the object which admin wanted
void Debug::editObject()
{
	enum FuntionOption { EXIT = 0, SHOWDATA, EDITDATA, ERASEDATA };
	Choose inputInteger;// get admin selection by class of Choose
	string searcher; // search specified capsule toy by ID
	int adminInput;
	do
	{
		showObjEditMenu();  //show the menu of edit object mode
		adminInput = inputInteger.excute(0, 3);
		if (adminInput == SHOWDATA)
		{// just show the data of capsule toys
			cout << "Which level you want to show? (press 0 to show the all data) ";
			Choose inputLevel;
			int level;

			// define range of num by size of level bucket, but level 0 is undefind in bucket, 
			// to get correct size, we sub 1
			// 0 is show the all data 
			level = inputLevel.excute(0, objdatabase.level_bucket_size - 1);
			objdatabase.showData(level, -1);
		}
		else if (adminInput == EDITDATA)
		{// edit the data
			cout << "Input the ID of object you want to edit: ";
			cin.ignore();
			getline(cin, searcher, '\n');

			objdatabase.findIDdata(searcher); // find the data position in vector and set row/column position

			// get the position in row/column of data
			int rdataPos = objdatabase.getRowPos(), cdataPos = objdatabase.getColPos();

			if (rdataPos > 0 && cdataPos >= 0)// row data(rdata) [0] is undefined
			{ //found the data and edit it
				enum FuntionOption { CANCEL = 0, EDITNAME, EDITID, EDITLEVEL };

				bool Cancel = false;
				while (!Cancel)
				{
					// tell admin which assigned data by dataPos will be modified
					showObjModSelet(rdataPos, cdataPos);

					int editer = inputInteger.excute(0, 3);
					switch (editer)// edit three different data of capsule toy
					{
					case EDITNAME: // modify the name of capsule toy
						modifyObj(rdataPos, cdataPos, EDITNAME);
						break;
					case EDITID:   // modify the ID of capsule toy
						modifyObj(rdataPos, cdataPos, EDITID);
						break;
					case EDITLEVEL:// modify the level of capsule toy
						modifyObj(rdataPos, cdataPos, EDITLEVEL);
						break;
					case CANCEL:   // admin cancel to edit and end the function
						Cancel = true;
						break;
					default: // admin did not enter an integer from 0-3
						cout << "\ninvalid input!" << endl;
						break;
					} // end switch
				} // end while
			} // end if
			else
			{ // no data be found 
				cout << "No data is here, would you want to creat new one? (Y/N)";
				char deside;
				while (true)
				{ 
					cin >> deside;
					if (deside == 'Y' || deside == 'y')
					{// creat the new object
						creatNewObj();
						break;
					}
					else if (deside == 'N' || deside == 'n')
					{// do not creat the new object 
						break;
					}
					else cout << "invalid input!!";
				}// end while
			}//end else
		}//end else if
		else if (adminInput == ERASEDATA)
		{// erase the data
			cout << "Input the ID of object you want to erase: ";
			cin >> searcher;
			objdatabase.findIDdata(searcher); // find the data position in vector and set row/column position
			
            // get the position in row/column of data								 
			int rdataPos = objdatabase.getRowPos(), cdataPos = objdatabase.getColPos();
			if (rdataPos < 0 || cdataPos < 0)
			{
				cout << "No data contain!" << endl;
				break;
			}
			objdatabase.showData(rdataPos, cdataPos);
			cout << "Confirm? (Y/N)";
			char deside;
			while (true)
			{
				cin >> deside;
				if (deside == 'Y' || deside == 'y')
				{// erase the data and return
					eraseObject(rdataPos, cdataPos);// erase data at [rpos][cpos]
					cout << "successful!" << endl;
					break;
				}
				else if (deside == 'N' || deside == 'n')
					break;
				else cout << "Invalid input! 87! " << endl;
			}
		}

	} while (adminInput != 0); //end do while
}
//function to edit level & chance of level
void Debug::editLevel()
{
	enum FuntionOption { CANCEL = 0, EDITLEVEL,DELETELEVEL};
	bool Cancel = false;
	int decision;
	Choose inputInteger;// get admin selection by class of Choose
	while (!Cancel)
	{
		cout << "\nChance used (<100%):"//tell admin how much percent of chance used
			<< cl_database.sum_of_chance / 100 << "%" << endl;
		cl_database.showData(-1);// show the all level data

		cout << "( 1 )To start, ( 0 )To end: ";
		decision = inputInteger.excute(0, 2);
		switch (decision)
		{
		case EDITLEVEL:// modify chance of level
			modifyLevel();
			break;
		case DELETELEVEL:// modify chance of level
			eraseLevel();
			break;
		case CANCEL:  // admin cancel to edit level data
			Cancel = true;
			break;
		default: // admin did not enter an integer from 0-2
			cout << "\nInvalid input!" << endl;
			break;
		}//end switch
	}//end while
}
// modify the data of assigned capsule toy by position [i] in vector
void Debug::modifyObj(int rdataPos,int cdataPos,int Option)
{
	enum option { MODNAME = 1, MODID, MODLEVEL };
	if (Option == MODNAME)
	{// modify name
		string newName;
		cout << "set the new name of capsule toy: ";
		cin.ignore();
		getline(cin, newName,'\n');
		if (objdatabase.duplicateName(newName))
		{//if system has same object, return
			cout << "\nThere is already a object of this system! \n" << endl;
			return;
		}
		objdatabase.resetName(rdataPos, cdataPos, newName);
		objdatabase.save_Edited_OBJFile(rdataPos);// save name data
		cout << "successful!!\n";
	}
	else if (Option == MODID)
	{ // modify ID
		string newID;
		cout << "set the new ID of capsule toy: ";
		cin.ignore();
		getline(cin, newID, '\n');
		if (objdatabase.duplicateID(newID))
		{//if system has same ID, return
			cout << "\nThere is already a object of this system!" << endl;
			return;
		}
		objdatabase.resetID(rdataPos, cdataPos, newID);
		objdatabase.save_Edited_OBJFile(rdataPos);// save ID data
		cout << "successful!!\n";
	}
	else if (Option== MODLEVEL)
	{ // modify level
		int newLevel;
		cout << "set the new Level of capsule toy: ";
		cin >> newLevel;
		if (!cl_database.duplicateLevel(newLevel))
		{//if adnim input the level does not exist in data, return
			cout << "No level exist in the system!(press 1 to creat, press 0 to skip)";
			Choose inputInteger;// get admin selection by class of Choose
			int option = inputInteger.excute(0, 1);
			if (option == 1)
			{// creat and return
				creatNewLevel();
				return;
			}
			else return;
		}
		objdatabase.resetLevel(rdataPos, cdataPos, newLevel);
		objdatabase.save_Edited_OBJFile(rdataPos);// save level data of obj
		cout << "successful!!\n";
	}
}
// function to edit chance of level
void Debug::modifyLevel()
{
	int levelPos;    // position of level data
	int choo_level;  // choose level to modify
	float mod_chance;// modify chance

	cout << "\nWhich level you want to modify?";
	cin >> choo_level;
	levelPos = cl_database.findLevel(choo_level);
	if (levelPos >= 0)
	{// find the position of assigned level
		cl_database.showData(levelPos);
		cout << "reset chance number: ";
		cin >> mod_chance;

		//ensure round off to second decimal places
		mod_chance = (float)((int)(mod_chance * 100)*0.01);

		//subtract original chance which be edited from sum_of_chance
		cl_database.sum_of_chance -= 100 * cl_database.getFile(levelPos).getChance();

		if (cl_database.Chance_Overload(mod_chance))
		{// re-plus original chance, tell admin overloading
			cl_database.sum_of_chance += 100 * cl_database.getFile(levelPos).getChance();
			cout << "\nPercent of chance used: " << cl_database.sum_of_chance / 100 << "%" << endl;
			cout << "overloading!!" << endl;
			return;
		}
		cl_database.resetChance(levelPos, mod_chance);
		cl_database.sum_of_chance += 100 * mod_chance;// re-plus new set chance to sum_of_chance
		cl_database.save_Edited_CLFile();// save chance of level data
		cout << "successful!!\n";
	}
	else 
	{// no level data be found
		cout << "no level data here, creat new one?(Y/N)";
		char deside;
		while (true)
		{ 
			cin >> deside;
			if (deside == 'Y' || deside == 'y')
			{// creat the new level
				creatNewLevel();
				break;
			}
			else if (deside == 'N' || deside == 'n')
			{// do not creat the new level
				break;
			}
			else cout << "invalid input!!";
		}//end while
	}//end else 
}
//
void Debug::eraseObject(int rpos, int cpos)
{
	objdatabase.erase(rpos, cpos);
}

void Debug::eraseLevel()
{
	int levelPos;   // position of level data
	int choo_level; // choose level to erase

	cout << "\nWhich level you want to erase?";
	cin >> choo_level;
	levelPos = cl_database.findLevel(choo_level);
	if (levelPos >= 0)
	{
		cl_database.showData(levelPos);
		cout << "Confirm? (Y/N)";
		char deside;
		while (true)
		{
			cin >> deside;
			if (deside == 'Y' || deside == 'y')
			{// erase level and return
				cl_database.erase(levelPos);
				cout << "successful!!\n";
				break;
			}
			else if (deside == 'N' || deside == 'n')
				break;
			else cout << "invalid input! Jerk!\n";
		}
	}
	else cout << "No level contain!" << endl;
}
// display function of debug mode
void Debug::displayFunction()
{
	cout << "\n< 1 > - Add new object" << endl;
	cout << "< 2 > - Add new level" << endl;
	cout << "< 3 > - Edit object" << endl;
	cout << "< 4 > - Edit Level" << endl;
	cout << "< 0 > - End the debug mode \n" << endl;
	cout << "Enter your choice: ";
}
//display the menu of edit object mode
void Debug::showObjEditMenu()
{
	cout << "\n< 1 > Show the capsule toy data" << endl
		<< "< 2 > Edit" << endl
		<< "< 3 > Delete" << endl
		<< "< 0 > Exit\n" << endl
		<< "Enter your choice: ";
}
// function to ask admin which data will be modified
void Debug::showObjModSelet(int rdataPos,int cdataPos)
{
	cout << endl << "--------------------------------------------" << endl;
	objdatabase.showData(rdataPos,cdataPos);
	cout << "       ( 1 ) Name, ( 2 ) ID, ( 3 ) Level" << endl
		<< " Which one would you edit?( press 0 to cancel): ";
}
#endif // !DEBUG
/* A_A  Let's defraud all consumer by Disposable Games!!!!!! / 咱們來用免洗遊戲坑錢吧!!!!!!  A_A */


//BUG: 更改物品的level後並不會變動其所在的level bucket