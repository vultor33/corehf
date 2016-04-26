#include <iostream>
#include <vector>

#include "ControlMopac.h"
#include "ReadQuantumOutput.h"
#include "Coordstructs.h"
#include "ReadQuantumOptions.h"

using namespace std;

int main()
{
	ReadQuantumOptions read_;

	ControlMopac controlMop;

	vector<string> options(5);
	options[0] = "mopac";
	options[1] = "h2-mol";
	options[2] = "";
	options[3] = "";
	options[4] = "";

	vector<CoordXYZ> atoms(2);
	atoms[0].atomlabel = "H";
	atoms[0].x = 0.0e0;
	atoms[0].y = 0.0e0;
	atoms[0].z = 0.0e0;
	atoms[1].atomlabel = "H";
	atoms[1].x = 0.74e0;
	atoms[1].y = 0.0e0;
	atoms[1].z = 0.0e0;

	controlMop.optimize(atoms, options);


	cout << "end" << endl;
	cin.get();
	return 0;
}


/* EXEMPLO GAMESS
vector<CoordXYZ> atoms(3);
atoms[0].atomlabel = "Na";
atoms[0].x = 0;
atoms[0].y = 0;
atoms[0].z = 0;
atoms[1].atomlabel = "Li";
atoms[1].x = 1;
atoms[1].y = 0;
atoms[1].z = 0;
atoms[2].atomlabel = "Li";
atoms[2].x = 2;
atoms[2].y = 0;
atoms[2].z = 0;

vector<string> options(20);
options[0] = "gamess";
options[1] = "build-na-1-li-2";
options[2] = " $CONTRL SCFTYP=ROHF RUNTYP=OPTIMIZE EXETYP=RUN MPLEVL=2 MAXIT=200 MULT=2";
options[3] = " ISPHER=1 COORD=UNIQUE NOSYM=1 UNITS=ANGS PP=READ $END";
options[4] = " $GUESS GUESS=HUCKEL $END";
options[5] = " $SYSTEM MWORDS=40 MEMDDI=20  $END";
options[6] = " $SCF DIRSCF=.FALSE. $END";
options[7] = " $DATA";
options[8] = " titulo";
options[9] = "C1";
options[10] = "EndOfHeader";
options[11] = "na-base.txt";
options[12] = "li-base.txt";
options[13] = "li-base.txt";
options[14] = "EndOfBasis";
options[15] = "ActivateEcp";
options[16] = "na-ecp.txt";
options[17] = "li-ecp.txt";
options[18] = "li-ecp.txt";
options[19] = "EndOfEcp";
















*/