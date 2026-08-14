#include "NuECrossSection.h"
#include "MatrixBuilder.h"
#include "ElectronEnergyDistribution.h"
#include <iostream>
#include <TFile.h>


int main(int argc, char *argv[]){

  TString flux_file = argv[1];
  TString output_path = argv[2];
  TString output_name = argv[3];

 
  std::vector<double> bins = {0.8,2,3,5,7,9,20}; // Electron Energy Distribution, that's the bins but then we'll pile in overflow into the 9-20 GeV bin, effectively making it 9 to infinity

  // Double check all of the POTs being used
  double input_POT_FHC_ME = 1.16e21; // Valencia paper
  double input_POT_RHC_ME = 1.12e21; // Luis' paper 
  double input_POT_FHC_LE = 3.32e+20; // from tuples
  double input_POT_RHC_LE = 1.03e+20; // from tuples

  int numUnivs = 100; // choose # of universes!

  EeDistributionCreator creator(flux_file, input_POT_FHC_ME, input_POT_RHC_ME, input_POT_FHC_LE, input_POT_RHC_LE, bins,"/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f1_muonNeutrinos.csv","/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f10_muonNeutrinos.csv","/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f1_muonAntineutrinos.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f10_muonAntineutrinos.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f1_electronNeutrinos.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f10_electronNeutrinos.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f10_electronAntineutrinos.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f1_electronAntineutrinos.csv",numUnivs);    

  std::cout << "Calling LoadFluxHistos()" << std::endl;
  creator.LoadFluxHistos();

  std::cout << "Calling CreateXSecMatrices()" << std::endl;
  creator.CreateXSecMatrices();
  creator.SetupEeDistributions();
//  std::cout << "Calling GetTotalEeDistribution()" << std::endl;
//  creator.GetTotalEeDistribution(); // Already gets called in the WriteEverything function
   creator.WriteEverythingToROOTFile(output_path, output_name);
 
//  creator.WriteFIG8ToFile(output_path, "FIG8","/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_numu.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_numubar.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_nue.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_nuebar.csv");

  // this still doesn't have the correction from the email but it is the numbers without digitizing them  which is how I originally did it

  creator.WriteFIG8ToFile(output_path, "FIG8_EmailNumbers","/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_numu_EmailNumbers.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_numubar_EmailNumbers.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_nue_EmailNumbers.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_nuebar_EmailNumbers.txt");

  // this is the plot with the updates from the email now

  creator.WriteFIG8ToFile(output_path, "FIG8_UpdatedEmailNumbers","/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Updated_Total_numu.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Updated_Total_numubar.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Updated_Total_nue.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Updated_Total_nuebar.txt");

  creator.WriteFluxesToFile(output_path, "Fluxes_Quinn_LE_FHC","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg14-minerva1.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg-14-minerva1.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg12-minerva1.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg-12-minerva1.root", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted");
  creator.WriteFluxesToFile(output_path, "Fluxes_Quinn_LE_RHC","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg14-minerva5.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg-14-minerva5.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg12-minerva5.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg-12-minerva5.root", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted");

  creator.WriteFluxesToFile(output_path, "Fluxes_FRW_ME_FHC","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg14-minervame1D1M1NWeightedAve.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg-14-minervame1D1M1NWeightedAve.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg12-minervame1D1M1NWeightedAve.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg-12-minervame1D1M1NWeightedAve.root", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted");

  creator.WriteFluxesToFile(output_path, "Fluxes_FRW_ME_RHC","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg14-minervame6A.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg-14-minervame6A.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg12-minervame6A.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg-12-minervame6A.root", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted");

  // Ensure calling this AFTER LoadFluxHistos(), CreateXSecMatrices(), SetupEeDistributions()


  creator.WriteRadCorrToFileForFixedEnu(output_path, "RadCorr_Enu5GeV",5.0);

 creator.WriteRadCorrToFileForFixedEnu(output_path, "RadCorr_Enu10GeV",10.0);

 creator.WriteRadCorrToFileForFixedEnu(output_path, "RadCorr_Enu1GeV",1.0);

 creator.WriteRadCorrToFileForFixedEnu(output_path, "RadCorr_Enu100GeV",100.0);

/*
   std::cout << GetY(2,4) << std::endl;

   Couplings test = GetCouplings(14);
   std::cout << test.cLL << " " << test.cLR << " " << test.isAntinu << std::endl;
   std::cout << "Mandelstam s: "<< GetMandelstamInvariant_s(2) << std::endl;

   std::cout << "DSigmaDY, pdg=14: " << DSigmaDY(14,4,0.8) << std::endl;

   std::cout << "DSigmaDY, pdg=-14: " << DSigmaDY(-14,3,0.8) << std::endl;
   std::cout << "DSigmaDY, pdg=12: " << DSigmaDY(12,3,0.8) << std::endl;
   std::cout << "DSigmaDY, pdg=-12: " << DSigmaDY(-12,3,0.8) << std::endl;
   
   std::cout << "Simpson's rule for numerical integration: "<< IntegrateDSigmaDY(14, 5, 0, 0.5, 1000) << std::endl;

   std::cout << "Matrix Builder Testing Now!" << std::endl;
   MatrixK m = MatrixK("Test", 3, 0.0, 100.0, 4, 0., 100.);
   std::cout << "Ee Bins: " << m.GetNumEeBins() << std::endl;
//   m.SetNumEeBins(6);
//   std::cout << "New Ee Bins: " << m.GetNumEeBins() << std::endl;
//   std::cout << "New Enu Bins: " << m.GetNumEnuBins() << std::endl;

   MatrixK q = MatrixK("Test2", {2,3,4}, 6,0,10 );

   auto histo = m.GetMatrixHisto();
   auto histoq = q.GetMatrixHisto();
   TFile outfile("test.root", "RECREATE");
   outfile.cd();
   histo->Write();
   histoq->Write();
   q.PopulateMatrixHisto(14);
   histoq = q.GetMatrixHisto();
   histoq->Write("AfterPopulate");

   outfile.Close();
*/
   


   return 0.0;


}



