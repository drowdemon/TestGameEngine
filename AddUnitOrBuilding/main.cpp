#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main()
{
	while(true)
	{
		cout << "Unit (1) or Building (2) or Research (3)?" << endl;
		int whichone=0;
		while(whichone!=1 && whichone!=2 && whichone!=3)
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
			float args[28]={0};
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
			cout << "attackarea   ";
			cin >> args[20];
			cout << "chanceHitting   ";
			cin >> args[21];
			cout << "trainingcof  ";
			cin >> args[22];
			cout << "foodontraining  ";
			cin >> args[23];
			cout << "woodontraining  ";
			cin >> args[24];
			cout << "goldontraining  ";
			cin >> args[25];
			cout << "stoneontraining  ";
			cin >> args[26];
			cout << "timeontraining  ";
			cin >> args[27];
			string name;
			cout << "name  ";
			cin >> name;
			for(int i=0; i<28; i++)
			{
				outf << args[i];
				if(i<28-1)
					outf << ",";
			}
			outf << " " << name;
		}
		else if(whichone==2)
		{
			ofstream outf("/home/marc/Coding/TestGameEngine/TestGameEnginev13/GameEngine2/building.specs", ios::app);
			if(!outf)
			{
				cout << "Unable to open building.specs" << endl;
				//system("pause");
				exit(-235);
			}
			//outf << "\n";
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
        else if(whichone==3) //research
        {
            ofstream outf("/home/marc/Coding/TestGameEngine/TestGameEnginev13/GameEngine2/research.specs", ios::app);
			if(!outf)
			{
				cout << "Unable to open research.specs" << endl;
				//system("pause");
				exit(-235);
			}
			//outf << "\n";
			float args[21]={0};
			cout << "changewhat  ";
			cin >> args[0];
            cout << "food  ";
			cin >> args[1];
            cout << "wood  ";
			cin >> args[2];
            cout << "gold  ";
			cin >> args[3];
            cout << "stone  ";
			cin >> args[4];
            cout << "time  ";
			cin >> args[5];
            cout << "researched where  ";
			cin >> args[6];
            cout << "armor  ";
			cin >> args[7];
            cout << "attack  ";
			cin >> args[8];
            cout << "range  ";
			cin >> args[9];
            cout << "accuracy  ";
			cin >> args[10];
            cout << "food speed  ";
			cin >> args[11];
            cout << "wood speed  ";
			cin >> args[12];
            cout << "gold speed  ";
			cin >> args[13];
            cout << "stone speed  ";
			cin >> args[14];
            cout << "building attack  ";
			cin >> args[15];
            cout << "build speed  ";
			cin >> args[16];
            cout << "max garrison  ";
			cin >> args[17];
            cout << "LOS  ";
	    cin >> args[18];
	    cout << "X   ";
	    cin >> args[19];
	    cout << "Y   ";
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
			outf << " " << name << "\n";
        }
		cout << endl << "Done!" << endl << "To quit input 'q', else input 'r' to input another unit/building/research" << endl;
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
