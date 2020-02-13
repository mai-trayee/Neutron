//Step
//
#ifndef INSCINT_STEP_H
#define INSCINT_STEP_H 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleGun.hh"

#include "InScint/InScintEvent.h"
#include "InScint/InScintVolume.h"

class Event;

class G4LogicalVolume;

/// Stepping action class
/// 

class Step : public G4UserSteppingAction
{
	public:
		Step(Event* eventAction);
		virtual ~Step();
		
		// method from the base class
		virtual void UserSteppingAction(const G4Step*);
		void CleanScoringVolume();
	
	private:
		Event*  fEventAction;
		G4LogicalVolume* fScoringVolume;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
