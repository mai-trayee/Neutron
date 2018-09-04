#include "CfInScint/Generator.h"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Generator::Generator() : 
	G4VUserPrimaryGeneratorAction(),
       	fGPS(0), 
	fSource(0)
{
	fGPS = new G4GeneralParticleSource();
	
	// default particle kinematic
	//G4ParticleTable* PartTab = G4ParticleTable::GetParticleTable();
	//G4ParticleDefinition* Part_n = PartTab->FindParticle("neutron");
	//fGPS->SetParticleDefinition(Part_n);
	//fGPS->SetParticleMomentumDirection(G4RandomDirection());
	//fGPS->SetParticleEnergy(2.*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Generator::~Generator()
{
	delete fGPS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Generator::GeneratePrimaries(G4Event* anEvent)
{
	//this function is called at the begining of each event
	//
	
	// In order to avoid dependence of PrimaryGeneratorAction
	// on DetectorConstruction class we get Envelope volume
	// from G4LogicalVolumeStore.
	
	/*
	G4double Radius = 0;
	G4double Height = 0;
	
	if (!fSource)
	{
		G4LogicalVolume* SourceLV = G4LogicalVolumeStore::GetInstance()->GetVolume("NeutronSource");
		if (SourceLV) fSource = dynamic_cast<G4Tubs*>(SourceLV->GetSolid());
	}
	
	if (fSource)
       	{
		Radius = fSource->GetOuterRadius();
		Height = fSource->GetZHalfLength()*2.;
	}  
	else
	{
		G4ExceptionDescription msg;
		msg << "Envelope volume of box shape not found.\n"; 
		msg << "Perhaps you have changed geometry.\n";
		msg << "The gun will be place at the center.";
		G4Exception("Generator::GeneratePrimaries()", "Code0002",JustWarning,msg);
	}
	
	double angle = G4UniformRand()*CLHEP::twopi;
	G4double x0 = cos(angle)*Radius;
	G4double y0 = sin(angle)*Radius;
	G4double z0 = Height * (G4UniformRand()-0.5);
	
	fGPS->SetParticlePosition(G4ThreeVector(x0,y0,z0));
	
	*/

	fGPS->GeneratePrimaryVertex(anEvent);

	fMomentum = anEvent->GetPrimaryVertex()->GetPrimary()->GetMomentum();
	fMass     = anEvent->GetPrimaryVertex()->GetPrimary()->GetMass();
	fEnergy   = anEvent->GetPrimaryVertex()->GetPrimary()->GetTotalEnergy();
	fEnergyK  = fEnergy - fMass;
}

G4double Generator::GetMomentum() const
{
	return fMomentum.dot(fMomentum);
}

G4double Generator::GetEnergy() const
{
	return fEnergy;
}

G4double Generator::GetEnergyKin() const
{
	return fEnergyK;
}

G4double Generator::GetMass() const
{
	return fMass;
}
