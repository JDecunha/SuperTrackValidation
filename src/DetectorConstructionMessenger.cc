//MicroTrackGenerator
#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"
//Geant4
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction* detector_construction) : pDetectorConstruction(detector_construction)
{
    // Directories in which commands are organised.
    geometry_directory = new G4UIdirectory("/geometry/");
    geometry_directory->SetGuidance("Commands relating to geometry.");

    // Set the voxel side length
    geometry_voxel_sidelength = new G4UIcmdWithADoubleAndUnit("/geometry/voxelSideLength", this);
    geometry_voxel_sidelength->SetGuidance("Set the side length of the cubic voxel the tracks are confined to.");
    geometry_voxel_sidelength->SetParameterName("sidelength", false);
    geometry_voxel_sidelength->SetUnitCategory("Length");
    geometry_voxel_sidelength->AvailableForStates(G4State_PreInit);

    // Set the voxel side length
    geometry_sphere_diameter = new G4UIcmdWithADoubleAndUnit("/geometry/sphereDiameter", this);
    geometry_sphere_diameter->SetGuidance("Set the diameter of the target spheres.");
    geometry_sphere_diameter->SetParameterName("sphereDiam", false);
    geometry_sphere_diameter->SetUnitCategory("Length");
    geometry_sphere_diameter->AvailableForStates(G4State_PreInit);
}


DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
    delete geometry_directory;
    delete geometry_voxel_sidelength;
    delete geometry_sphere_diameter;
}

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String value)
{
    if (command == geometry_voxel_sidelength)
    {
      pDetectorConstruction->SetSideLength(geometry_voxel_sidelength->GetNewDoubleValue(value.c_str()));
    }

    if (command == geometry_sphere_diameter)
    {
      pDetectorConstruction->SetSphereDiameter(geometry_sphere_diameter->GetNewDoubleValue(value.c_str()));
    }
}

