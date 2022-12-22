
// SIM //
#include "SphereParameterisation.hh"

// GEANT4 //
#include "G4SystemOfUnits.hh"

#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"

#include "G4VisAttributes.hh"


SphereParameterisation::SphereParameterisation(G4double boxHalfSideLength, G4double sphereDiameter)
{
    _extents = G4ThreeVector(boxHalfSideLength, boxHalfSideLength, boxHalfSideLength);
    _sphereDiameter = sphereDiameter;
    _numInRow = (boxHalfSideLength*2)/_sphereDiameter;
    _totalSpheres = _numInRow*_numInRow*_numInRow;
}


SphereParameterisation::~SphereParameterisation()
{
}


void SphereParameterisation::ComputeTransformation(
        const G4int copy_number, G4VPhysicalVolume* physical_volume) const
{
  G4int xnum = 0;
  G4int ynum = 0;
  G4int znum = 0;

  //Increment ynum for every time a row in x is completed
  ynum = (G4int)(copy_number/_numInRow);

  //Increment znum for every time a whole layer in x and y are completed
  znum = (G4int)(copy_number/(_numInRow*_numInRow));

  //Subtract from xnum to make it in the range 0-_numInRow
  xnum = copy_number - (ynum*_numInRow);

  //Subtract from ynum to make it in the range 0-yinc
  ynum = ynum - (znum*_numInRow);

  //Break if too many replicas
  if(copy_number > (_numInRow*_numInRow*_numInRow)) { G4cout << "Too many replicas entered into SphericalParameterisation for box size, Voxel #:" << copy_number+1 << G4endl; return;}

  G4double xpos = xnum*_sphereDiameter-_extents.x()+_sphereDiameter/2;
  G4double ypos = ynum*_sphereDiameter-_extents.y()+_sphereDiameter/2;
  G4double zpos = znum*_sphereDiameter-_extents.z()+_sphereDiameter/2;

  G4ThreeVector position(xpos, ypos, zpos);
  physical_volume->SetTranslation(position);
  physical_volume->SetRotation(0);

  //std::cout << "volume number: " << copy_number << " x,y,z: " << xpos << " " << ypos << " " << zpos << std::endl;
}


void SphereParameterisation::ComputeDimensions( G4Orb& orb
                                              , const int copy_number
                                              , const G4VPhysicalVolume*) const
{
    orb.SetRadius(_sphereDiameter/2);
}


G4LogicalVolume* SphereParameterisation::Placement(
        G4LogicalVolume* mother_logical
      , G4Material* sphere_material
      , G4Material* surrounds_material
      , G4Colour colour)
{
    // Setup the volume container the replicated spheres.
    /*auto container_solid = new G4Box( "container_solid"
                                    , _extents.x() 
                                    , _extents.y()
                                    , _extents.z());

    auto container_logical = new G4LogicalVolume( container_solid
                                                , surrounds_material
                                                , "container_logical"
                                                , 0, 0, 0);

    //container_logical->SetVisAttributes(G4VisAttributes::Invisible);

    auto container_physical = new G4PVPlacement( 0
                                               , G4ThreeVector()
                                               , container_logical
                                               , "container_physical"
                                               , mother_logical
                                               , false, 0);*/

    // Replicate the spheres in the container volume.
    auto sphere_solid = new G4Orb("body_solid", 1*nm); //this radius is changed in parameterisation

    auto sphere_logical = new G4LogicalVolume( sphere_solid
                                             , sphere_material
                                             , "body_logical"
                                             , 0, 0, 0);

    sphere_logical->SetVisAttributes(colour);

    // calculate Nspheres
    G4int nSpheres = (_extents.x()*2)/_sphereDiameter;
    nSpheres = nSpheres*nSpheres*nSpheres; //cube it

    new G4PVParameterised( "replicated_spheres"
                         , sphere_logical
                         , mother_logical
                         , kUndefined
                         , nSpheres
                         , this
                         , false);

    return sphere_logical;
}

