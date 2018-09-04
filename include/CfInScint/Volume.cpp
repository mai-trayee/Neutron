#include "CfInScint/Volume.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Volume::Volume() : 
	G4VUserDetectorConstruction(), 
	fScoringVolume(0)
{
	fSize = 10*cm;
	fShape = Cube;
	Msg = new MsgVolume(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Volume::~Volume()
{
	delete Msg;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* Volume::Construct()
{
	std::cout << "Constructing Volume" << std::endl;
	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();
	
	//world
	//3m x 3m x 3m cube
	G4Material* matAir = nist->FindOrBuildMaterial("G4_AIR");
	double world_l = 1.5*m;

	G4Box *solWorld = 
		new G4Box ("World",
			   world_l, 
			   world_l, 
			   world_l);
	G4LogicalVolume * logWorld = 
		new G4LogicalVolume(solWorld, 
				    matAir, 
				    "World");
	G4VPhysicalVolume *phyWorld = 
		new G4PVPlacement (0,
				    G4ThreeVector(0, 0, 0),
				    logWorld,
				    "World",
				    NULL,
				    false,
				    0);

	//scintillator/detector
	//cylinder or cube, try different shapes and sizes
	//10cm x 10cm x 10cm cube
	G4Material* matScint = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	double scint_l = GetScintillatorSize();

	G4CSGSolid *solScint;
	switch (GetScintillatorShape())
	{
		case Cube:
			G4cout << "Creating a cubic scintillator of size " << scint_l;
			G4cout << " cm, volume " << pow(scint_l, 3)*1e-3 << " cm³" << std::endl;
			solScint = new G4Box("Scintillator",
					     scint_l, scint_l, scint_l);
			break;
		case Cylinder:
			G4cout << "Creating a cylindric scintillator of size " << scint_l;
			G4cout << " cm, volume " << CLHEP::pi * pow(scint_l, 3) / 4.0 << " cm³" << std::endl;
			solScint = new G4Tubs("Scintillator",
					      0.0*cm, scint_l/2, scint_l,
					      0.0*deg, 360.0*deg);
			break;
		default:
			solScint = NULL;
			break;
	}

	G4LogicalVolume * logScint = 
		new G4LogicalVolume(solScint, 
				    matScint,
				    "Scintillator");
	G4VPhysicalVolume *phyScint = 
		new G4PVPlacement (0,
				    G4ThreeVector(0, 0, 0),
				    logScint,
				    "Scintillator",
				    logWorld,
				    false,
				    0);

	//neutron source
	//Cf252 point like source
	//
	//G4Material* matCf252 = nist->FindOrBuildMaterial("G4_Cf");
	//G4Material* matCf252 = 
	//	new G4Material("Cf252", 98, 252.0*g/mole, 1000000.0);
	//double radius = 1.*mm;
	//double height = 1.*mm;

	//G4Tubs *solNeutS =
	//	new G4Tubs("NeutronSource", 
	//		   0.*cm, 
	//		   radius, 
	//		   height, 
	//		   0.*deg, 
	//		   360.*deg);
	//G4LogicalVolume * logNeutS = 
	//	new G4LogicalVolume(solNeutS, 
	//			    matCf252,
	//			    "NeutronSource");
	//G4VPhysicalVolume *phyNeutS = 
	//	new G4PVPlacement (0,
	//			    G4ThreeVector(0, 0, 0),
	//			    logNeutS,
	//			    "NeutronSource",
	//			    logWorld,
	//			    false,
	//			    0);

	//to get deposited energy in the scintillator
	fScoringVolume = logScint;
	
	return phyWorld;
}

void Volume::UpdateVolume()
{
	G4RunManager::GetRunManager()->DefineWorldVolume(Construct(), true);
}

void Volume::SetScintillatorSize(G4double Size)
{
	fSize = Size;
}

double Volume::GetScintillatorSize()
{
	return fSize;
}

void Volume::SetScintillatorShape(Shape shape)
{
	fShape = shape;
}

Volume::Shape Volume::GetScintillatorShape()
{
	return fShape;
}

