/* Messenger
 * handle UI of macro file
 */


#ifndef INSCINT_MSGVOLUME_H
#define INSCINT_MSGVOLUME_H 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "InScint/InScintVolume.h"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class Volume;

class MsgVolume: public G4UImessenger
{
	public:
		MsgVolume(Volume* Scint);
		~MsgVolume();
	
		void SetNewValue(G4UIcommand* command, G4String newValues);
		G4String GetCurrentValue(G4UIcommand* command);
	
	private:
		Volume* Scintillator;

		G4UIdirectory* IODir;
		G4UIcmdWithADoubleAndUnit* Size;
		G4UIcmdWithAString* Shape;
		G4UIcmdWithAString* Type;
		G4UIcmdWithoutParameter* Build;
};

#endif


