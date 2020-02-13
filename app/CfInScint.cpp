//std

#include <cstring>
#include <getopt.h>

//Geant4
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "FTFP_BERT_HP.hh"

//user defined
#include "InScint/InScintVolume.h"
#include "InScint/InScintGenerator.h"
#include "InScint/InScintRun.h"
#include "InScint/InScintEvent.h"
#include "InScint/InScintStep.h"

void Usage(char *argv0);
int main(int argc, char** argv)
{
	const struct option longopts[] = 
	{
		{"macro", 	required_argument,	0, 'm'},
		{"dimension", 	required_argument,	0, 'L'},
		{"cube", 	no_argument,		0, 'q'},
		{"cylinder", 	no_argument,		0, 'c'},
		{"plastic", 	no_argument,		0, 'P'},
		{"bgo", 	no_argument,		0, 'B'},
		{"output", 	required_argument,	0, 'o'},
		{"help", 	no_argument,	 	0, 'h'},
		{0,	0, 	0,	0},
	};

	int index; 
	int iarg = 0;
	opterr = 1;
	
	//Initialize variables
	std::string outName, macFile;
	double size = 5;	//5 cm
	bool cube = true;	//cylinder = false
	bool plastic = true;	//bgo      = false
	unsigned int numEvents = 1000;
	
	while((iarg = getopt_long(argc,argv, "m:L:qcPBo:n:h", longopts, &index)) != -1)
	{
		switch(iarg)
		{
			case 'm':
				macFile.assign(optarg);
				break;
			case 'L':
				size = std::strtod(optarg, NULL);
				break;
			case 'q':
				cube = true;
				break;
			case 'c':
				cube = false;
				break;
			case 'P':
				plastic = true;
				break;
			case 'B':
				plastic = false;
				break;
			case 'o':
				outName.assign(optarg);
				break;
			case 'n':
				numEvents = std::strtol(optarg, NULL, 10);
				break;
			case 'h':
				Usage(argv[1]);
				break;
			default:
				return 1;
				break;
		}
	}

	// construct the default run manager
	G4RunManager* runManager = new G4RunManager;
	
	//get the pointer to the UI manager and set verbosities
	G4UImanager* UI = G4UImanager::GetUIpointer();

	// set mandatory initialization classes
	runManager->SetUserInitialization(new Volume());
	runManager->SetUserInitialization(new FTFP_BERT_HP());	//should be good for neutrons
	//runManager->SetUserInitialization(new Action(Save));

	Generator * generatorAction = new Generator();
	Run*        runAction       = new Run();
	Event*      eventAction     = new Event(runAction, generatorAction);
	Step*       stepAction      = new Step(eventAction);

	runManager->SetUserAction(generatorAction); //generator
	runManager->SetUserAction(runAction);	    //runaction
	runManager->SetUserAction(eventAction);	    //eventaction
	runManager->SetUserAction(new Step(eventAction));	//stepping
	
	// initialize Gu kernel
	runManager->Initialize();

	UI->ApplyCommand("/control/execute " + macFile);

	//outName = "/Output/RootFile " + outName;
	G4String dim, shape, type;

	std::stringstream sSize;
	sSize << size;
	sSize >> dim;

	if (cube)
		shape = "Cube";
	else
		shape = "Cylinder";

	if (plastic)
		type = "Plastic";
	else
		type = "BGO";

	outName += "_" + shape + "_" + type + "_" + dim + ".root";

	UI->ApplyCommand("/Output/RootFile " + outName);
	UI->ApplyCommand("/Volume/Size " + dim + " cm");
	UI->ApplyCommand("/Volume/Shape " + shape);
	UI->ApplyCommand("/Volume/Type " + type);
	UI->ApplyCommand("/Volume/Construct");

	runManager->BeamOn(numEvents);

	//job termination
	delete runManager;
	return 0;
}

void Usage(char* argv0)
{
	std::cout << "Description" << std::endl;
	std::cout << "Usage : " << std::endl;
	std::cout << argv0 << " [OPTIONS]" << std::endl;
	std::cout <<"\n  -i,  --input" << std::endl;
	std::cout << "\t\tInput file, tabulated as Mass\tUu\tnEvt" << std::endl;
	std::cout <<"\n  -o,  --output" << std::endl;
	std::cout << "\t\tOutput file" << std::endl;
	std::cout <<"\n  -t,  --threshold" << std::endl;
	std::cout << "\t\tEvent threshold for signal. As mass dependent threshold, t is the y-intercept" << std::endl;
	std::cout <<"\n  -m,  --massdep" << std::endl;
	std::cout << "\t\tIf specified, a mass dependance is considered for threshold, [thr] = t + m * [Mass]" << std::endl;
	std::cout <<"\n  -h,  --help" << std::endl;
	std::cout << "\t\tPrint this message and exit" << std::endl;
}
