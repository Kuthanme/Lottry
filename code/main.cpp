#include"Capsule_Toys.h"// Capsule_Toys class definition
#include "Debug.h"		// Debug class definition
#include"resource.h"

void displayMainMenu();

int main()
{	// define the icon of program(Lottry.exe)
	HICON   hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	HWND     hwnd = GetConsoleWindow();
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

	srand((unsigned)time(NULL));
	// enumeration constants represent main menu options
	enum MenuOption { PLAY = 1, DEBUGG, EXIT };

	cout << "\n<< Let's get the Jackpot! >>( v 1.458  )\n";

	CL_database cldatabase;	 // chance of level information database
	Objdatabase objDatabase(cldatabase.getSize()); // object information database
	CapsuleToys StartGame(objDatabase, cldatabase);
	Debug debug(objDatabase, cldatabase);
	
	bool Exit = false;	// user has not chosen to exit
						// loop while user has not chosen option to exit system
	while (!Exit)
	{
		displayMainMenu();// show main menu and get user selection
		Choose inputInteger;// get user selection by class of Choose
		int mainMenuSelection = inputInteger.excute(1, 3);
		switch (mainMenuSelection)	// decide how to proceed based on user's menu selection
		{// user chose to perform one of three transaction types
		case PLAY:  // start to play
			StartGame.excute();
			break;
		case DEBUGG:// debug(add the new object)
			debug.execute();	
			break;
		case EXIT:  // user chose to terminate session
			cout << "\nGoodbye jerk!";
			Sleep(887);  // delay 0.887s
			Exit = true; // this session should end
			break;
		default: // user did not enter an integer from 1-3
			cout << "\nIncorrect choice! You are an idiot! " << endl;
			break;
		} // end switch
	} // end while
}
// display the main menu and return an input selection
void displayMainMenu()
{
	cout << "\n< 1 > - Play Capsule Game ^_^" << endl;
	cout << "< 2 > - Debug mode" << endl;
	cout << "< 3 > - End \n" << endl;
	cout << "Enter your choice: (1~3): ";
} // end function displayMainMenu

//check version: ^b*[^:b#/]+.*$
/* A_A  Let's defraud all consumer by Disposable Games!!!!!! / 咱們來用免洗遊戲坑錢吧!!!!!!  A_A */ //Ver 1.458     