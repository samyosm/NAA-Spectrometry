#pragma once

#include "EventAction.hh"
#include "G4UserRunAction.hh"

class RunAction : public G4UserRunAction {
public:
  RunAction(EventAction *eventAction);
  virtual ~RunAction();
  virtual void BeginOfRunAction(const G4Run *) override;
  virtual void EndOfRunAction(const G4Run *) override;

private:
  EventAction *fEventAction;
};
