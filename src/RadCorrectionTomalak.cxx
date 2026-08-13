#include "../include/RadCorrectionTomalak.h"

RadCorrTomalak::RadCorrTomalak(TString f1FilePath, TString f10FilePath){

   auto f1_Graph = new TGraph(f1FilePath, "%lg,%lg"); // reading floating point numbers from .csv file which is comma separated
   auto f10_Graph = new TGraph(f10FilePath, "%lg,%lg");

   // fit to a 6th order polynomial
//   f1_Graph->Fit("pol6", "R", "", 0, 7); //"R" tells the range, xmin, xmax
//   f10_Graph->Fit("pol6", "R", "", 0, 7);

   f1_Graph->Fit("pol6");
   f10_Graph->Fit("pol6");

   // get the fit function
   auto f1_fitFunc = f1_Graph->GetFunction("pol6");
   auto f10_fitFunc = f10_Graph->GetFunction("pol6");

   // get the fit parameters
   a0 = f1_fitFunc->GetParameter(0);
   a1 = f1_fitFunc->GetParameter(1);
   a2 = f1_fitFunc->GetParameter(2);
   a3 = f1_fitFunc->GetParameter(3);
   a4 = f1_fitFunc->GetParameter(4);
   a5 = f1_fitFunc->GetParameter(5);
   a6 = f1_fitFunc->GetParameter(6);

   b0 = f10_fitFunc->GetParameter(0);
   b1 = f10_fitFunc->GetParameter(1);
   b2 = f10_fitFunc->GetParameter(2);
   b3 = f10_fitFunc->GetParameter(3);
   b4 = f10_fitFunc->GetParameter(4);
   b5 = f10_fitFunc->GetParameter(5);
   b6 = f10_fitFunc->GetParameter(6);

} // end constructor


RadCorrTomalak::~RadCorrTomalak(){ } 

double RadCorrTomalak::GetRadCorr_Tomalak(double Enu, double y){
  double rad;
  rad = (a0 + (b0-a0)*std::log(Enu)/std::log(10)) + (a1 + (b1-a1)*std::log(Enu)/std::log(10))*y + (a2 + (b2-a2)*std::log(Enu)/std::log(10))*y*y + (a3 + (b3-a3)*std::log(Enu)/std::log(10))*y*y*y + (a4 + (b4-a4)*std::log(Enu)/std::log(10))*y*y*y*y + (a5 + (b5-a5)*std::log(Enu)/std::log(10))*y*y*y*y*y + (a6 + (b6-a6)*std::log(Enu)/std::log(10))*y*y*y*y*y*y;

  rad = rad / 100.0; // return in percent
  return rad;

} // end RadCorrTomalak::GetRadCorr_Tomalak
