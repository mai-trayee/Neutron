#include "InScint/InScintMsgOutput.h"

MsgOutput::MsgOutput(Run* Act) :
	Action(Act)	//run actio
{ 
	IODir = new G4UIdirectory("/Output/");
	IODir->SetGuidance("Commands to select I/O options");
	
	RootFile = new G4UIcmdWithAString("/Output/RootFile",this);
	RootFile->SetGuidance("Set the root file name");
	//RootFile->SetGuidance("Enter the name of the output ROOT file");
	RootFile->SetParameterName("RootFileName", false);
	//RootFile->SetDefaultValue("Californium.root");
}

MsgOutput::~MsgOutput()
{
	delete IODir;
	delete RootFile;
}

void MsgOutput::SetNewValue(G4UIcommand* Command, G4String newValue)
{
	if (Command == RootFile)
	{
		Action->SetRootFileName(newValue);
		G4cout << "Output ROOT file set to " << newValue << G4endl;
	}
}

G4String MsgOutput::GetCurrentValue(G4UIcommand* Command)
{
	if (Command == RootFile)
		return Action->GetRootFileName();
}
