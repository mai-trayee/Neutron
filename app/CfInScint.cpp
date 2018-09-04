//std

#include <string>

//Geant4
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "FTFP_BERT_HP.hh"

//user defined
#include "CfInScint/Volume.h"
#include "CfInScint/Generator.h"
#include "CfInScint/Run.h"
#include "CfInScint/Event.h"
#include "CfInScint/Step.h"

int main(int argc, char** argv)
{
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

	G4String command = "/control/execute ";
	G4String Batch = argv[1];
	G4cout << "Running macro " << Batch << std::endl;
	UI->ApplyCommand(command+Batch);

	//job termination
	delete runManager;
	return 0;
}
