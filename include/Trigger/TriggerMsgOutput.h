/* Messenger
 * handle UI of macro file
 */


#ifndef TRIGGER_MSGOUTPUT_H
#define TRIGGER_MSGOUTPUT_H 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

#include "Trigger/TriggerRun.h"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class Run;

class MsgOutput: public G4UImessenger
{
	public:
		MsgOutput(Run* Act);
		~MsgOutput();
	
		void SetNewValue(G4UIcommand* command, G4String newValues);
		G4String GetCurrentValue(G4UIcommand* command);
	
	private:
		Run* Action;

		G4UIdirectory* IODir;
		G4UIcmdWithAString* RootFile;
};

#endif


