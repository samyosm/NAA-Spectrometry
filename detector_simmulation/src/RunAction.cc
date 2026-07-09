#include "RunAction.hh"
#include "EventAction.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction(EventAction *eventAction)
    : G4UserRunAction(), fEventAction(eventAction) {

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("csv");

  analysisManager->CreateNtuple("Spectrum", "Simulated NAA Gamma Spectrum");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->CreateNtupleDColumn("EdepKeV");
  analysisManager->FinishNtuple();
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run *) {
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile(
      "/home/samy/NAA-Spectrometry/data/simulated_spectrum.csv");
}

void RunAction::EndOfRunAction(const G4Run *) {
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
