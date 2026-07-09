#pragma once

#include "EventAction.hh"
#include "G4UserRunAction.hh"
#include <filesystem>

namespace fs = std::filesystem;

class RunAction : public G4UserRunAction {
public:
  RunAction(EventAction *eventAction, fs::path dataDir);
  virtual ~RunAction();
  virtual void BeginOfRunAction(const G4Run *) override;
  virtual void EndOfRunAction(const G4Run *) override;

private:
  EventAction *fEventAction;
  fs::path dataDir;
};
