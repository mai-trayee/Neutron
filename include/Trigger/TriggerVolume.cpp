#include "Trigger/TriggerVolume.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Volume::Volume() : 
	G4VUserDetectorConstruction(), 
	fPMTVolume(0), 
	fAbsVolume(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Volume::~Volume()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* Volume::Construct()
{
	ConstructMaterial();
	return ConstructGeometry();
}

G4VPhysicalVolume* Volume::ConstructGeometry()
{
	//world 
	//
	double world_l = 1.5*m;

	G4Box *solWorld = new G4Box ("s_World",
				      world_l, world_l, world_l);

	G4LogicalVolume * logWorld = new G4LogicalVolume(solWorld, 
							 matAir, 
							 "l_World");

	G4VPhysicalVolume *phyWorld = new G4PVPlacement (0,
							 G4ThreeVector(0, 0, 0),
							 logWorld,
							 "p_World",
							 NULL, false, 0);


	//source housing
	//
	double src_od = 0.371*2.54*cm;	//outer diameter
	double src_Bd = 0.325*2.54*cm;	//base  diameter
	double src_id = 0.234*2.54*cm;	//inner diameter
	double src_bd = 0.141*2.54*cm;	//base inner diameter
	double src_td = 0.190*2.54*cm;	//handle diameter

	double src_mh = 1.230*2.54*cm;	//main body height
	double src_ch = 1.020*2.54*cm;	//cavity height
	double src_bh = 0.050*2.54*cm;	//base height
	double src_th = 0.200*2.54*cm;	//handle height

	G4CSGSolid *solHousingShell  = new G4Tubs("s_HousingShell",
						  0.0*cm, src_od / 2., src_mh / 2.,
						  0.0*deg, 360.0*deg);

	G4CSGSolid *solHousingCavity = new G4Tubs("s_HousingCavity",
						  0.0*cm, src_id / 2., src_ch / 2.,
						  0.0*deg, 360.0*deg);

	G4CSGSolid *solHousingBase   = new G4Tubs("s_HousingBase",
						  src_bd / 2., src_Bd / 2., src_bh / 2.,
						  0.0*deg, 360.0*deg);

	G4CSGSolid *solHousingHandle = new G4Tubs("s_HousingHandle",
						  0.0*cm, src_td / 2., src_th / 2.,
						  0.0*deg, 360.0*deg);

	double diff_hh = fabs(src_mh-src_ch)/2.0 - 0.080*2.54*cm;

	G4RotationMatrix rotm = G4RotationMatrix();	//no rotation
	G4ThreeVector transCavity (0.0*cm, 0.0*cm, diff_hh);

	//creating shell from difference
	G4SubtractionSolid* solHousingMain = new G4SubtractionSolid("s_HousingMain", 
								    solHousingShell,
								    solHousingCavity,
								    &rotm,
								    transCavity);

	G4ThreeVector transHousingMain  (0.0*cm, 0.0*cm, src_bh + src_mh/2.0);
	G4ThreeVector transHousingBase  (0.0*cm, 0.0*cm, src_bh/2.0);
	G4ThreeVector transHousingHandle(0.0*cm, 0.0*cm, src_bh + src_mh + src_th/2.0);

	G4Transform3D moveHousingMain   = G4Transform3D(rotm, transHousingMain);
	G4Transform3D moveHousingBase   = G4Transform3D(rotm, transHousingBase);
	G4Transform3D moveHousingHandle = G4Transform3D(rotm, transHousingHandle);

	//union of pieces
	//
	G4MultiUnion* solHousing = new G4MultiUnion("s_Housing");

	solHousing->AddNode(*solHousingMain,	moveHousingMain);
	solHousing->AddNode(*solHousingBase,	moveHousingBase);
	solHousing->AddNode(*solHousingHandle,	moveHousingHandle);
	solHousing->Voxelize();

	G4LogicalVolume * logHousing = new G4LogicalVolume(solHousing, 
							   matHousing,
							   "l_Housing");

	G4VPhysicalVolume *phyHousing = new G4PVPlacement (0,
							   G4ThreeVector(0, 0, 0),
							   logHousing,
							   "p_Housing",
							   logWorld, false, 0);

	//air cavity inside housing
	//
	//G4LogicalVolume * logCavity = new G4LogicalVolume(solHousingCavity, 
	//						 matAir,
	//						 "l_Cavity");

	//G4VPhysicalVolume *phyCavity = new G4PVPlacement (0,
	//						 G4ThreeVector(0.0*cm, 0.0*cm, src_bh + src_mh/2.0 + diff_hh),
	//						 logCavity,
	//						 "p_Cavity",
	//						 logWorld, false, 0);

	//scintillator
	//
	double scint_l    = 3.0*2.54*cm;
	double scint_hole = 0.4*2.54*cm;

	G4CSGSolid *solTube = new G4Tubs("s_Tube",
					 0.0*cm, scint_l / 4.0, scint_l / 2.0,		//1.5" x 3.0"
					 0.0*deg, 360.0*deg);

	G4CSGSolid *solHole = new G4Tubs("s_Hole",
					 0.0*cm, scint_hole / 2.0, scint_l / 4.0,
					 0.0*deg, 360.0*deg);

	G4ThreeVector transHole (0.0*cm, 0.0*cm, scint_l / 4.0);

	G4SubtractionSolid* solScint = new G4SubtractionSolid("s_Scint", 
							      solTube,
							      solHole,
							      &rotm,
							      transHole);

	G4LogicalVolume * logScint = new G4LogicalVolume(solScint, 
							 matScint,
							 "l_Scint");

	G4VPhysicalVolume *phyScint = new G4PVPlacement (0,
							 G4ThreeVector(0, 0, 0),
							 logScint,
							 "p_Scint",
							 logWorld, false, 0);

	//detectors and abosrbers
	//
	double pmt_l = 1.0*2.54*cm;

	//small cylinder holding place for PMT
	//
	G4CSGSolid *solPMT = new G4Tubs("s_PMT",
					0.0*cm, scint_l / 4.0, pmt_l / 2.0,	//1.5" x 3.0"
					0.0*deg, 360.0*deg);

	G4LogicalVolume * logPMT = new G4LogicalVolume(solPMT, 
						       matGlass,
						       "l_PMT");

	G4VPhysicalVolume *phyPMT = new G4PVPlacement (0,
						       G4ThreeVector(0, 0, -(scint_l + pmt_l)/2.0),
						       logPMT,
						       "p_PMT",
						       logWorld, false, 0);

	//absorber placed opposite PMT, to keep source in place
	//
	G4CSGSolid *solAbs = new G4Tubs("s_Absorber",
					 0.0*cm, scint_l / 4.0, pmt_l / 2.0,	//1.5" x 3.0"
					 0.0*deg, 360.0*deg);

	G4LogicalVolume * logAbs = new G4LogicalVolume(solAbs, 
						       matGlass,
						       "l_Absorber");

	G4VPhysicalVolume *phyAbs = new G4PVPlacement (0,
						       G4ThreeVector(0, 0,  (scint_l + pmt_l)/2.0),
						       logAbs,
						       "p_Absorber",
						       logWorld, false, 0);

	//small layer of air (0.5mm thick), uniform for simplicity
	//
	//G4Tubs *solAir = new G4Tubs("s_Air",
	//			    src_od / 2., src_od / 2.0 + 0.5*mm, (scint_l + pmt_l) / 2.,
	//			    0.0*deg, 360.0*deg);

	//G4LogicalVolume * logAir = new G4LogicalVolume(solAir,
	//					       matAir,
	//					       "l_Air");

	//G4VPhysicalVolume *phyAir = new G4PVPlacement (0,
	//					       G4ThreeVector(0, 0, 0),
	//					       logAir,
	//					       "p_Air",
	//					       NULL, false, 0);

	//holder, just a simple tube, with 0.1mm gap for air
	//
	double holder_d = scint_l / 2.0;
	G4Tubs *solHolder = new G4Tubs("s_Holder",
				       holder_d / 2. + 0.1*mm, holder_d / 2. + .1 * (mm + cm), scint_l / 2. + pmt_l,
				       0.0*deg, 360.0*deg);

	G4LogicalVolume * logHolder = new G4LogicalVolume(solHolder,
							  matHousing,
							  "l_Holder");

	G4VPhysicalVolume *phyHolder = new G4PVPlacement (0,
							 G4ThreeVector(0, 0, 0),
							 logHolder,
							 "p_Holder",
							 logWorld, false, 0);

	//optical surface between scintillator and air
	//
	//G4LogicalSkinSurface* ScintSurf = new G4LogicalSkinSurface("sf_MylarLayer",
	//							   logScint, 
	//							   //phyWorld, phyScint
	//							   optScintAir);
	G4LogicalBorderSurface* sfScintAir = new G4LogicalBorderSurface("sf_ScintAir",
									phyScint,
									phyWorld,
									optScintAir);

	//housing is opaque to photons
	//
	//G4LogicalSkinSurface* HouseSurf = new G4LogicalSkinSurface("sf_Housing",
	//							     logHousing, 
	//							     //phyWorld, phyScint
	//							     optHouseSurf);
	G4LogicalBorderSurface* sfHouseAir = new G4LogicalBorderSurface("sf_HouseAir",
									phyWorld,
									phyHousing,
									optHouseAir);


	G4LogicalBorderSurface* sfHolderAir = new G4LogicalBorderSurface("sf_HolderAir",
									phyWorld,
									phyHolder,
									optHolderAir);

	fPMTVolume = logPMT;
	fAbsVolume = logAbs;

	return phyWorld;
}

void Volume::GetData()
{
	double transm = 0.90;	//% transmittance
	double absL = 3.0*2.54*cm / log(1./transm);
	std::cout << "absorbance " << absL << std::endl;

	std::ifstream ej_input("data/ej200_response.dat");
	std::stringstream ssE;
	std::string line;
	//std::vector<double> vEnergy, vYield, vRIndex;
	double wl, yield;
	while(std::getline(ej_input, line))
	{
		ssE.clear();
		ssE.str("");

		if (line[0] == '#')
			continue;

		ssE << line;
		ssE >> wl >> yield;

		vEnergy.push_back((c_light*h_Planck) / (wl * nm));
		vYield.push_back(yield);
		vRIndex.push_back(1.58);
		vAbsL.push_back(absL);
	}
	std::reverse(vEnergy.begin(), vEnergy.end());
	std::reverse(vYield.begin(), vYield.end());
}

void Volume::ConstructMaterial()
{
	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();

	if (vEnergy.size() == 0)	//load data for ej
		GetData();
	
	//air for world
	//
	matAir = nist->FindOrBuildMaterial("G4_AIR");
	G4MaterialPropertiesTable *matAir_MT = new G4MaterialPropertiesTable();
	double air_pp  [2] = {vEnergy.front(), vEnergy.back()}; 
	double air_ridx[2] = {1.0003, 1.0003};
	matAir_MT->AddProperty("RINDEX", air_pp, air_ridx, 2);
	matAir->SetMaterialPropertiesTable(matAir_MT);

	//304 stainless steel for sorce housing
	//
	matHousing = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

	//plastic for scintillator of type EJ200
	//
	matScint = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	matScint->GetIonisation()->SetBirksConstant(0.16*mm/MeV);

	//loading properties of EJ200
	G4MaterialPropertiesTable *matScint_MT = new G4MaterialPropertiesTable();

	double *ej_Energy = &vEnergy[0];
	double *ej_Yield  = &vYield[0];
	double *ej_RIndex = &vRIndex[0];
	double *ej_AbsL   = &vAbsL[0];

	double *ej_ElecEnergy = new double[vEnergy.size()];
	double *ej_ElecYield  = new double[vEnergy.size()];

	double ejM = (log10(4.0e5) - log10(1.0e3)) / (log10(40*MeV) - log10(0.1*MeV));
	double ejQ = log10(4.0e5) - ejM * log10(40*MeV);
	double lEnStep = ( log10(40*MeV) - log10(0.1*MeV) ) / vEnergy.size();
	for (unsigned int i = 0; i < vEnergy.size(); ++i)
	{
		double lEn = log10(0.1*MeV) + i*lEnStep;

		ej_ElecEnergy[i] = pow(10.0, lEn);		//energy for electron yield
		ej_ElecYield[i]  = pow(10.0, ejM*lEn + ejQ);	//yield at electron passage
	}

	matScint_MT->AddProperty("RINDEX"	, ej_Energy, ej_RIndex, vEnergy.size());
	matScint_MT->AddProperty("ABSLENGTH"    , ej_Energy, ej_AbsL,   vEnergy.size());
	matScint_MT->AddProperty("FASTCOMPONENT", ej_Energy, ej_Yield,  vEnergy.size());
	matScint_MT->AddProperty("SLOWCOMPONENT", ej_Energy, ej_Yield,  vEnergy.size());
	matScint_MT->AddProperty("ELECTRONSCINTILLATIONYIELD", ej_ElecEnergy, ej_ElecYield, vEnergy.size());

	matScint_MT->AddConstProperty("SCINTILLATIONYIELD", 10574.0/MeV);
	matScint_MT->AddConstProperty("FASTTIMECONSTANT", 2.1*ns);
	matScint_MT->AddConstProperty("FASTSCINTILLATIONRISETIME", 0.9*ns);
	matScint_MT->AddConstProperty("SLOWTIMECONSTANT", 2.1*ns);
	matScint_MT->AddConstProperty("SLOWSCINTILLATIONRISETIME", 0.9*ns);
	matScint_MT->AddConstProperty("YIELDRATIO",1.0);
	matScint_MT->AddConstProperty("RESOLUTIONSCALE", 1.0);
	matScint->SetMaterialPropertiesTable(matScint_MT);


	//glass for PMT
	matGlass = nist->FindOrBuildMaterial("G4_GLASS_PLATE");

	//mylar lining for optical properties of surface
	G4MaterialPropertiesTable *mylar_MT = new G4MaterialPropertiesTable();
	G4MaterialPropertiesTable *steel_MT = new G4MaterialPropertiesTable();
	double energy_pp    [2] = {vEnergy.front(), vEnergy.back()}; 
	double mylar_reflect[2] = {0.90, 0.90};
	double steel_reflect[2] = {0.10, 0.10};

	mylar_MT->AddProperty("REFLECTIVITY", energy_pp, mylar_reflect, 2);
	steel_MT->AddProperty("REFLECTIVITY", energy_pp, steel_reflect, 2);

	optScintAir = new G4OpticalSurface("ScintSurface");
	optScintAir->SetType(dielectric_dielectric);
	optScintAir->SetModel(unified);
	optScintAir->SetFinish(polished);
	optScintAir->SetSigmaAlpha(0.1);	//boh?
	//optScintAir->SetMaterialPropertiesTable(mylar_MT);

	optHouseAir = new G4OpticalSurface("HousingSurface");
	optHouseAir->SetType(dielectric_metal);
	//optHouseAir->SetModel(unified);
	optHouseAir->SetFinish(ground);
	optHouseAir->SetSigmaAlpha(0.1);	//boh?
	optHouseAir->SetMaterialPropertiesTable(steel_MT);

	optHolderAir = new G4OpticalSurface("HousingSurface");
	optHolderAir->SetType(dielectric_metal);
	//optHolderAir->SetModel(unified);
	optHolderAir->SetFinish(ground);
	optHolderAir->SetSigmaAlpha(0.1);	//boh?
	optHolderAir->SetMaterialPropertiesTable(mylar_MT);
}
