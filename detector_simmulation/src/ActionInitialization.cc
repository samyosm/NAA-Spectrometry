#include "ActionInitialization.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
  SetUserAction(new RunAction(nullptr));
}

void ActionInitialization::Build() const {
  PrimaryGeneratorAction *primaryGeneratorAction = new PrimaryGeneratorAction(
      "/home/samy/NAA-Spectrometry/data/srm_1633c_gamma_yields.csv");
  SetUserAction(primaryGeneratorAction);

  EventAction *eventAction = new EventAction();
  SetUserAction(eventAction);

  RunAction *runAction = new RunAction(eventAction);
  SetUserAction(runAction);

  SteppingAction *steppingAction = new SteppingAction(eventAction);
  SetUserAction(steppingAction);
}
