/* Run
 * RunAction class
 * handling whole run of events, from beginning to end
 *
 * Begin: Open root file, initialise histogram
 * End  : Calculate depoted energy, save file
 */

//
#ifndef INSCINT_ACTION_H
#define INSCINT_ACTION_H 1

#include <string>

//GEANT4
#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4String.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "InScint/InScintGenerator.h"
#include "InScint/InScintVolume.h"
#include "InScint/InScintMsgOutput.h"

//ROOT
#include "TFile.h"
#include "TTree.h"

class G4Run;
class MsgOutput;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class Run : public G4UserRunAction
{
	public:
		Run();
		virtual ~Run();

		// virtual G4Run* GenerateRun();
		virtual void BeginOfRunAction(const G4Run*);
		virtual void EndOfRunAction(const G4Run*);

		void SetRootFileName(G4String fName);
		G4String GetRootFileName();

		void Fill();
		void SetInitialEnergy (G4double energy); 
		void SetFinalEnergy   (G4double energy); 
		void SetAbsorbEnergy  (G4double energy); 

	private:
		TFile *OutFile;
		TTree *Data;
		double InitialEnergy, FinalEnergy, AbsorbEnergy;

		MsgOutput *Msg;
		G4String fRootFileName;
};

#endif
