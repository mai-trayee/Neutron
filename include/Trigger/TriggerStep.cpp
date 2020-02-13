#include "Trigger/TriggerStep.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Step::Step(Event* eventAction) : 
	G4UserSteppingAction(),
	fEventAction(eventAction),
	fHitVolume(0),
	fAbsVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Step::~Step()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Step::UserSteppingAction(const G4Step* step)
{
	//fHitVolume = fEventAction->GetHitVolume();
	//fAbsVolume = fEventAction->GetAbsVolume();

	// get volume of the end of step
	G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	G4StepPoint* preStepPoint = step->GetPreStepPoint();
	G4Track* aTrack = step->GetTrack();
			
	G4VPhysicalVolume *nextVolume = aTrack->GetNextVolume();
	G4double gammaTot = aTrack->GetTotalEnergy();

	if (aTrack->GetParticleDefinition() == G4OpticalPhoton::Definition())
	{
		if (nextVolume && nextVolume->GetName() == "p_PMT")
		{
			if (IsDetected(gammaTot))
				fEventAction->RecordHit(aTrack->GetKineticEnergy());
			else
			KillTrack(aTrack);
		}
		else if (nextVolume && nextVolume->GetName() == "p_Absorber")
			KillTrack(aTrack);
	}
}

void Step::KillTrack(G4Track *track)
{
	track->SetTrackStatus(fStopAndKill);
}

bool Step::IsDetected(G4double energy)
{
	return fEventAction->IsDetected(energy);
}
