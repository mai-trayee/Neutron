#include "CfInScint/Action.h"
#include "CfInScint/Generator.h"
#include "CfInScint/Run.h"
#include "CfInScint/Event.h"
#include "CfInScint/Step.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Action::Action() : 
	G4VUserActionInitialization()
{}

Action::Action(std::string Save) : 
	G4VUserActionInitialization(),
	File(Save)
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Action::~Action()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Action::BuildForMaster() const
{
  Run* runAction = new Run(File);
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Action::Build() const
{
	SetUserAction(new Generator());
	
	Run* runAction = new Run(File);
	SetUserAction(runAction);
	
	Event* eventAction = new Event(runAction);
	SetUserAction(eventAction);
	
	SetUserAction(new Step(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
