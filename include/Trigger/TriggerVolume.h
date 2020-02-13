//Volume.h
//

#ifndef TRIGGER_VOLUME_h
#define TRIGGER_VOLUME_h 1

#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include "G4PhysicalConstants.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4MultiUnion.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4MaterialPropertiesTable;
class G4OpticalSurface;

/// Detector construction class to define materials and geometry.

class Volume : public G4VUserDetectorConstruction
{
	public:
		Volume();
		virtual ~Volume();
		
		virtual G4VPhysicalVolume* Construct();
		
		G4LogicalVolume* GetPMTVolume() const { return fPMTVolume; }
		G4LogicalVolume* GetAbsVolume() const { return fAbsVolume; }

		void ConstructMaterial();
		G4VPhysicalVolume* ConstructGeometry();
		void UpdateVolume();
		void GetData();
		
	protected:

		std::vector<double> vEnergy, vYield, vRIndex, vAbsL;

		G4LogicalVolume*  fAbsVolume, *fPMTVolume;
		G4Material *matAir, *matHousing, *matScint, *matGlass;
		G4OpticalSurface *optScintAir, *optHouseAir, *optHolderAir;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

