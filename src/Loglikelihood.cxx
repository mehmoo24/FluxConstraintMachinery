#include <TH1D.h>
#include "PlotUtils/MnvH1D.h"
#include "../include/Loglikelihood.h"
#include <cmath>
#include <ostream>

double CalculateNormLogLikelihood(TH1D* measured, TH1D* expected){
double total_measured = 0.0;
double total_expected = 0.0;

// loop over the bins and get total # of events
for (int bin=1; bin<expected->GetNbinsX()+1; bin++){
   total_measured = total_measured + measured->GetBinContent(bin);
   total_expected = total_expected + expected->GetBinContent(bin);
}

double norm_log = 0.0;
if (total_expected != 0) norm_log = total_measured*log(total_expected) - total_expected -lgamma(total_measured + 1);

return norm_log;
}

long long factorial(int num){
   long long result = 1;
   // loop over the number
   for (int i = num; i>=1; i--){
   result = result * i;
   }
   return result;
}

double SumOfBins(TH1D* univ){
double val = 0.0;
for (int bin=1; bin<univ->GetNbinsX()+1; bin++){
  val = val + univ->GetBinContent(bin);
}
std::cout<<"Here: " << val << std::endl;
return val;
}

double CalculateTotalLogLikelihood(TH1D* measured, TH1D* univ){
// we are getting the total log-likelihood for one universe and returning it

double total = 0.0;
double n; double u; double val;
// loop over the bins
for (int bin=1;bin<measured->GetNbinsX()+1;bin++){
   // get the measured val and the expected val for that  bin
   n = measured->GetBinContent(bin);
   u = univ->GetBinContent(bin);
   // connect them in the log relationship
   // add in checks 
   if (u==0) {val = 0;}
   else {val = n*log(u) - u - lgamma(n+1);}
   // add the result to the running result since want the total log-likelihood across the bins
   total = total + val;
}
// return the total result
return total;

}


