// SteppingAction.cc
#include "SteppingAction.hh"
#include "EventAction.hh"

#include <G4Step.hh>
#include <G4String.hh>
#include <G4VPhysicalVolume.hh>

SteppingAction::SteppingAction(EventAction *eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *step) {
  G4VPhysicalVolume *volume = step->GetPreStepPoint()->GetPhysicalVolume();
  if (!volume)
    return;

  auto edep = step->GetTotalEnergyDeposit();
  if (edep <= 0.0)
    return;

  if (volume->GetName() == "GeCrystal") {
    fEventAction->AddEdep(edep);
  }
}
