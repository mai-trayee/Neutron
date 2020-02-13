#include "Trigger/TriggerEvent.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Event::Event(Run* runAction, Generator *generatorAction) : 
	G4UserEventAction(),
       	fRunAction(runAction),
	fGPS(generatorAction),
       	fHitEnergy(0.)
{
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Event::~Event()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Event::BeginOfEventAction(const G4Event* anEvent)
{
	//std::cout << "Event #" << anEvent->GetEventID() << std::endl;

	//const Volume* Scintillator = 	//begin of each run get scoring volume which can be updated
		//static_cast<const Volume*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Event::EndOfEventAction(const G4Event*)
{   
	fRunAction->Fill();
}

//adding energy deposited in tree
void Event::RecordHit(G4double energy)	//called by Step
{
	fRunAction->RecordHit(energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* Event::GetHitVolume()
{
	return fHitVolume;
}

G4LogicalVolume* Event::GetAbsVolume()
{
	return fAbsVolume;
}

bool Event::IsDetected(G4double energy)
{
	return fRunAction->IsDetected(energy);
}
