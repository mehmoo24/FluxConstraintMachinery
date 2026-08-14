#include "../include/MatrixBuilder.h"
#include "../include/NuECrossSection.h"
#include <algorithm>


MatrixK::MatrixK(const char* matrixName, int numEnuBins, double Enu_min, double Enu_max, int numEeBins, double Ee_min, double Ee_max)
   : fnumEeBins(numEeBins), fnumEnuBins(numEnuBins) {


   matrixHisto = new TH2D(matrixName, matrixName, fnumEnuBins, Enu_min, Enu_max, fnumEeBins, Ee_min, Ee_max);
   TString histname = TString(matrixName)+"_xsec";
   xsecHisto = new TH1D(histname, histname, fnumEnuBins, Enu_min, Enu_max);
   FIG8_ref_xsecHisto = new TH1D(TString(histname)+"_FIG8_Ref", TString(histname)+"_FIG8_Ref", fnumEnuBins, Enu_min, Enu_max);

} // end of constructor

MatrixK::MatrixK(const char* matrixName, std::vector<Double_t> EnuBinEdges, std::vector<Double_t> EeBinEdges)
   : fnumEnuBins(EnuBinEdges.size()-1), fnumEeBins(EeBinEdges.size()-1) {
//   fnumEnuBins = EnuBinEdges.size()-1;
//   fnumEeBins = EeBinEdges.size()-1;
   matrixHisto = new TH2D(matrixName, matrixName, fnumEnuBins, EnuBinEdges.data(), fnumEeBins, EeBinEdges.data());
   TString histname = TString(matrixName)+"_xsec";
   xsecHisto = new TH1D(histname, histname, fnumEnuBins, EnuBinEdges.data());    FIG8_ref_xsecHisto = new TH1D(TString(histname)+"_FIG8_Ref", TString(histname)+"_FIG8_Ref", fnumEnuBins, EnuBinEdges.data());


} // end of constructor w/ bin edges array

MatrixK::MatrixK(const char* matrixName, std::vector<Double_t> EnuBinEdges, int numEeBins, double Ee_min, double Ee_max)
   : fnumEnuBins(EnuBinEdges.size()-1), fnumEeBins(numEeBins) {
  
   // determine the bin edges for electron energy w/ info given
   std::vector<Double_t> EeBinEdges(numEeBins+1);
   double width = (Ee_max-Ee_min)/numEeBins;
   for(int i=0; i<=numEeBins; i++) EeBinEdges[i]=Ee_min+i*width;

   matrixHisto = new TH2D(matrixName, matrixName, fnumEnuBins, EnuBinEdges.data(), fnumEeBins, EeBinEdges.data());
   TString histname = TString(matrixName)+"_xsec";
   xsecHisto = new TH1D(histname, histname, fnumEnuBins, EnuBinEdges.data());
   
   FIG8_ref_xsecHisto = new TH1D(TString(histname)+"_FIG8_Ref", TString(histname)+"_FIG8_Ref", fnumEnuBins, EnuBinEdges.data());

}


MatrixK::~MatrixK(){ delete matrixHisto; delete xsecHisto; } 

double MatrixK::GetNumEeBins() {return fnumEeBins;}

double MatrixK::GetNumEnuBins() {return fnumEnuBins;}

void MatrixK::SetNumEeBins(int numEeBins) {fnumEeBins = numEeBins;}

void MatrixK::SetNumEnuBins(int numEnuBins) {fnumEnuBins = numEnuBins;}

TH2D* MatrixK::GetMatrixHisto(){ return matrixHisto;}   

void MatrixK::FillMatrixProj_XSecHisto(){
  // we need to add up the xsec for each column to get the total nu-e scattering xsec for a particular neutrino energy

  // can just take the projection, it should be easier and more correct
  xsecHisto = matrixHisto->ProjectionX(); 
//  double factor = 1e42* 3.894e-28;
  double factor = 3.89379e-28; // conversion from GeV^-2 to cm^2
//  xsecHisto->Scale(factor,"width");
//  double factor = 1.0;
  xsecHisto->Scale(factor);
  double binCenter; double val; 
//  xsecHisto->Scale(1, "width");



  for (int b=1; b <= xsecHisto->GetNbinsX(); b++){
     binCenter = xsecHisto->GetBinCenter(b);
     if (binCenter == 0) { binCenter = 0.0001; }
     val = (xsecHisto->GetBinContent(b)) / binCenter;
     xsecHisto->SetBinContent(b, val);
  }


/*
  int globalBin;
  double val = 0.0; double factor = 1; 
  for (int x=1; x <= fnumEnuBins; x++){
     for (int y=1; y <= fnumEeBins; y++){
        globalBin = matrixHisto->GetBin(x,y);
          factor = 1.0;
//        factor = 1e42* 3.894e-28 / xsecHisto->GetBinCenter(x);
//          factor = 3.894e-28 / xsecHisto->GetBinWidth(x);
//        factor = 3.894e-28; 
//        factor = 3.89379e-28; // conversion from GeV^-2 to cm^2
        val = val + matrixHisto->GetBinContent(globalBin)*factor;
        xsecHisto->SetBinContent(x, val);
     }
  }
*/

} // end FillMatrixProj_XSecHisto

TH1D* MatrixK::GetMatrixProj_XSecHisto(){ return xsecHisto;}

void MatrixK::PopulateMatrixHisto(int pdg, RadCorrTomalak* radCorr, int N){
  int globalBin; double Enu; double ymin; double ymax; double val;
  int globalBin_OF; double content; double content_OF;
  double Emin, Emax; // min and max total electron energy

  // we explicitly loop from 0 to bins+1 so that we are populating the underflow and overflow of the xsec matrix for both neutrino energy bins and electron energy bins
  for (int x=0; x <= fnumEnuBins+1; x++){
     // use the bin center for the Enu value
     Enu = matrixHisto->GetXaxis()->GetBinCenter(x);
     for (int y=0; y <= fnumEeBins+1; y++){
        globalBin = matrixHisto->GetBin(x,y);
        Emin = matrixHisto->GetYaxis()->GetBinLowEdge(y);
        Emax = matrixHisto->GetYaxis()->GetBinUpEdge(y);
        ymin = GetY(Emin, Enu);
        ymax = GetY(Emax, Enu);

        // only integrate for the y values within the physical bounds: between 0 and 1:
        if (ymin >=1 || ymax <= 0) {val = 0.0;}
        else {
           ymin = std::max(ymin, 0.0);
           ymax = std::min(1.0, ymax);
           val = IntegrateDSigmaDY(pdg, Enu, ymin, ymax, radCorr, N);
        }

        matrixHisto->SetBinContent(globalBin, val);
        matrixHisto->SetBinError(globalBin, 0.0);
     } // end looping over EeBins (ybins)
  } // end looping over EnuBins (xbins)

  // now, we want to add the Ee bin overflow to the 9-20GeV Electron energy bin
  for (int x=0; x <= fnumEnuBins+1; x++){ // looping over all Enu bins, including OF and UF
     globalBin = matrixHisto->GetBin(x,fnumEeBins);
     globalBin_OF = matrixHisto->GetBin(x,fnumEeBins+1);

     content = matrixHisto->GetBinContent(globalBin);
     content_OF = matrixHisto->GetBinContent(globalBin_OF);
     matrixHisto->SetBinContent(globalBin, content+content_OF);
     matrixHisto->SetBinContent(globalBin_OF, 0.0); 
  }


} // end PopulateMatrixHisto



