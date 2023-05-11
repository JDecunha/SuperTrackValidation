//MicroTrackGenerator
#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"
#include "SphereParameterisation.hh"
#include "SensitiveDetector.hh"
//Geant4
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SDManager.hh"
#include "G4PVPlacement.hh"
#include "G4Color.hh"
#include "Types.hh"

DetectorConstruction::DetectorConstruction():G4VUserDetectorConstruction()
{
  sideLength = 0;
  sideLengthInitialized = false;
  sphereDiameter = 0;
  sphereDiameterInitialized = false;

  pMessenger = new DetectorConstructionMessenger(this);
}  

DetectorConstruction::~DetectorConstruction()
{
  delete pMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  if (sideLengthInitialized == false)
  {
      G4ExceptionDescription description;
      description << "Voxel side length was not defined. Set in macro before initialization with /geometry/voxelSideLength" << G4endl;
      G4Exception("DetectorConstruction::Construct()", "Voxel side length NDEF.", FatalException, description, "");
  }

  if (sphereDiameterInitialized == false)
  {
      G4ExceptionDescription description;
      description << "Sphere diameter was not defined. Set in macro before initialization with /geometry/sphereDiameter" << G4endl;
      G4Exception("DetectorConstruction::Construct()", "Sphere diameter NDEF.", FatalException, description, "");
  }

  /*//test the spherical parameterisation
  G4double _boxHalfLength = 1.5*mm;
  G4double _sphereDiameter = 0.15*mm;
  G4double _numInRow = (_boxHalfLength*2)/_sphereDiameter;

  G4int numIterations = (_boxHalfLength*2)/_sphereDiameter;
  numIterations = numIterations*numIterations*numIterations; //cube it

  for (int i = 0; i < numIterations; i++)
  {
      G4int xnum = 0;
      G4int ynum = 0;
      G4int znum = 0;

      //Increment ynum for every time a row in x is completed
      ynum = (G4int)(i/_numInRow);

      //Increment znum for every time a whole layer in x and y are completed
      znum = (G4int)(i/(_numInRow*_numInRow));

      //Subtract from xnum to make it in the range 0-_numInRow
      xnum = i - (ynum*_numInRow);

      //Subtract from ynum to make it in the range 0-yinc
      ynum = ynum - (znum*_numInRow);

      //This is an error statment, but also a break statement. Once x,y, and z have been incremented over. break.
      //if(copyNo > (xinc*yinc*zinc)) { G4cout << "Too many voxels entered into RectangularParameterisation, Voxel #:" << copyNo+1 << G4endl; return;}

      G4double xpos = xnum*_sphereDiameter-_boxHalfLength+_sphereDiameter/2;;
      G4double ypos = ynum*_sphereDiameter-_boxHalfLength+_sphereDiameter/2;;
      G4double zpos = znum*_sphereDiameter-_boxHalfLength+_sphereDiameter/2;;

      std::cout << "Copy number: " << i << " X,Y,Z " << xpos << ", " << ypos << ", " << zpos << std::endl;
  }*/

  return ConstructDetector();
}

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
  //Define water material
  G4NistManager * man = G4NistManager::Instance();
  G4Material * H2O = man->FindOrBuildMaterial("G4_WATER");

  // Create a voxel within which to confine the tracks
  G4VSolid* solidWorld = new G4Box("World", sideLength/2,sideLength/2,sideLength/2);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,  //its solid
                                    H2O,  //its material
                                    "World");    //its name

  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,      //no rotation
                                  G4ThreeVector(),  //at (0,0,0), a requirement of the mother volume
                                  "World",    //its name
                                  logicWorld,    //its logical volume
                                  0,      //its mother  volume
                                  false,      //no boolean operation
                                  0);      //copy number
  
  return physiWorld;
}

void DetectorConstruction::ConstructSDandField()
{

}
