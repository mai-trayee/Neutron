//Volume.h
//

#ifndef INSCINT_VOLUME_H
#define INSCINT_VOLUME_H 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "InScint/InScintMsgVolume.h"

class G4VPhysicalVolume;
class G4LogicalVolume;
class MsgVolume;

/// Detector construction class to define materials and geometry.

class Volume : public G4VUserDetectorConstruction
{
	public:
		enum Shape
		{
			Cube,
			Cylinder,
		};

		enum Type
		{
			Plastic,
			BGO,
		};

		Volume();
		virtual ~Volume();
		
		virtual G4VPhysicalVolume* Construct();
		
		G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

		void SetScintillatorSize(G4double Size);
		G4double GetScintillatorSize();
		void SetScintillatorShape(Shape shape);
		Shape GetScintillatorShape();
		void SetScintillatorType(Type type);
		Type GetScintillatorType();

		void UpdateVolume();
		
	protected:
		MsgVolume *Msg;

		G4LogicalVolume*  fScoringVolume;

		G4double fSize;
		Shape fShape;
		Type fType;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

