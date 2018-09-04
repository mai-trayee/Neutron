#include "CfInScint/Event.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Event::Event(Run* runAction, Generator *generatorAction) : 
	G4UserEventAction(),
       	fRunAction(runAction),
	fGPS(generatorAction),
       	fAbsorbEnergy(0.),
       	fFinalEnergy(0.)
{
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Event::~Event()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Event::BeginOfEventAction(const G4Event*)
{
	G4cout << "begin of event" << std::endl;

	fAbsorbEnergy = 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Event::EndOfEventAction(const G4Event*)
{   
	// accumulate statistics in run action
	// energy deposited in each step
	//G4double Energy = fGenerator->GetParticleEnergy();
	fRunAction->SetInitialEnergy(fGPS->GetEnergyKin());
	fRunAction->SetFinalEnergy(fGPS->GetEnergyKin() - fAbsorbEnergy);
	fRunAction->SetAbsorbEnergy(fAbsorbEnergy);
	fRunAction->Fill();
}

//adding energy deposited in scoring volume during the event
void Event::AddAbsorbEnergy(G4double energy)	//called by Step
{
	fAbsorbEnergy += energy;
}

//saving energy of particle out of scoring volume
void Event::AddFinalEnergy(G4double energy)	//called by Step
{
	fFinalEnergy = energy;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
