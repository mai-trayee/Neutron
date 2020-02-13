#include "InScint/InScintStep.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Step::Step(Event* eventAction) : 
	G4UserSteppingAction(),
	fEventAction(eventAction),
	fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Step::~Step()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Step::UserSteppingAction(const G4Step* step)
{
	fScoringVolume = fEventAction->GetScoringVolume();	//should keep it updated

	// get volume of the current step
	G4LogicalVolume* volume = 
		step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
			
	// check if we are in scoring volume
	if (volume != fScoringVolume)
	{
		//const Generator* fGenerator = 
		//	static_cast<const Generator*>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
		return;
	}

	// collect energy deposited in this step
	G4double edepStep = step->GetTotalEnergyDeposit();
	std::cout << "energy lost " << edepStep << std::endl;
	fEventAction->AddAbsorbEnergy(edepStep);	
}

void Step::CleanScoringVolume()
{
	fScoringVolume = 0;
}
