#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "FTFP_BERT.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;
int main(int argc, char *argv[]) {
  auto runManager = G4RunManagerFactory::CreateRunManager();

  std::string dataDir = "./data/";

  if (argc >= 2) {
    fs::path dirPath(argv[1]);
    dataDir = fs::absolute(dirPath);
    std::cout << "Data directory: " << dataDir << std::endl;
  }

  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new FTFP_BERT);

  auto actionInitialization =
      new ActionInitialization(dataDir, "srm_1633c_gamma_yields.csv");
  runManager->SetUserInitialization(actionInitialization);

  runManager->Initialize();

  G4UImanager *UI = G4UImanager::GetUIpointer();
  UI->ApplyCommand("/run/verbose 0");
  UI->ApplyCommand("/event/verbose 0");
  UI->ApplyCommand("/tracking/verbose 0");
  UI->ApplyCommand("/run/printProgress 1000000");

  long long numberOfEvent = 2995;

  if (argc >= 3) {
    numberOfEvent = std::stoll(argv[2]);
  }

  runManager->BeamOn(numberOfEvent);

  delete runManager;

  return 0;
}
