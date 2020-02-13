//Event
//
#ifndef INSCINT_EVENT_H
#define INSCINT_EVENT_H 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "InScint/InScintRun.h"
#include "InScint/InScintGenerator.h"

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

		void AddAbsorbEnergy(G4double energy);
		void AddFinalEnergy(G4double energy);
		G4LogicalVolume* GetScoringVolume();

	private:
		Run* fRunAction;
		Generator *fGPS;
		G4double fAbsorbEnergy, fFinalEnergy;
		G4LogicalVolume* fScoringVolume;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

		
