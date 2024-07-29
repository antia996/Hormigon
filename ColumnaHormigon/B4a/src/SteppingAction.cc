//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B4/B4a/src/SteppingAction.cc
/// \brief Implementation of the B4a::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4AnalysisManager.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"

using namespace B4;

namespace B4a
{

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    SteppingAction::SteppingAction(const DetectorConstruction* detConstruction,
        EventAction* eventAction)
        : fDetConstruction(detConstruction),
        fEventAction(eventAction),
        fLastPosition(G4ThreeVector())
    {}

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void SteppingAction::UserSteppingAction(const G4Step* step)
    {

        // Collect energy and track length step by step

          // get volume of the current step
        auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

        // energy deposit
        auto edep = step->GetTotalEnergyDeposit();

        auto particle = step->GetTrack()->GetDefinition();

        G4ThreeVector position = step->GetPostStepPoint()->GetPosition();

        // step length
        G4double stepLength = 0.;
        if (step->GetTrack()->GetDefinition()->GetPDGCharge() == 0. && particle->GetPDGEncoding() == 2112) {
            stepLength = step->GetStepLength();
        }

        if (volume == fDetConstruction->GetDetectorPhys()) {
            fEventAction->AddDetector(edep, stepLength);
        }



        
        //DetectorConstruction* detectorConstruction =
          //  dynamic_cast<DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        //
        // const G4VPhysicalVolume* detectorPhys = detectorConstruction->GetDetectorPhys();

        const DetectorConstruction* detectorConstruction =
            dynamic_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        if (DetectorConstruction) {
            // Obtener el volumen físico del detector
            const G4VPhysicalVolume* detectorPhys = DetectorConstruction->GetDetectorPhys();
        


            if (particle->GetPDGEncoding() == 2112) {

                if (step->GetPreStepPoint()->GetTouchable()->GetVolume() == detectorPhys)
                {
                    // Lógica para obtener la última posición de la partícula
                    const G4Track* track = step->GetTrack();
                    const G4ThreeVector& position = step->GetPostStepPoint()->GetPosition();

                    // Actualiza la última posición en el EventAction
                    if (fEventAction)
                    {
                        fEventAction->SetLastPosition(position);
                    }
                }
            }
            
            /*
            if (volume == fDetConstruction->GetDetectorPhys()) {
                G4Track* track = step->GetTrack();

                G4VUserTrackInformation* userInfo = track->GetUserInformation();

                if (!userInfo) {
                    userInfo = new G4VUserTrackInformation();
                    track->SetUserInformation(userInfo);
                    G4double x = step->GetPreStepPoint()->GetPosition().x();
                    G4double y = step->GetPreStepPoint()->GetPosition().y();

                    auto analysisManager = G4AnalysisManager::Instance();


                    //G4double x = step->GetPreStepPoint()->GetPosition().x();
                    //G4double y = step->GetPreStepPoint()->GetPosition().y();

                    //auto analysisManager = G4AnalysisManager::Instance();
                    //analysisManager->FillH2(0, -x, y);
                    if (volume == fDetConstruction->GetDetectorPhys()) {
                        analysisManager->FillH2(0, x, y);
                    }
                }
            }
            */

        }



        //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    }
}
