#include "EventAction.hh"
#include <G4AnalysisManager.hh>
#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
#include <G4Types.hh>
#include <G4UserEventAction.hh>

void EventAction::BeginOfEventAction(const G4Event *event) {}
void EventAction::EndOfEventAction(const G4Event *event) {
  if (fEdepTotal <= 0.0)
    return;

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(0, event->GetEventID());
  analysisManager->FillNtupleDColumn(1, fEdepTotal / keV);
  analysisManager->AddNtupleRow();

  fEdepTotal = 0.0;
}
