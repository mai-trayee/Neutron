#include "Trigger/TriggerRun.h"

Run::Run() : 
	G4UserRunAction(),
	genMT(new TRandom3(0)),
	nEvent(0)
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
	// inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);
	G4Random::setTheSeed(genMT->GetSeed());
	
	OutFile = new TFile(GetRootFileName().c_str(), "RECREATE");

	fWavelength = new double[10000];

	Data = new TTree("Data", "Information on particle energy");
	//Data->Branch("HitEnergy",  &fHitEnergy,  "fHitEnergy/D");
	Data->Branch("PE", &nEvent,     "nEvent/i");
	Data->Branch("wl", fWavelength, "fWavelength[nEvent]/D");
	//Data->Branch("InitialE", &fInitialE, "fInitialE/D");
	//Data->Branch("Particle", &fParticle, "fParticle/I");

	std::ifstream ej_input("data/ET9902_QE.dat");
	std::stringstream ssE;
	std::string line;
	double wl, yield;
	while(std::getline(ej_input, line))
	{
		ssE.clear();
		ssE.str("");

		if (line[0] == '#')
			continue;

		ssE << line;
		ssE >> wl >> yield;
		vPMT_wl.push_back((c_light*h_Planck) / (wl * nm));
		vPMT_eff.push_back(yield);
	}
	std::reverse(vPMT_wl.begin(), vPMT_wl.end());
	std::reverse(vPMT_eff.begin(), vPMT_eff.end());
}

void Run::EndOfRunAction(const G4Run* run)
{
	G4int nofEvents = run->GetNumberOfEvent();
	if (nofEvents) 
		Data->Write();

	OutFile->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//energy from end of event
void Run::Fill()
{
	Data->Fill();
	nEvent = 0;
}

void Run::RecordHit(G4double energy)
{
	if (energy == 0.0)
		fWavelength[nEvent] = -1.0;
	else
		fWavelength[nEvent] = (c_light * h_Planck) / (energy * eV); 

	++nEvent;
}

bool Run::IsDetected(G4double energy)
{
	for (unsigned int i = 1; i < vPMT_wl.size(); ++i)
		if (energy < vPMT_wl.at(i))
			return (genMT->Rndm() < (vPMT_eff.at(i-1) + vPMT_eff.at(i)) / 2.0);
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
