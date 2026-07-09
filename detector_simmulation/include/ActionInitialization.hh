#pragma once

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization {
public:
  ActionInitialization();
  virtual ~ActionInitialization();

  virtual void Build() const override;
  virtual void BuildForMaster() const override;
};
