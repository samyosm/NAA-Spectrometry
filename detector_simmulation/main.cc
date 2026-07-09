#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "FTFP_BERT.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <regex>
#include <vector>

namespace fs = std::filesystem;

void MergePerThreadCsvOutputs(const fs::path &dataDir) {
  static const std::regex perThreadCsvPattern(R"(.*[tT][0-9].*\.csv)");

  std::vector<fs::path> csvFiles;
  for (const auto &entry : fs::directory_iterator(dataDir)) {
    if (entry.is_regular_file() &&
        std::regex_match(entry.path().filename().string(),
                         perThreadCsvPattern)) {
      csvFiles.push_back(entry.path());
    }
  }
  std::sort(csvFiles.begin(), csvFiles.end());

  std::ofstream out(dataDir / "detector_simulation.csv");
  bool headerWritten = false;
  for (const auto &file : csvFiles) {
    {
      std::ifstream in(file);
      std::string line;
      bool firstLine = true;
      while (std::getline(in, line)) {
        if (firstLine) {
          firstLine = false;
          if (headerWritten) {
            continue;
          }
          headerWritten = true;
        }
        out << line << '\n';
      }
    }
    fs::remove(file);
  }
}

int main() {
  auto runManager = G4RunManagerFactory::CreateRunManager();

  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new FTFP_BERT);
  runManager->SetUserInitialization(new ActionInitialization);

  runManager->Initialize();

  G4UImanager *UI = G4UImanager::GetUIpointer();
  UI->ApplyCommand("/run/verbose 0");
  UI->ApplyCommand("/event/verbose 0");
  UI->ApplyCommand("/tracking/verbose 0");

  int numberOfEvent = 200000;
  runManager->BeamOn(numberOfEvent);

  delete runManager;

  MergePerThreadCsvOutputs("../data/");

  return 0;
}
