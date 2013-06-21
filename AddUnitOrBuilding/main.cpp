#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main()
{
	while(true)
	{
		cout << "Unit (1) or Building (2)?" << endl;
		int whichone=0;
		while(whichone!=1 && whichone!=2)
			cin >> whichone;
		int dowhat=0;
		cout << "Add (1), Modify (n/a), or Delete (n/a)" << endl;
		while(dowhat!=1)
			cin >> dowhat;
		if(whichone==1)
		{
			ofstream outf("/home/marc/Coding/TestGameEngine/TestGameEnginev13/GameEngine2/unit.specs", ios::app);
			if(!outf)
			{
				cout << "Unable to open unit.specs" << endl;
				//system("pause");
				exit(-235);
			}
			outf << "\n";
			float args[26]={0};
			cout << "health  ";
			cin >> args[0];
			cout << "meleeattack  ";
			cin >> args[1];
			cout << "rangedattack  ";
			cin >> args[2];
			cout << "armor  ";
			cin >> args[3];
			cout << "buildingattack  ";
			cin >> args[4];
			cout << "LOS  ";
			cin >> args[5];
			cout << "speed  ";
			cin >> args[6];
			cout << "attackspeed  ";
			cin >> args[7];
			cout << "foodconsumed  ";
			cin >> args[8];
			cout << "sleepneeded  ";
			cin >> args[9];
			cout << "attackrange  ";
			cin >> args[10];
			cout << "id  ";
			cin >> args[11];
			cout << "camouflage  ";
			cin >> args[12];
			cout << "height  ";
			cin >> args[13];
			cout << "width  ";
			cin >> args[14];
			cout << "buildspeed  ";
			cin >> args[15];
			cout << "maxhold  ";
			cin >> args[16];
			cout << "whatisit  ";
			cin >> args[17];
			cout << "mingarrison  ";
			cin >> args[18];
			cout << "maxgarrison  ";
			cin >> args[19];
			cout << "trainingcof  ";
			cin >> args[20];
			cout << "foodontraining  ";
			cin >> args[21];
			cout << "woodontraining  ";
			cin >> args[22];
			cout << "goldontraining  ";
			cin >> args[23];
			cout << "stoneontraining  ";
			cin >> args[24];
			cout << "timeontraining  ";
			cin >> args[25];
			string name;
			cout << "name  ";
			cin >> name;
			for(int i=0; i<26; i++)
			{
				outf << args[i];
				if(i<26-1)
					outf << ",";
			}
			outf << " " << name;
		}
		else
		{
			ofstream outf("/home/marc/Coding/TestGameEngine/TestGameEnginev13/GameEngine2/building.specs", ios::app);
			if(!outf)
			{
				cout << "Unable to open building.specs" << endl;
				//system("pause");
				exit(-235);
			}
			outf << "\n";
			float args[21]={0};
			cout << "health  ";
			cin >> args[0];
			cout << "armor  ";
			cin >> args[1];
			cout << "garrison  ";
			cin >> args[2];
			cout << "healhealth  ";
			cin >> args[3];
			cout << "garrisonedarmorup  ";
			cin >> args[4];
			cout << "chanceofbeinghit  ";
			cin >> args[5];
			cout << "garrisonedrangedattack  ";
			cin >> args[6];
			cout << "garrisonedmeleeattack  ";
			cin >> args[7];
			cout << "range  ";
			cin >> args[8];
			cout << "id  ";
			cin >> args[9];
			cout << "maxhold  ";
			cin >> args[10];
			cout << "width  ";
			cin >> args[11];
			cout << "height  ";
			cin >> args[12];
			cout << "radiustodistribute  ";
			cin >> args[13];
			cout << "numtrainsimultaneously  ";
			cin >> args[14];
			cout << "landorwater  ";
			cin >> args[15];
			cout << "foodtobuild  ";
			cin >> args[16];
			cout << "woodtobuild  ";
			cin >> args[17];
			cout << "goldtobuild  ";
			cin >> args[18];
			cout << "stonetobuild  ";
			cin >> args[19];
			cout << "timetobuild  ";
			cin >> args[20];
			string name;
			cout << "name  ";
			cin >> name;
			for(int i=0; i<21; i++)
			{
				outf << args[i];
				if(i<21-1)
					outf << ",";
			}
			outf << " " << name;
		}
		cout << endl << "Done!" << endl << "To quit input 'q', else input 'r' to input another unit/building" << endl;
		char input='t';
		while(input!='q' && input!='r')
		{
			cin >> input;
		}
		if(input=='q')
			break;
	}
	return 0;
}
