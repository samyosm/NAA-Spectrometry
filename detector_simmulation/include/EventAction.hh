#pragma once
#include "G4UserEventAction.hh"
#include <G4Types.hh>

class EventAction : public G4UserEventAction {
public:
  EventAction() = default;
  virtual ~EventAction() = default;

  virtual void BeginOfEventAction(const G4Event *event) override;
  virtual void EndOfEventAction(const G4Event *event) override;

  void AddEdep(G4double edep) { fEdepTotal += edep; }
  G4double GetEdep() const { return fEdepTotal; }

private:
  G4double fEdepTotal = 0.0;
};
