#include "DetectorConstruction.hh"

#include <G4Box.hh>
#include <G4Colour.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4String.hh>
#include <G4SystemOfUnits.hh>
#include <G4ThreeVector.hh>
#include <G4Tubs.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VisAttributes.hh>

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  G4NistManager *nist = G4NistManager::Instance();

  G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material *germanium = nist->FindOrBuildMaterial("G4_Ge");
  G4Material *aluminum = nist->FindOrBuildMaterial("G4_Al");
  G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");

  // World
  const G4double worldSize = 1.0 * m;
  G4Box *solidWorld =
      new G4Box("World", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
  G4LogicalVolume *logicWorld =
      new G4LogicalVolume(solidWorld, worldMat, "World");
  logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VPhysicalVolume *physWorld = new G4PVPlacement(
      nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);

  // Coaxial HPGe crystal inside an aluminum end cap, separated by a vacuum
  // gap, standing off along +z from the point source at the origin.
  const G4double standoffDistance = 10.0 * cm;

  const G4double crystalRadius = 3.0 * cm;
  const G4double crystalHalfLength = 3.0 * cm;

  const G4double vacuumGap = 3.0 * mm;
  const G4double capThickness = 1.0 * mm;

  const G4double gapRadius = crystalRadius + vacuumGap;
  const G4double gapHalfLength = crystalHalfLength + vacuumGap;

  const G4double capRadius = gapRadius + capThickness;
  const G4double capHalfLength = gapHalfLength + capThickness;

  // Place the end cap so its front face sits at standoffDistance from the
  // source, with the crystal centered inside it.
  const G4double capCenterZ = standoffDistance + capHalfLength;

  // Aluminum end cap (mother volume for the vacuum gap + crystal)
  G4Tubs *solidCap =
      new G4Tubs("EndCap", 0.0, capRadius, capHalfLength, 0.0, 360.0 * deg);
  G4LogicalVolume *logicCap = new G4LogicalVolume(solidCap, aluminum, "EndCap");

  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, capCenterZ), logicCap,
                    "EndCap", logicWorld, false, 0, true);

  // Vacuum gap between the end cap and the crystal
  G4Tubs *solidVacuum =
      new G4Tubs("Vacuum", 0.0, gapRadius, gapHalfLength, 0.0, 360.0 * deg);
  G4LogicalVolume *logicVacuum =
      new G4LogicalVolume(solidVacuum, vacuum, "Vacuum");
  logicVacuum->SetVisAttributes(G4VisAttributes::GetInvisible());

  new G4PVPlacement(nullptr, G4ThreeVector(), logicVacuum, "Vacuum", logicCap,
                    false, 0, true);

  // Germanium crystal
  G4Tubs *solidCrystal = new G4Tubs("GeCrystal", 0.0, crystalRadius,
                                    crystalHalfLength, 0.0, 360.0 * deg);
  G4LogicalVolume *logicCrystal =
      new G4LogicalVolume(solidCrystal, germanium, "GeCrystal");

  G4VisAttributes *crystalVis = new G4VisAttributes(G4Colour(0.0, 0.6, 1.0));
  crystalVis->SetForceSolid(true);
  logicCrystal->SetVisAttributes(crystalVis);

  new G4PVPlacement(nullptr, G4ThreeVector(), logicCrystal, "GeCrystal",
                    logicVacuum, false, 0, true);

  return physWorld;
}

void DetectorConstruction::ConstructSDandField() {}
