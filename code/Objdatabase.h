#ifndef OBJDATABASE
#define OBJDATABASE
#include"CL_database.h"
#include"Obj.h"

class CapsuleToys;
bool compareByID(const Obj &lhs, const Obj &rhs);
class Objdatabase // contain every objects
{
public:
	Objdatabase(int rlevel_bucket_size);
	~Objdatabase();
	bool duplicateID(string &userIDNumber); // check if userIDNumber belongs to objects
	bool duplicateName(string &tName); // check if Name belongs to objects
	void findIDdata(string &rID);// find data by item's ID
	void showData(int rpos,int cpos);// show the object data
	void resetName(unsigned int i, unsigned int j, string &rName);
	void resetID(unsigned int i, unsigned int j, string &rID);
	void resetLevel(unsigned int i, unsigned int j, int &rLevel);
	void save_Edited_OBJFile(int rpos);// when object data has been edited, save it immediately
	void define_pos_ofuserObj_inVec();
	int getSize_ofEachBucket(unsigned int i)const;
	int getRowPos()const; // get row pos of obj
	int getColPos()const; // get col pos of obj
	void pushBack(Obj &newObj);
	void erase(int rpos, int cpos);
	Obj getFile(unsigned int i, unsigned int j)const;
	vector< Obj >* get_LevelBucket ()const;// function to return pointer witch point to bucket array

	int level_bucket_size;// [0] is undefind 
	
protected:
	void loadFromOBJFile();
	void saveToOBJFile();

	int row, column; // row: number of bucket, defined by level/column: number of object data
	vector< Obj >* LevelBucket = nullptr; // pointer, point to level bucket array
};

// constructor, get the level size to implement the bucket size
Objdatabase::Objdatabase(int rlevel_bucket_size) 
{
	// determine size of bucket, but [0] we never use it, because lowest level is 1
	// if bucket size is 6, plus 1 become 7 and [1]to[6] are used
	level_bucket_size = rlevel_bucket_size + 1; 

	// generated bucket and user bucket by array and every bucket contain one vector 
	LevelBucket = new vector< Obj >[level_bucket_size](); 

	loadFromOBJFile(); // load data and push in bucket by function: pushBack()

	for (int i = 1; i < level_bucket_size; ++i)
	{// each level's object should be sorted
		sort(LevelBucket[i].begin(), LevelBucket[i].end(), compareByID);
	}

	define_pos_ofuserObj_inVec();
}
// destructor, save the object data
Objdatabase::~Objdatabase()
{
	saveToOBJFile();
	delete[]LevelBucket;
}
// check if IDNumber of restaurant belongs to members
bool Objdatabase::duplicateID(string &IDNumber)
{
	for (int i = 1; i < level_bucket_size; ++i)
	{
		int left = 0, right = LevelBucket[i].size() - 1;
		while (left <= right)
		{// use binary search
			int middle = (right + left) / 2;

			if (LevelBucket[i][middle].getIDNumber() == IDNumber)
				return true;  // find name

			if (LevelBucket[i][middle].getIDNumber() > IDNumber)
				right = middle - 1;
			else
				left = middle + 1;
		}
	}
	return false;
}
// check if Name belongs to members
bool Objdatabase::duplicateName(string &rName) 
{
	for (int i = 1; i < level_bucket_size; ++i)
	{
		for (int j = 0; j < LevelBucket[i].size(); ++j)
		{
			if (LevelBucket[i][j].getName() == rName)
				return true;
		}
	}
	return false;
}
//find data position in bucket by ID number of data 
void Objdatabase::findIDdata(string &rID)
{// find data by ID, give row/col position by [i][j] of vector 
	for (int i = 1; i < level_bucket_size; ++i)
	{
		int left = 0, right = LevelBucket[i].size() - 1;
		while (left <= right)
		{// use binary search
			int middle = (right + left) / 2;

			if (LevelBucket[i][middle].getIDNumber() == rID)
			{
				row = i; column = middle;
				return;
			}

			// because type of ID is 'string', So we must transfer it to 'int' to use operator ">"
			if (stoi(LevelBucket[i][middle].getIDNumber()) > stoi(rID))
				right = middle - 1;
			else
				left = middle + 1;
		}
	}
	row = -1; column = -1;// no data be found, give minus number to row/col
}
// display data of capsule toys by assigned 
// rpos = row position in array of vector type, (level bucket)
// cpos = column position in vector of array (data whicth every level bucket contain)
void Objdatabase::showData(int rpos, int cpos)
{
	cout << "Level| " << setw(20) << "Name " << setw(9) << "  |  " << setw(2) << "ID" 
		<< "\n===========================================" << endl;
	if (rpos <= 0 && cpos < 0)
	{// if rpos and cpos is undefided position, show the all data
		for (int i = 1; i < level_bucket_size; i++)
		{// for loop to show the all data in row/col position
			for (int j = 0; j < LevelBucket[i].size(); j++)
			{
				cout << setw(3) << LevelBucket[i][j].getObjLevel() << setw(4) << "| " << setw(25)
					<< LevelBucket[i][j].getName() << " | "
					<< LevelBucket[i][j].getIDNumber() << endl;	
			}
			cout << "--------------------------------------------" << endl;
		}
	}
	else if (rpos > 0 && cpos < 0)
	{// show the all data by assign level bucket 
		for (int j = 0; j < LevelBucket[rpos].size(); j++)
		{
			cout << setw(3) << LevelBucket[rpos][j].getObjLevel() << setw(4) << "| " << setw(25)
				<< LevelBucket[rpos][j].getName() << " | "
				<< LevelBucket[rpos][j].getIDNumber() << endl;
		}
		cout << "--------------------------------------------" << endl;
	}
	else
	{// print out the data which at [rpos][cpos] position
		cout << setw(3) << LevelBucket[rpos][cpos].getObjLevel() << setw(4) << "| " << setw(25)
			<< LevelBucket[rpos][cpos].getName() << " | "
			<< LevelBucket[rpos][cpos].getIDNumber() << endl;
		cout << "--------------------------------------------" << endl;
	}
}
//  reset the name of object
void Objdatabase::resetID(unsigned int i, unsigned int j, string &rID)
{ // [i] is level [j], is the object in [i] level
	LevelBucket[i][j].setIDNumber(rID);
}
//  reset the ID number of object
void Objdatabase::resetName(unsigned int i, unsigned int j, string &rName)
{ // [i] is level [j], is the object in [i] level
	LevelBucket[i][j].setName(rName);
}
//  reset the level of object
void Objdatabase::resetLevel(unsigned int i, unsigned int j, int &rLevel)
{ // [i] is level [j], is the object in [i] level
	LevelBucket[i][j].setObjLevel(rLevel);
}
// this function can save obj data immediately
void Objdatabase::save_Edited_OBJFile(int rpos)
{// sort the row which contain edited data or new data, and then save into file
	sort(LevelBucket[rpos].begin(), LevelBucket[rpos].end(), compareByID);
	saveToOBJFile();
}
//define every object in vector[i]
void Objdatabase::define_pos_ofuserObj_inVec()
{
	for (int i = 1; i < level_bucket_size; ++i)
	{
		for (int j = 0; j < LevelBucket[i].size(); ++j)
			LevelBucket[i][j].setPosition_inVec(j);
	}
}
//return size of bucket in LevelBucket by assigned
int Objdatabase::getSize_ofEachBucket(unsigned int i)const
{
	return LevelBucket[i].size();  
}
// function to return row pos
int Objdatabase::getRowPos()const
{
	return row;
}
// function to return column pos
int Objdatabase::getColPos()const
{
	return column;
}
// add an element at the end of members of vector in bucket
void Objdatabase::pushBack(Obj &newObj) 
{ // if newObj's level is 1, we push it in vector of bucket[1] => LevelBucket[1].push_back()
	LevelBucket[newObj.getObjLevel()].push_back(newObj);
}
// erase object data
void Objdatabase::erase(int rpos, int cpos)
{
	LevelBucket[rpos].erase(LevelBucket[rpos].begin() + cpos);
}
// get data in bucket
Obj Objdatabase::getFile(unsigned int i, unsigned int j)const
{
	return LevelBucket[i][j];
}
// get bucket of each level 
vector< Obj >* Objdatabase::get_LevelBucket()const
{
	return LevelBucket;
}
// load object data
void Objdatabase::loadFromOBJFile()
{
	Obj O_load;	// contain data from file
	fstream  Obj_load("Obj.dat", ios::in | ios::binary); // file which has object's info
	if (!Obj_load)
	{
		Obj_load.close();
		cout << "File Obj.dat could not be opened!" << endl;
		ofstream inFile("Obj.dat", ios::out | ios::binary);
		inFile.close();
		Obj_load.open("Obj.dat", ios::in | ios::binary);
		system("pause");
	}
	Obj_load.seekg(0, ios::beg);
	Obj_load.read(reinterpret_cast<char*>(&O_load), sizeof(Obj));
	while (!Obj_load.eof())
	{
		pushBack(O_load);
		Obj_load.read(reinterpret_cast<char*>(&O_load), sizeof(Obj));
	}
	Obj_load.close();
}
// save object data
void Objdatabase::saveToOBJFile()
{
	fstream O_write("Obj.dat", ios::out | ios::binary);
	if (!O_write)
	{
		O_write.close();
		cout << "File Obj.dat could not be opened." << endl;
		ofstream inFile("Obj.dat", ios::out | ios::binary);
		inFile.close();
		O_write.open("Obj.dat", ios::in | ios::binary);
		system("pause");
	}
	O_write.seekp(0, ios::beg);
	for (unsigned int i = 1; i < level_bucket_size; i++)
	{
		for (unsigned int j = 0; j < LevelBucket[i].size(); j++)
			O_write.write(reinterpret_cast<char*>(&LevelBucket[i][j]), sizeof(Obj));
	}
	O_write.close();
}
// define how to compare object
bool compareByID(const Obj &lhs, const Obj &rhs)
{// from low to high base on the ID of object. stoi:string to int
	return stoi(lhs.getIDNumber()) < stoi(rhs.getIDNumber());
}
#endif // !1 
/* A_A  Let's defraud all consumer by Disposable Games!!!!!! / 咱們來用免洗遊戲坑錢吧!!!!!!  A_A */