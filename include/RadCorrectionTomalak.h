// RadCorrectionTomalak.h

#ifndef RADCORRTOMALAK_H
#define RADCORRTOMALAK_H

#include <TFile.h>
#include <TGraph.h>
#include <TF1.h>

class RadCorrTomalak{
   public:
      RadCorrTomalak(TString f1FilePath, TString f10FilePath);

      ~RadCorrTomalak(); // destructor

      double GetRadCorr_Tomalak(double Enu, double y); 


   private:
      double a0; double a1; double a2; double a3; double a4; double a5; double a6;
       double b0; double b1; double b2; double b3; double b4; double b5; double b6; 


}; // end class RadCorrTomalak

#endif
