#include "../include/ElectronEnergyDistribution.h"
#include "../include/MatrixBuilder.h"

EeDistributionCreator::EeDistributionCreator(TString fluxFilePath, double input_POT_FHC_ME, double input_POT_RHC_ME, double input_POT_FHC_LE, double input_POT_RHC_LE, std::vector<double> EeBinEdges, TString f1_FilePath_numu, TString f10_FilePath_numu, TString f1_FilePath_numubar, TString f10_FilePath_numubar, TString f1_FilePath_nue, TString f10_FilePath_nue, TString f1_FilePath_nuebar, TString f10_FilePath_nuebar)
   : kfluxFilePath(fluxFilePath), kXSecMatrix_PDG14(nullptr),
     kXSecMatrix_PDGminus14(nullptr), 
     kXSecMatrix_PDG12(nullptr),
     kXSecMatrix_PDGminus12(nullptr), kXSecMatrix_UFOF_PDG14(nullptr), 
     kXSecMatrix_UFOF_PDGminus14(nullptr),
     kXSecMatrix_UFOF_PDG12(nullptr),
     kXSecMatrix_UFOF_PDGminus12(nullptr),
     kEeBinEdges(EeBinEdges){

   kEeDistr_PDG14 = nullptr;
   kEeDistr_PDGminus14 = nullptr;
   kEeDistr_PDG12 = nullptr;
   kEeDistr_PDGminus12 = nullptr;

   kFluxDistr_PDG14 = nullptr;
   kFluxDistr_PDGminus14 = nullptr;
   kFluxDistr_PDG12  = nullptr;
   kFluxDistr_PDGminus12 = nullptr;

   kEeBinEdges_UFOF = EeBinEdges;
   kEeBinEdges_UFOF.push_back(EeBinEdges.back()+100);
   kEeBinEdges_UFOF.insert(kEeBinEdges_UFOF.begin(),0);

   kRadCorr_PDG14 = new RadCorrTomalak( f1_FilePath_numu, f10_FilePath_numu);
   kRadCorr_PDGminus14 = new RadCorrTomalak( f1_FilePath_numubar, f10_FilePath_numubar);
   kRadCorr_PDG12 = new RadCorrTomalak( f1_FilePath_nue, f10_FilePath_nue);
   kRadCorr_PDGminus12 = new RadCorrTomalak(f1_FilePath_nuebar, f10_FilePath_nuebar);

   POT_FHC_ME = input_POT_FHC_ME;
   POT_RHC_ME = input_POT_RHC_ME;
   POT_FHC_LE = input_POT_FHC_LE;
   POT_RHC_LE = input_POT_RHC_LE;

} // end constructor

EeDistributionCreator::~EeDistributionCreator(){
   delete kXSecMatrix_PDG14;
   delete kXSecMatrix_PDGminus14;
   delete kXSecMatrix_PDG12;
   delete kXSecMatrix_PDGminus12;

   delete kXSecMatrix_UFOF_PDG14;
   delete kXSecMatrix_UFOF_PDGminus14;
   delete kXSecMatrix_UFOF_PDG12;
   delete kXSecMatrix_UFOF_PDGminus12;

   delete kRadCorr_PDG14;
   delete kRadCorr_PDGminus14;
   delete kRadCorr_PDG12;
   delete kRadCorr_PDGminus12;

   delete kEeDistr_PDG14;
   delete kEeDistr_PDGminus14;
   delete kEeDistr_PDG12;
   delete kEeDistr_PDGminus12;
}

PlotUtils::MnvH1D* EeDistributionCreator::GetTotalEeDistribution(){

   auto totalEeDistr = (PlotUtils::MnvH1D*)(kEeDistr_PDG14->Clone("kEeDistr_TOTAL"));
   totalEeDistr->Add(kEeDistr_PDGminus14);
   totalEeDistr->Add(kEeDistr_PDG12);
   totalEeDistr->Add(kEeDistr_PDGminus12);
   return totalEeDistr;
}
   

void EeDistributionCreator::CreateXSecMatrices(){
//   kXSecMatrix_PDG14 = new MatrixK("MatrixK_PDG14", {5,6,7,8}, 6,0,10 );
   auto bins_PDG14 = GetNeutrinoEnergyBinning(14);
   auto bins_PDGminus14 = GetNeutrinoEnergyBinning(-14);
   auto bins_PDG12 = GetNeutrinoEnergyBinning(12);
   auto bins_PDGminus12 = GetNeutrinoEnergyBinning(-12);

   // for raw pointers, delete any old objects first
   delete kXSecMatrix_PDG14;
   delete kXSecMatrix_PDGminus14;
   delete kXSecMatrix_PDG12;
   delete kXSecMatrix_PDGminus12; 
   delete kXSecMatrix_UFOF_PDG14;
   delete kXSecMatrix_UFOF_PDGminus14;
   delete kXSecMatrix_UFOF_PDG12;
   delete kXSecMatrix_UFOF_PDGminus12; 

   kXSecMatrix_PDG14 = new MatrixK("kXSecMatrix_PDG14",bins_PDG14, kEeBinEdges);
   kXSecMatrix_PDGminus14 = new MatrixK("kXSecMatrix_PDGminus14", bins_PDGminus14, kEeBinEdges);
   kXSecMatrix_PDG12 = new MatrixK("kXSecMatrix_PDG12", bins_PDG12, kEeBinEdges);
   kXSecMatrix_PDGminus12 = new MatrixK("kXSecMatrix_PDGminus12", bins_PDGminus12, kEeBinEdges);

   // The bins that explicitly include the OF and UF
   bins_PDG14.push_back(bins_PDG14.back()+100);
   bins_PDGminus14.push_back(bins_PDGminus14.back()+100);
   bins_PDG12.push_back(bins_PDG12.back()+100);
   bins_PDGminus12.push_back(bins_PDGminus12.back()+100);
   bins_PDG14.insert(bins_PDG14.begin(), 0); // since mass of electron is 0.511 MeV, maybe this should start at 0.511 ??
   bins_PDGminus14.insert(bins_PDGminus14.begin(), 0);
   bins_PDG12.insert(bins_PDG12.begin(), 0);
   bins_PDGminus12.insert(bins_PDGminus12.begin(), 0);
   kXSecMatrix_UFOF_PDG14 = new MatrixK("kXSecMatrix_UFOF_PDG14",bins_PDG14, kEeBinEdges_UFOF);
   kXSecMatrix_UFOF_PDGminus14 = new MatrixK("kXSecMatrix_UFOF_PDGminus14", bins_PDGminus14, kEeBinEdges_UFOF);
   kXSecMatrix_UFOF_PDG12 = new MatrixK("kXSecMatrix_UFOF_PDG12", bins_PDG12, kEeBinEdges_UFOF);
   kXSecMatrix_UFOF_PDGminus12 = new MatrixK("kXSecMatrix_UFOF_PDGminus12", bins_PDGminus12, kEeBinEdges_UFOF);

   kXSecMatrix_PDG14->PopulateMatrixHisto(14, kRadCorr_PDG14);
   kXSecMatrix_PDGminus14->PopulateMatrixHisto(-14, kRadCorr_PDGminus14);
   kXSecMatrix_PDG12->PopulateMatrixHisto(12, kRadCorr_PDG12);
   kXSecMatrix_PDGminus12->PopulateMatrixHisto(-12, kRadCorr_PDGminus12);

   kXSecMatrix_UFOF_PDG14->PopulateMatrixHisto(14, kRadCorr_PDG14);
   kXSecMatrix_UFOF_PDGminus14->PopulateMatrixHisto(-14, kRadCorr_PDGminus14);
   kXSecMatrix_UFOF_PDG12->PopulateMatrixHisto(12, kRadCorr_PDG12);
   kXSecMatrix_UFOF_PDGminus12->PopulateMatrixHisto(-12, kRadCorr_PDGminus12);

   // fill the xsec histos which are the projections of these matrices for ref too
  kXSecMatrix_PDG14->FillMatrixProj_XSecHisto();
  kXSecMatrix_PDGminus14->FillMatrixProj_XSecHisto();
  kXSecMatrix_PDG12->FillMatrixProj_XSecHisto();
  kXSecMatrix_PDGminus12->FillMatrixProj_XSecHisto();

  kXSecMatrix_UFOF_PDG14->FillMatrixProj_XSecHisto();
  kXSecMatrix_UFOF_PDGminus14->FillMatrixProj_XSecHisto();
  kXSecMatrix_UFOF_PDG12->FillMatrixProj_XSecHisto();
  kXSecMatrix_UFOF_PDGminus12->FillMatrixProj_XSecHisto();

} // end CreateXSecMatrix

void EeDistributionCreator::SetupEeDistributions(){
   // raw pointers
   delete kEeDistr_PDG14;
   delete kEeDistr_PDGminus14;
   delete kEeDistr_PDG12;
   delete kEeDistr_PDGminus12;

   // setup the distributions now
   kEeDistr_PDG14 = new PlotUtils::MnvH1D("kEeDistr_PDG14", "kEeDistr_PDG14", kEeBinEdges.size()-1, kEeBinEdges.data());
   kEeDistr_PDGminus14 = new PlotUtils::MnvH1D("kEeDistr_PDGminus14", "kEeDistr_PDGminus14", kEeBinEdges.size()-1, kEeBinEdges.data());
   kEeDistr_PDG12 = new PlotUtils::MnvH1D("kEeDistr_PDG12", "kEeDistr_PDG12", kEeBinEdges.size()-1, kEeBinEdges.data());
   kEeDistr_PDGminus12 = new PlotUtils::MnvH1D("kEeDistr_PDGminus12", "kEeDistr_PDGminus12", kEeBinEdges.size()-1, kEeBinEdges.data());

} // end SetupEeDistributions

void EeDistributionCreator::PopulateEeDistributions(){
  // we want to populate the electron energy distribution
  // multiply row of matrix with flux col to get corresponding entry in electron energy distribution

  
  double val = 0.0; double xsec; double flux; double width;
  int globalBin; 
//  double POT = 1.0583546e+21;
  double POT = 1.16e+21;
  for (int j=1; j<=kEeDistr_PDG14->GetNbinsX()+1; j++){
     for (int i=1; i<=kXSecMatrix_PDG14->GetMatrixHisto()->GetNbinsX(); i++){
        globalBin = kXSecMatrix_PDG14->GetMatrixHisto()->GetBin(i,j);
        xsec = kXSecMatrix_PDG14->GetMatrixHisto()->GetBinContent(globalBin);
        flux = kFluxDistr_PDG14->GetBinContent(i); 
        width = kFluxDistr_PDG14->GetBinWidth(i);
        val = val + xsec*flux*width;

   } // looping over neutrino energy bins of cross-section matrix
     val = val * (3.894e-28) * (1e-4) * kNumElectrons * POT; 
     kEeDistr_PDG14->SetBinContent(j, val);
     val = 0.0;
  }


} // end PopulateEeDistributions


void EeDistributionCreator::LoadFluxHistos(){
   auto fluxFile = new TFile(kfluxFilePath, "READ");
   std::cout << "NOTE: INPUT CORRECT FLUX HISTO NAMES" << std::endl;
   kFluxDistr_PDG14 = (PlotUtils::MnvH1D*) fluxFile->Get("flux_E_cvweighted_numu");
   kFluxDistr_PDGminus14 = (PlotUtils::MnvH1D*)fluxFile->Get("flux_E_cvweighted_numubar");
   kFluxDistr_PDG12 = (PlotUtils::MnvH1D*)fluxFile->Get("flux_E_cvweighted_nue");
   kFluxDistr_PDGminus12 = (PlotUtils::MnvH1D*)fluxFile->Get("flux_E_cvweighted_nuebar");
} // end LoadFluxHistos




std::vector<double> EeDistributionCreator::GetNeutrinoEnergyBinning(int pdg){
   std::vector<double> bins;
   TH1D* flux = nullptr;
   if (pdg == 14){ flux = kFluxDistr_PDG14; }
   else if (pdg == -14) { flux = kFluxDistr_PDGminus14; }
   else if (pdg == 12) { flux = kFluxDistr_PDG12; }
   else if (pdg == -12) { flux = kFluxDistr_PDGminus12; }

   if (!flux) return bins; // don't want to call GetNbins if nullptr

   for(int i=1;i<=flux->GetNbinsX()+1;i++){
         bins.push_back(flux->GetXaxis()->GetBinLowEdge(i));
   }
   return bins;
}


TH2D* EeDistributionCreator::GetXSecMatrix(int pdg){
   if(pdg == 14){return kXSecMatrix_PDG14->GetMatrixHisto();}
//   else if (pdg == -14){return kXSecMatrix_PDGminus14.GetMatrixHisto();}
//   else if (pdg == 12) {return kXSecMatrix_PDG12.GetMatrixHisto();}
//   else if (pdg == -12) {return kXSecMatrix_PDGminus12.GetMatrixHisto();}
   else {throw std::runtime_error("Invalid PDG!");}
}

void EeDistributionCreator::WriteFluxesToFile(TString path, TString output_name, TString f_numuFilePath, TString f_numubarFilePath, TString f_nueFilePath, TString f_nuebarFilePath, TString f_numuFluxName, TString f_numubarFluxName, TString f_nueFluxName, TString f_nuebarFluxName){
   auto file_numu = new TFile(f_numuFilePath, "read");
   auto file_numubar = new TFile(f_numubarFilePath, "read");
   auto file_nue = new TFile(f_nueFilePath, "read");
   auto file_nuebar = new TFile(f_nuebarFilePath, "read");

   auto numu_flux = file_numu->Get(f_numuFluxName);
   auto numubar_flux = file_numubar->Get(f_numubarFluxName);
   auto nue_flux = file_nue->Get(f_nueFluxName);
   auto nuebar_flux = file_nuebar->Get(f_nuebarFluxName);

   auto fileName = path+output_name+".root";
   TFile f(fileName, "RECREATE");
   f.cd();
   numu_flux->Write("flux_E_cvweighted_numu");
   numubar_flux->Write("flux_E_cvweighted_numubar");
   nue_flux->Write("flux_E_cvweighted_nue");
   nuebar_flux->Write("flux_E_cvweighted_nuebar");
   
   f.Close();
} // end EeDistributionCreator::WriteFluxesToFile

void EeDistributionCreator::WriteFIG8ToFile(TString path, TString output_name, TString f_numuFilePath, TString f_numubarFilePath, TString f_nueFilePath, TString f_nuebarFilePath){
   auto numu_Graph = new TGraph(f_numuFilePath, "%lg,%lg"); // reading floating point numbers from .csv file which is comma separated
   auto numubar_Graph = new TGraph(f_numubarFilePath, "%lg,%lg");
   auto nue_Graph = new TGraph(f_nueFilePath, "%lg,%lg");
   auto nuebar_Graph = new TGraph(f_nuebarFilePath, "%lg,%lg");


   numu_Graph->Sort(); // since the 0.5 point is that the end, root wasn't picking it up when it was the first point
   numubar_Graph->Sort();
   nue_Graph->Sort();
   nuebar_Graph->Sort();


   auto filename = path+output_name+".root";
   TFile f(filename, "RECREATE");
   f.cd();
   numu_Graph->Write("numu");
   numubar_Graph->Write("numubar");   
   nue_Graph->Write("nue");
   nuebar_Graph->Write("nuebar");

   f.Close();
} // end EeDistributionCreator::WriteFIG8ToFile

// write a function that writes the radiative correction to file for a given neutrino energy, for all of the 4 modes
void EeDistributionCreator::WriteRadCorrToFileForFixedEnu(TString path, TString output_name, double Enu){
   TH1D* numu_rad = new TH1D("numu_rad", "numu_rad", 100, 0, 1);
   TH1D* numubar_rad = new TH1D("numubar_rad", "numubar_rad", 100, 0, 1);
   TH1D* nue_rad = new TH1D("nue_rad", "nue_rad", 100, 0, 1);
   TH1D* nuebar_rad = new TH1D("nuebar_rad", "nuebar_rad", 100, 0, 1);

   double y;
   for (int i =1;i<numu_rad->GetNbinsX()+1;i++){
     y = numu_rad->GetBinCenter(i); 
     numu_rad->SetBinContent(i, kRadCorr_PDG14->GetRadCorr_Tomalak(Enu, y));
     numubar_rad->SetBinContent(i, kRadCorr_PDGminus14->GetRadCorr_Tomalak(Enu, y));
     nue_rad->SetBinContent(i, kRadCorr_PDG12->GetRadCorr_Tomalak(Enu, y));
     nuebar_rad->SetBinContent(i, kRadCorr_PDGminus12->GetRadCorr_Tomalak(Enu, y));
   }

   auto fileName = path+output_name+".root";
   TFile f(fileName, "RECREATE");
   numu_rad->Write("numu_rad");
   numubar_rad->Write("numubar_rad");
   nue_rad->Write("nue_rad");
   nuebar_rad->Write("nuebar_rad");

   f.Close();
   
   // getting a memory leak when calling this func a second time
   delete numu_rad;
   delete numubar_rad;
   delete nue_rad;
   delete nuebar_rad;


} // end WriteRadCorrToFileForFixedEnu


void EeDistributionCreator::WriteEverythingToROOTFile(TString path, TString output_name){
   auto filename = path+output_name+".root";
   TFile f(filename, "RECREATE");
   f.cd();
   std::cout << "yes! " << std::endl;
   kFluxDistr_PDG14->Write("kFluxDistr_PDG14");  
   kFluxDistr_PDGminus14->Write("kFluxDistr_PDGminus14");
   kFluxDistr_PDG12->Write("kFluxDistr_PDG12");
   kFluxDistr_PDGminus12->Write("kFluxDistr_PDGminus12");

   kXSecMatrix_PDG14->GetMatrixHisto()->Write("kXSecMatrix_PDG14");
   kXSecMatrix_PDGminus14->GetMatrixHisto()->Write("kXSecMatrix_PDGminus14");
   kXSecMatrix_PDG12->GetMatrixHisto()->Write("kXSecMatrix_PDG12");
   kXSecMatrix_PDGminus12->GetMatrixHisto()->Write("kXSecMatrix_PDGminus12");   

   kXSecMatrix_PDG14->GetMatrixProj_XSecHisto()->Write("kXSecMatrix_XsecProj_PDG14");
   kXSecMatrix_PDGminus14->GetMatrixProj_XSecHisto()->Write("kXSecMatrix_XsecProj_PDGminus14");
   kXSecMatrix_PDG12->GetMatrixProj_XSecHisto()->Write("kXSecMatrix_XsecProj_PDG12");
   kXSecMatrix_PDGminus12->GetMatrixProj_XSecHisto()->Write("kXSecMatrix_XsecProj_PDGminus12");

   kEeDistr_PDG14->Write("kEeDistr_PDG14");   
   kEeDistr_PDGminus14->Write("kEeDistr_PDGminus14");
   kEeDistr_PDG12->Write("kEeDistr_PDG12");
   kEeDistr_PDGminus12->Write("kEeDistr_PDGminus12");

   auto kEeDistr_TOTAL = GetTotalEeDistribution();
   kEeDistr_TOTAL->Write("kEeDistr_TOTAL");
   delete kEeDistr_TOTAL;

   kXSecMatrix_UFOF_PDG14->GetMatrixHisto()->Write("kXSecMatrix_UFOF_PDG14");
   kXSecMatrix_UFOF_PDGminus14->GetMatrixHisto()->Write("kXSecMatrix_UFOF_PDGminus14");
   kXSecMatrix_UFOF_PDG12->GetMatrixHisto()->Write("kXSecMatrix_UFOF_PDG12");
   kXSecMatrix_UFOF_PDGminus12->GetMatrixHisto()->Write("kXSecMatrix_UFOF_PDGminus12");
  
   kXSecMatrix_UFOF_PDG14->GetMatrixProj_XSecHisto()->Write("kXSecMatrix_UFOF_XsecProj_PDG14");
   kXSecMatrix_UFOF_PDGminus14->GetMatrixProj_XSecHisto()->Write("kXSecMatrix_UFOF_XsecProj_PDGminus14");
   kXSecMatrix_UFOF_PDG12->GetMatrixProj_XSecHisto()->Write("kXSecMatrix_UFOF_XsecProj_PDG12");
   kXSecMatrix_UFOF_PDGminus12->GetMatrixProj_XSecHisto()->Write("kXSecMatrix_UFOF_XsecProj_PDGminus12");

   f.Close();

}
