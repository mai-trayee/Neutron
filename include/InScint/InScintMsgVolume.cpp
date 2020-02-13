#include "InScint/InScintMsgVolume.h"

MsgVolume::MsgVolume(Volume* Scint) :
	Scintillator(Scint)	//run actio
{ 
	IODir = new G4UIdirectory("/Volume/");
	IODir->SetGuidance("Commands to select I/O options");
	
	Size = new G4UIcmdWithADoubleAndUnit("/Volume/Size", this);
	Size->SetGuidance("Set the scintillator size");
	Size->SetParameterName("ScintillatorSize", true, true);
	Size->SetDefaultUnit("cm");
	Size->SetUnitCandidates("mm cm m");
	Size->SetDefaultValue(10.*cm);
	Size->SetRange("ScintillatorSize > 0.");

	Shape = new G4UIcmdWithAString("/Volume/Shape", this);
	Shape->SetGuidance("Set the scintillator shape");
	Shape->SetParameterName("ScintillatorShape", true);
	Shape->SetCandidates("Cube Cylinder");
	Shape->SetDefaultValue("Cube");

	Type = new G4UIcmdWithAString("/Volume/Type", this);
	Type->SetGuidance("Set the scintillator material");
	Type->SetParameterName("ScintillatorType", true);
	Type->SetCandidates("Plastic BGO");
	Type->SetDefaultValue("Plastic");

	Build = new G4UIcmdWithoutParameter("/Volume/Construct", this);
	Build->SetGuidance("Update the volume settings");
}

MsgVolume::~MsgVolume()
{
	delete IODir;
	delete Size;
	delete Shape;
	delete Type;
	delete Build;
}

void MsgVolume::SetNewValue(G4UIcommand* Command, G4String newValue)
{
	if (Command == Size)
		Scintillator->SetScintillatorSize(Size->GetNewDoubleValue(newValue));
	if (Command == Shape)
	{
		if (newValue == "Cube")
			Scintillator->SetScintillatorShape(Volume::Cube);
		else if (newValue == "Cylinder")
			Scintillator->SetScintillatorShape(Volume::Cylinder);
	}
	if (Command == Type)
	{
		if (newValue == "Plastic")
			Scintillator->SetScintillatorType(Volume::Plastic);
		else if (newValue == "BGO")
			Scintillator->SetScintillatorType(Volume::BGO);
	}
	if (Command == Build)
		Scintillator->UpdateVolume();
}

G4String MsgVolume::GetCurrentValue(G4UIcommand* Command)
{
	if (Command == Size)
		return Size->ConvertToString(Scintillator->GetScintillatorSize(), "cm");
	else if (Command == Shape)
	{
		if (Scintillator->GetScintillatorShape() == Volume::Cube)
			return G4String("Cube");
		else if (Scintillator->GetScintillatorShape() == Volume::Cylinder)
			return G4String("Cylinder");
	}
	else if (Command == Type)
	{
		if (Scintillator->GetScintillatorType() == Volume::Plastic)
			return G4String("Plastic");
		else if (Scintillator->GetScintillatorType() == Volume::BGO)
			return G4String("BGO");
	}
}
