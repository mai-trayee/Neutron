/* Run
 * RunAction class
 * handling whole run of events, from beginning to end
 *
 * Begin: Open root file, initialise histogram
 * End  : Calculate depoted energy, save file
 */

//
#ifndef TRIGGER_ACTION_H
#define TRIGGER_ACTION_H 1

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

#include "Trigger/TriggerGenerator.h"
#include "Trigger/TriggerVolume.h"
#include "Trigger/TriggerMsgOutput.h"

//ROOT
#include "TFile.h"
#include "TTree.h"
#include "TRandom3.h"

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
		void RecordHit (G4double energy); 
		void SetInitialE  (G4double energy); 
		void SetParticle  (G4int idCode); 

		bool IsDetected(G4double energy);

	private:
		TFile *OutFile;
		TTree *Data;
		double *fWavelength;
		unsigned int nEvent;

		MsgOutput *Msg;
		G4String fRootFileName;

		TRandom3 *genMT;

		std::vector<double> vPMT_wl, vPMT_eff;
};

#endif
