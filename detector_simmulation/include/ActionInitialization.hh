#pragma once

#include "G4VUserActionInitialization.hh"
#include <filesystem>

namespace fs = std::filesystem;

class ActionInitialization : public G4VUserActionInitialization {
public:
  ActionInitialization(fs::path dataDir, std::string input_filename);
  virtual ~ActionInitialization();

  virtual void Build() const override;
  virtual void BuildForMaster() const override;

private:
  fs::path dataDir;
  std::string input_filename;
};
