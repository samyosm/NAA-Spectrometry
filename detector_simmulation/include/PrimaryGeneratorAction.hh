#pragma once

#include "G4String.hh"
#include "G4Types.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include <vector>

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction(const G4String &csvPath);
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event *anEvent) override;

private:
  void LoadSpectrum(const G4String &csvPath);
  G4double SampleLineEnergy() const;

private:
  G4ParticleGun *fParticleGun;
  std::vector<G4double> fEnergiesKeV;
  std::vector<G4double> fCumulativeWeights;
};
