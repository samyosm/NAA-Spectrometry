#pragma once

#include "G4VUserDetectorConstruction.hh"
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4Types.hh>

class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();
  virtual G4VPhysicalVolume *Construct();
  virtual void ConstructSDandField();

private:
};
