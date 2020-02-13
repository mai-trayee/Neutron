//Step
//
#ifndef TRIGGER_STEP_h
#define TRIGGER_STEP_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleGun.hh"
#include "G4OpticalPhoton.hh"

#include "Trigger/TriggerEvent.h"
#include "Trigger/TriggerVolume.h"

class Event;

class G4LogicalVolume;
class G4VPhysicalVolume;

/// Stepping action class
/// 

class Step : public G4UserSteppingAction
{
	public:
		Step(Event* eventAction);
		virtual ~Step();
		
		// method from the base class
		virtual void UserSteppingAction(const G4Step*);
		void KillTrack(G4Track *track);

		bool IsDetected(G4double energy);
	
	private:
		Event*  fEventAction;
		G4LogicalVolume *fHitVolume, *fAbsVolume;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
