#include "RunAction.hh"
#include "EventAction.hh"
#include "G4AnalysisManager.hh"
#include <filesystem>

RunAction::RunAction(EventAction *eventAction, fs::path dataDir)
    : G4UserRunAction(), fEventAction(eventAction), dataDir(dataDir) {

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");

  analysisManager->SetNtupleMerging(true);
  analysisManager->SetNtupleRowWise(false, false);
  // analysisManager->SetBasketSize(32 * 1024);

  analysisManager->CreateNtuple("Spectrum", "Simulated NAA Gamma Spectrum");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->CreateNtupleDColumn("EdepKeV");
  analysisManager->FinishNtuple();
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run *) {
  auto analysisManager = G4AnalysisManager::Instance();

  fs::path out(dataDir / "srm_1633c_HPGe_detector_simulation.root");
  std::string path = fs::absolute(out);
  analysisManager->OpenFile(path);
}

void RunAction::EndOfRunAction(const G4Run *) {
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
