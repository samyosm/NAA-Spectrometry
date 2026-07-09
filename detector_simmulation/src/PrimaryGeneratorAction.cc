#include "PrimaryGeneratorAction.hh"

#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4ThreeVector.hh>
#include <Randomize.hh>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {

std::vector<std::string> SplitCSVLine(const std::string &line) {
  std::vector<std::string> fields;
  std::stringstream ss(line);
  std::string field;
  while (std::getline(ss, field, ',')) {
    fields.push_back(field);
  }
  return fields;
}

} // namespace

PrimaryGeneratorAction::PrimaryGeneratorAction(const G4String &csvPath)
    : G4VUserPrimaryGeneratorAction() {
  fParticleGun = new G4ParticleGun(1);
  G4ParticleDefinition *gamma =
      G4ParticleTable::GetParticleTable()->FindParticle("gamma");
  fParticleGun->SetParticleDefinition(gamma);

  LoadSpectrum(csvPath);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

void PrimaryGeneratorAction::LoadSpectrum(const G4String &csvPath) {
  std::ifstream file(csvPath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open spectrum file: " +
                             std::string(csvPath));
  }

  std::string headerLine;
  std::getline(file, headerLine);
  std::vector<std::string> headers = SplitCSVLine(headerLine);

  int energyCol = -1, yieldCol = -1;
  for (size_t i = 0; i < headers.size(); ++i) {
    if (headers[i] == "energy")
      energyCol = static_cast<int>(i);
    if (headers[i] == "n_gamma")
      yieldCol = static_cast<int>(i);
  }

  if (energyCol < 0 || yieldCol < 0) {
    throw std::runtime_error(
        "Could not find 'energy' and/or 'n_gamma' columns in header");
  }

  std::vector<G4double> weights;
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty())
      continue;
    std::vector<std::string> fields = SplitCSVLine(line);

    int maxCol = std::max(energyCol, yieldCol);
    if (static_cast<int>(fields.size()) <= maxCol)
      continue;

    const std::string &energyStr = fields[energyCol];
    const std::string &yieldStr = fields[yieldCol];
    if (energyStr.empty() || yieldStr.empty())
      continue;

    G4double energy, yield;
    try {
      energy = std::stod(energyStr);
      yield = std::stod(yieldStr);
    } catch (const std::exception &) {
      continue; // malformed row, skip rather than crash
    }

    if (energy <= 0 || yield <= 0)
      continue;

    fEnergiesKeV.push_back(energy);
    weights.push_back(yield);
  }

  if (fEnergiesKeV.empty()) {
    throw std::runtime_error("No valid spectrum lines loaded from " +
                             std::string(csvPath));
  }

  G4double total = 0.0;
  for (auto w : weights)
    total += w;

  fCumulativeWeights.resize(weights.size());
  G4double running = 0.0;
  for (size_t i = 0; i < weights.size(); ++i) {
    running += weights[i] / total;
    fCumulativeWeights[i] = running;
  }
}

G4double PrimaryGeneratorAction::SampleLineEnergy() const {
  G4double r = G4UniformRand();
  auto it =
      std::lower_bound(fCumulativeWeights.begin(), fCumulativeWeights.end(), r);
  size_t idx = std::distance(fCumulativeWeights.begin(), it);
  if (idx >= fEnergiesKeV.size())
    idx = fEnergiesKeV.size() - 1;
  return fEnergiesKeV[idx];
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  G4double energy = SampleLineEnergy() * keV;
  fParticleGun->SetParticleEnergy(energy);

  G4double cosTheta = 2.0 * G4UniformRand() - 1.0;
  G4double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
  G4double phi = 2.0 * M_PI * G4UniformRand();
  G4ThreeVector direction(sinTheta * std::cos(phi), sinTheta * std::sin(phi),
                          cosTheta);
  fParticleGun->SetParticleMomentumDirection(direction);

  fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
