#include "ActionInitialization.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include <filesystem>

namespace fs = std::filesystem;

ActionInitialization::ActionInitialization(fs::path dataDir,
                                           std::string input_filename)
    : G4VUserActionInitialization(), dataDir(dataDir),
      input_filename(input_filename) {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
  SetUserAction(new RunAction(nullptr, dataDir));
}

void ActionInitialization::Build() const {
  std::string path = dataDir / input_filename;

  PrimaryGeneratorAction *primaryGeneratorAction =
      new PrimaryGeneratorAction(path);
  SetUserAction(primaryGeneratorAction);

  EventAction *eventAction = new EventAction();
  SetUserAction(eventAction);

  RunAction *runAction = new RunAction(eventAction, dataDir);
  SetUserAction(runAction);

  SteppingAction *steppingAction = new SteppingAction(eventAction);
  SetUserAction(steppingAction);
}
