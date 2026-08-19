#include "NuECrossSection.h"
#include "MatrixBuilder.h"
#include "ElectronEnergyDistribution.h"
#include "Loglikelihood.h"
#include <iostream>
#include <TFile.h>


int main(int argc, char *argv[]){

  TString flux_file = argv[1];
  TString output_path = argv[2];
  TString output_name = argv[3];  
  double POT = std::stod(argv[4]);
 
  int numUnivs = 100; // choose # of universes!



  // ************ RUNNING LOG LIKELIHOOD CODE ***************
  TString file_Ee = "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/EeDistribution_ME_FHC_1000.root"; // this is the output of the files that get created above since using one runProject type file to run the whole thing
  TString file_EeMeasured = "/exp/minerva/data/users/lzazueta/NuE_stiched_histv2.root"; // this is Luis' measured Ee distribution, the first 6 bins are nu FHC and the second 6 bins are antinu RHC, see Table IV of https://arxiv.org/pdf/2209.05540
  TString histo_name = "kEeDistr_TOTAL";
  TString luis_histo_name = "data_stitched_electronE";

  // now we read the files and collect the histograms
  TFile* f_EePred = TFile::Open(file_Ee, "READ");
  PlotUtils::MnvH1D* EePred = dynamic_cast<PlotUtils::MnvH1D*>(f_EePred->Get(histo_name));

  TFile* f_EeData = TFile::Open(file_EeMeasured, "READ");
  PlotUtils::MnvH1D* EeMeasured = dynamic_cast<PlotUtils::MnvH1D*>(f_EeData->Get(luis_histo_name)); 

  // let's get the measurement
  TH1D* Ee_measured_FHC = EePred->Clone("Ee_measured_FHC");
  TH1D* Ee_measured_RHC = EePred->Clone("Ee_measured_RHC");
 
  for (int bin=1; bin<EePred->GetNbinsX()+1; bin++){
     Ee_measured_FHC->SetBinContent(bin,EeMeasured->GetBinContent(bin));
     Ee_measured_FHC->SetBinError(bin,EeMeasured->GetBinError(bin));

     Ee_measured_RHC->SetBinContent(bin,EeMeasured->GetBinContent(bin+6));
     Ee_measured_RHC->SetBinError(bin,EeMeasured->GetBinError(bin+6));
  }

  // we've read in the histograms, now we loop over the number of universes
  std::vector<double> loglike_total;
  std::vector<double> loglike_norm;
  std::vector<double> loglike_shape;

  TH1D* h_loglike_total = new TH1D("h_loglike_total", "h_loglike_total", 10, -100,0);
  TH1D* h_loglike_norm = new TH1D("h_loglike_norm", "h_loglike_norm", 10, -100, 0)
;
  TH1D* h_loglike_shape = new TH1D("h_loglike_shape", "h_loglike_shape", 10, -100, 0)
;

  double val; double val_norm; double val_shape;

  // we loop over the universes
  for (int univ=0; univ < 1000; univ++){
     val = CalculateTotalLogLikelihood(Ee_measured_FHC, EePred->GetVertErrorBand("Flux")->GetHist(univ));
     loglike_total.push_back(val);
     std::cout << val << std::endl;
     h_loglike_total->Fill(val);
      
     val_norm = CalculateNormLogLikelihood(Ee_measured_FHC, EePred->GetVertErrorBand("Flux")->GetHist(univ));
     loglike_norm.push_back(val_norm);
     std::cout << "Norm: " << val_norm << std::endl;
     h_loglike_norm->Fill(val_norm);

     val_shape = val - val_norm;
     loglike_shape.push_back(val_shape);
     std::cout << "Shape: " << val_shape << std::endl;
     h_loglike_shape->Fill(val_shape);

  } // end looping over universes
  std::cout << "Here's the val: "+std::to_string(val) << std::endl;

  TH1D* h_sumOfBins = new TH1D("sumOfBins", "sumOfBins", 100, 1000, 2000);
  double sum = 0;
  for (int univ=0; univ<1000; univ++){
     sum = SumOfBins(EePred->GetVertErrorBand("Flux")->GetHist(univ));
     h_sumOfBins->Fill(sum);
  }
  TFile f_sum("SumOfBins_ME_FHC.root", "Recreate");
  f_sum.cd();
  h_sumOfBins->Write("h_sumOfBins");
  f_sum.Close();

  TFile f("LogLikelihood_ME_FHC.root", "Recreate");
  f.cd();
  h_loglike_total->Write("h_loglike_total");
  h_loglike_norm->Write("h_loglike_norm");
  h_loglike_shape->Write("h_loglike_shape");
  f.Close();


  // ********************************************************





  std::vector<double> bins = {0.8,2,3,5,7,9,20}; // Electron Energy Distribution, that's the bins but then we'll pile in overflow into the 9-20 GeV bin, effectively making it 9 to infinity

  // Double check all of the POTs being used
  double input_POT_FHC_ME = 1.16e21; // Valencia paper
  double input_POT_RHC_ME = 1.12e21; // Luis' paper 
  double input_POT_FHC_LE = 3.32e+20; // from tuples
  double input_POT_RHC_LE = 1.03e+20; // from tuples


  EeDistributionCreator creator(flux_file, POT, bins,"/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f1_muonNeutrinos.csv","/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f10_muonNeutrinos.csv","/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f1_muonAntineutrinos.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f10_muonAntineutrinos.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f1_electronNeutrinos.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f10_electronNeutrinos.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f10_electronAntineutrinos.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/f1_electronAntineutrinos.csv",numUnivs);    

  std::cout << "Calling LoadFluxHistos()" << std::endl;
  creator.LoadFluxHistos();
  creator.CreateXSecMatrices();
  creator.WriteEverythingToROOTFile(output_path, output_name);
 






//  creator.WriteFIG8ToFile(output_path, "FIG8","/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_numu.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_numubar.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_nue.csv", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_nuebar.csv");

  // this still doesn't have the correction from the email but it is the numbers without digitizing them  which is how I originally did it

//  creator.WriteFIG8ToFile(output_path, "FIG8_EmailNumbers","/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_numu_EmailNumbers.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_numubar_EmailNumbers.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_nue_EmailNumbers.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Total_nuebar_EmailNumbers.txt");

  // this is the plot with the updates from the email now

  creator.WriteFIG8ToFile(output_path, "FIG8_UpdatedEmailNumbers","/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Updated_Total_numu.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Updated_Total_numubar.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Updated_Total_nue.txt", "/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/inputs/Updated_Total_nuebar.txt");

//************************
// Write Fluxes to File
//************************

//  creator.WriteFluxesToFile(output_path, "Fluxes_Quinn_LE_FHC","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg14-minerva1.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg-14-minerva1.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg12-minerva1.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg-12-minerva1.root", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted");
//  creator.WriteFluxesToFile(output_path, "Fluxes_Quinn_LE_RHC","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg14-minerva5.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg-14-minerva5.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg12-minerva5.root","/exp/minerva/data/users/qvuong/flux_studies/producedFluxes_p8/flux-gen2thin-pdg-12-minerva5.root", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted");

  creator.WriteFluxesToFile(output_path, "Fluxes_FRW_ME_FHC","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg14-minervame1D1M1NWeightedAve.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg-14-minervame1D1M1NWeightedAve.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg12-minervame1D1M1NWeightedAve.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg-12-minervame1D1M1NWeightedAve.root", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted");

//  creator.WriteFluxesToFile(output_path, "Fluxes_FRW_ME_RHC","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg14-minervame6A.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg-14-minervame6A.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg12-minervame6A.root","/exp/minerva/app/users/mmehmood/MAT_AL9/opt/lib/data/flux/flux-gen2thin-pdg-12-minervame6A.root", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted", "flux_E_cvweighted");

// *******************************

  // Ensure calling this AFTER LoadFluxHistos(), CreateXSecMatrices(), SetupEeDistributions()

// For writing radiative corrections to file
/*
 creator.WriteRadCorrToFileForFixedEnu(output_path, "RadCorr_Enu5GeV",5.0);

 creator.WriteRadCorrToFileForFixedEnu(output_path, "RadCorr_Enu10GeV",10.0);

 creator.WriteRadCorrToFileForFixedEnu(output_path, "RadCorr_Enu1GeV",1.0);

 creator.WriteRadCorrToFileForFixedEnu(output_path, "RadCorr_Enu100GeV",100.0);
*/


   return 0.0;


}



