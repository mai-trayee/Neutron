//Event
//
#ifndef TRIGGER_EVENT_H
#define TRIGGER_EVENT_H 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "Trigger/TriggerRun.h"
#include "Trigger/TriggerGenerator.h"

class G4LogicalVolume;
class Run;
class Generator;

/// Event action class
///

class Event : public G4UserEventAction
{
	public:
		Event(Run* runAction, Generator* generatorAction);
		virtual ~Event();

		virtual void BeginOfEventAction(const G4Event* event);
		virtual void EndOfEventAction(const G4Event* event);

		void RecordHit(G4double energy);
		G4LogicalVolume* GetHitVolume();
		G4LogicalVolume* GetAbsVolume();

		bool IsDetected(G4double energy);

	private:
		Run* fRunAction;
		Generator *fGPS;
		G4double fHitEnergy;
		G4LogicalVolume *fHitVolume, *fAbsVolume;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

		
