// MatrixBuilder.h 

#ifndef MATRIXBUILDER_H
#define MATRIXBUILDER_H

#include <TH2D.h>

#include "../include/RadCorrectionTomalak.h"

class MatrixK{
public:
   MatrixK(const char* matrixName, int numEnuBins, double Enu_min, double Enu_max, int numEeBins, double Ee_min, double Ee_max);

   MatrixK(const char* matrixName, std::vector<Double_t> EnuBinEdges, std::vector<Double_t> EeBinEdges);

   MatrixK(const char* matrixName, std::vector<Double_t> EnuBinEdges, int numEeBins, double Ee_min, double Ee_max);

   ~MatrixK(); // destructor

   double GetNumEeBins(); 
   double GetNumEnuBins();

   void SetNumEeBins(int numEeBins);
   void SetNumEnuBins(int numEnuBins);

   TH2D* GetMatrixHisto();
   TH1D* GetMatrixProj_XSecHisto();

   void PopulateMatrixHisto(int pdg,RadCorrTomalak* radCorr, int N=1000); 
   void FillMatrixProj_XSecHisto();

private:
   int fnumEeBins;
   int fnumEnuBins;

   TH2D* matrixHisto;

   TH1D* xsecHisto;

   TH1D* FIG8_ref_xsecHisto;

}; // end class MatrixK






#endif



