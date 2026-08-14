// ElectronEnergyDistribution.h

#ifndef ELECTRONENERGYDISTRIBUTION_H
#define ELECTRONENERGYDISTRIBUTION_H

#include <TFile.h>
#include <TH2D.h>
#include "PlotUtils/MnvH1D.h"
#include "MatrixBuilder.h"
#include "RadCorrectionTomalak.h"

constexpr double kNumElectrons = 1.99e30; // number of electrons, picked up from pg 12 of Valencia paper: https://arxiv.org/pdf/1906.00111

class EeDistributionCreator{
   public:
      EeDistributionCreator(TString fluxFilePath,double input_POT_FHC_ME, double input_POT_RHC_ME, double input_POT_FHC_LE, double input_POT_RHC_LE, std::vector<double> EeBinEdges, TString f1_FilePath_numu, TString f10_FilePath_numu, TString f1_FilePath_numubar, TString f10_FilePath_numubar, TString f1_FilePath_nue, TString f10_FilePath_nue, TString f1_FilePath_nuebar, TString f10_FilePath_nuebar, int numUnivs);      

      ~EeDistributionCreator(); // destructor

      void CreateXSecMatrices();

      TH2D* GetXSecMatrix(int pdg);

      void SetupEeDistributions();

      std::vector<double> GetNeutrinoEnergyBinning(int pdg);
       
      void WriteFIG8ToFile(TString path, TString output_name, TString f_numuFilePath,TString f_numubarFilePath, TString f_nueFilePath, TString f_nuebarFilePath);
     
      void WriteEverythingToROOTFile(TString path, TString output_name);

      void LoadFluxHistos();

      void WriteFluxesToFile(TString path, TString ouput_name, TString f_numuFilePath, TString f_numubarFilePath, TString f_nueFilePath, TString f_nuebarFilePath, TString f_numuFluxName, TString f_numubarFluxName, TString f_nueFluxName, TString f_nuebarFluxName);

      // write the rad correction for fixed neutrino energy to file
      void WriteRadCorrToFileForFixedEnu(TString path, TString output_name, double Enu);

      PlotUtils::MnvH1D* PopulateEeDistributions(MatrixK* kXSecMatrix, PlotUtils::MnvH1D* kFluxDistr, PlotUtils::MnvH1D* kEeDistr, double POT);
      void PopulateEeDistributions(int pdg);
  
      PlotUtils::MnvH1D* GetTotalEeDistribution();

   private:
      TString kfluxFilePath;
      // electron energy distributions for each flavour
      PlotUtils::MnvH1D* kEeDistr_PDG14;
      PlotUtils::MnvH1D* kEeDistr_PDGminus14;
      PlotUtils::MnvH1D* kEeDistr_PDG12;
      PlotUtils::MnvH1D* kEeDistr_PDGminus12;
      // flux distributions for each flavour
      PlotUtils::MnvH1D* kFluxDistr_PDG14;
      PlotUtils::MnvH1D* kFluxDistr_PDGminus14;
      PlotUtils::MnvH1D* kFluxDistr_PDG12;
      PlotUtils::MnvH1D* kFluxDistr_PDGminus12;

      double POT_FHC_ME;
      double POT_RHC_ME;
      double POT_FHC_LE;
      double POT_RHC_LE;


      // xsec matrices for each flavour
//      TH2D* kXSecMatrix_PDG14;
//      TH2D* kXSecMatrix_PDGminus14;
//      TH2D* kXSecMatrix_PDG12;
//      TH2D* kXSecMatrix_PDGminus12;

      MatrixK* kXSecMatrix_PDG14;
      MatrixK* kXSecMatrix_PDGminus14;
      MatrixK* kXSecMatrix_PDG12;
      MatrixK* kXSecMatrix_PDGminus12;

      MatrixK* kXSecMatrix_UFOF_PDG14;
      MatrixK* kXSecMatrix_UFOF_PDGminus14;
      MatrixK* kXSecMatrix_UFOF_PDG12;
      MatrixK* kXSecMatrix_UFOF_PDGminus12;

      std::vector<double> kEeBinEdges;
      std::vector<double> kEeBinEdges_UFOF;

      int kNumUnivs; 

      RadCorrTomalak* kRadCorr_PDG14;
      RadCorrTomalak* kRadCorr_PDGminus14;
      RadCorrTomalak* kRadCorr_PDG12;
      RadCorrTomalak* kRadCorr_PDGminus12;

}; // end class EeDistributionCreator



#endif 
