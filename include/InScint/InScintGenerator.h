//Generator
//
#ifndef INSCINT_GENERATOR_H
#define INSCINT_GENERATOR_H 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;
class G4Tubs;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 2 MeV neutron, randomly distribued 

class Generator : public G4VUserPrimaryGeneratorAction
{
	public:
		Generator();    
		virtual ~Generator();
		
		// method from the base class
		virtual void GeneratePrimaries(G4Event*);         
		
		// method to access particle gun
		const G4GeneralParticleSource* GetGeneralParticleSource() const { return fGPS; }

		G4double GetMomentum()  const;
		G4double GetMass()      const;
		G4double GetEnergy()    const;
		G4double GetEnergyKin() const;
	
	private:
		G4GeneralParticleSource*  fGPS; // pointer a to G4 gun class
		G4Tubs* fSource;

		G4ThreeVector fMomentum;
		G4double fMass, fEnergy, fEnergyK;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
