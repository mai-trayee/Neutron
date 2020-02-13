#include "InScint/InScintRun.h"

Run::Run() : 
	G4UserRunAction()
{ 
	// add new units for dose
	// 
	// Register accumulable to the accumulable manager
	//G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	//accumulableManager->RegisterAccumulable(fEdep);

	Msg = new MsgOutput(this);
}

Run::~Run()
{
	delete Msg;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::BeginOfRunAction(const G4Run*)
{ 
	std::cout << "begin of run " << GetRootFileName() << std::endl;

	// inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);
	
	OutFile = new TFile(GetRootFileName().c_str(), "RECREATE");

	Data = new TTree("Data", "Information on particle energy");
	Data->Branch("Initial", &InitialEnergy, "fInitial/D");
	Data->Branch("Final",   &FinalEnergy,   "fFinal/D");
	Data->Branch("Absorb",  &AbsorbEnergy,  "fAbsorb/D");
}

void Run::EndOfRunAction(const G4Run* run)
{
	G4int nofEvents = run->GetNumberOfEvent();
	if (nofEvents == 0) 
		return;
	
	Data->Write();
	OutFile->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//energy from end of event
void Run::Fill()
{
	Data->Fill();
}

void Run::SetAbsorbEnergy(G4double energy)
{
	AbsorbEnergy = energy;
}

void Run::SetInitialEnergy(G4double energy)
{
	InitialEnergy = energy;
}

void Run::SetFinalEnergy(G4double energy)
{
	FinalEnergy = energy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::SetRootFileName(G4String fName)
{
	fRootFileName = fName;
}

G4String Run::GetRootFileName()
{
	return fRootFileName;
}

