#include <cmath>
#include <stdexcept>
#include "TMath.h"
#include <iostream>
#include "../include/NuECrossSection.h"
#include "../include/RadCorrectionTomalak.h"

double GetY(double Ee, double Enu){
   // Returns the y value corresponding to particular electron energy and neutrino energy, total electron energy being used
   return (Ee - kElectronMass) / Enu; // kinetic energy over neutrino energy
}

Couplings GetCouplings(int pdg){
   Couplings c;
   if(std::abs(pdg) == 14){
     c.cLL = -0.2723;
   }
   else if(std::abs(pdg) == 12) c.cLL = 0.7277;
   else throw std::runtime_error("Unsupported PDG!");
   c.cLR = 0.2277;
   c.isAntinu = (pdg < 0);
  
   return c;
}


Couplings GetCouplings_Tomalak(int pdg){
   Couplings c;
   if(std::abs(pdg) == 14){
     c.cLL = -0.90084e-5;
   }
   else if(std::abs(pdg) == 12) c.cLL = 2.39818e-5;
   else throw std::runtime_error("Unsupported PDG!");
   c.cLR = 0.76911e-5;
   c.isAntinu = (pdg < 0);

   return c;
}

Couplings GetCouplings_OneLoop(int pdg){
   Couplings c; // corresponding dimensionless quantities from tomalak and hill paper
   if(std::abs(pdg) == 14){
     c.cLL = -0.2731;
   }
   else if(std::abs(pdg) == 12) c.cLL = 0.7271;
   else throw std::runtime_error("Unsupported PDG!");
   c.cLR = 0.2331;
   c.isAntinu = (pdg < 0);
  
   return c;
}

double GetMandelstamInvariant_s(double Enu){
   return (kElectronMass*kElectronMass + 2*kElectronMass*Enu);
} // end GetMandelstamInvariant_s

/*
double GetRadCorr_Tomalak(int pdg, double Enu, double y){
   double rad_corr=0;
   // muon neutrinos
   if (pdg == 14){
      rad_corr = (-0.695 + (-0.184662)*std::log(Enu)) +
                 (-11.529 + (0.5385)*std::log(Enu))*y +
                 (69.661 + (-3.7371)*std::log(Enu))*y*y + 
                 (-217.52 + (12.4729)*std::log(Enu))*y*y*y + 
                 (349.09 + (-21.3760)*std::log(Enu))*y*y*y*y + 
                 (-276.14 + (18.0406)*std::log(Enu))*y*y*y*y*y + 
                 (85.294 + (-5.9333)*std::log(Enu))*y*y*y*y*y*y;
   }
   rad_corr = rad_corr / 100.0; // return the correction in percent
   return rad_corr;


} // end GetRadCorr_Tomalak
*/


double DSigmaDY_Tomalak(int pdg, double Enu, double y, RadCorrTomalak* radCorr){
   Couplings c = GetCouplings_Tomalak(pdg);
//   std::cout << c.cLL << " " << c.cLR << std::endl;
   double dSigma_dy;
   double factor = kElectronMass*Enu/(4.0*kPi);   

   if (y < 0.0 || y > 1.0) throw std::runtime_error("y must be between 0 and 1, physically y = T_e/Enu");
//   if (Enu <= 0.0) throw std::runtime_error("Enu must be positive");
   if (Enu <= 0.0) return 0;     
 
   if (!c.isAntinu)
     dSigma_dy = factor * (c.cLL*c.cLL + (c.cLR*c.cLR)*((1-y)*(1-y)) - c.cLL*c.cLR*kElectronMass*y/Enu);
   else // antineutrino case LOOK INTO THE ANTINEUTRINO CASE
     dSigma_dy = factor * (c.cLR*c.cLR + (c.cLL*c.cLL)*((1-y)*(1-y)) - c.cLR*c.cLL*kElectronMass*y/Enu);

      // putting in the radiative correction
  if (y == 0) y=1e-10;

//   double rad_corr = -0.228*std::log(y) - 0.158*std::log(Enu) - 1.872;
//   rad_corr = rad_corr / 100.0;
   double rad_corr = 0.0;

   rad_corr = radCorr->GetRadCorr_Tomalak(Enu, y);
   dSigma_dy = dSigma_dy + dSigma_dy * rad_corr;
   return dSigma_dy;

} // end DSigmaDY_Tomalak

double DSigmaDY(int pdg, double Enu, double y){
   Couplings c = GetCouplings(pdg);
   double s = GetMandelstamInvariant_s(Enu);
   double factor = s * (kG_F * kG_F) / kPi;
//   double factor = 2 * (kG_F * kG_F) * kElectronMass * Enu / kPi;
   double dSigma_dy;
   if (y < 0.0 || y > 1.0) throw std::runtime_error("y must be between 0 and 1, physically y = T_e/Enu");
   if (Enu <= 0.0) throw std::runtime_error("Enu must be positive");

   if (!c.isAntinu)
     dSigma_dy = factor * (c.cLL*c.cLL + (c.cLR*c.cLR)*((1-y)*(1-y)) - c.cLL*c.cLR*kElectronMass*y/Enu);
   else // antineutrino case
     dSigma_dy = factor * (c.cLR*c.cLR + (c.cLL*c.cLL)*((1-y)*(1-y)) - c.cLR*c.cLL*kElectronMass*y/Enu);  


   // putting in the radiative correction
//   double rad_corr = -0.228*std::log(y) - 0.158*std::log(Enu) - 1.872;
   double rad_corr = 0.0;
   dSigma_dy = dSigma_dy + dSigma_dy * rad_corr; 
   return dSigma_dy;
} // end DSigmaDY

double DSigmaDY_Valencia(int pdg, double Enu, double y)
{
   Couplings c = GetCouplings_OneLoop(pdg); // get the one loop couplings
//   c = GetCouplings(pdg);
   double factor = (GetMandelstamInvariant_s(Enu) * (kG_F * kG_F)) / kPi;
   double dSigma_dy;
   if (y < 0.0 || y > 1.0) throw std::runtime_error("y must be between 0 and 1, physically y = T_e/Enu");
   if (Enu <= 0.0) throw std::runtime_error("Enu must be positive");

   // radiative corrections 
   // std::log(x) is natural log
   double X1 = (-2.0/3.0)*std::log(2.0*y*Enu/kElectronMass) + y*y/24.0 - 5.0*y/12.0 - kPi*kPi/6.0 + 23.0/72.0;
   double X2 = (-2.0/3.0)*std::log(2.0*y*Enu/kElectronMass) - (y*y)/18.0*(1.0-y)*(1.0-y) - kPi*kPi/6.0 - 2.0*y/(9.0*(1.0-y)*(1.0-y)) +23.0/(72.0*(1.0-y)*(1.0-y));
   double X3 = (-3.0/2.0)*std::log(2.0*y*Enu/kElectronMass) + (1.0/4.0) +3.0/(4.0*y) -3.0/(4.0*y*y) - kPi*kPi/6.0;

//    X1 = 0;
//    X2 = 0;
//    X3 = 0;

   if (!c.isAntinu)
     dSigma_dy = factor * (c.cLL*c.cLL*(1 + X1*kAlphaEM/kPi) + (c.cLR*c.cLR)*((1-y)*(1-y))*(1 + X2*kAlphaEM/kPi) - (c.cLL*c.cLR*kElectronMass*y/Enu)*(1 + X3*kAlphaEM/kPi));
   else // antineutrino case
     dSigma_dy = factor * (c.cLR*c.cLR*(1 + X1*kAlphaEM/kPi) + (c.cLL*c.cLL)*((1-y)*(1-y))*(1 + X2*kAlphaEM/kPi) - (c.cLR*c.cLL*kElectronMass*y/Enu)*(1 + X3*kAlphaEM/kPi));

   return dSigma_dy;

} // end DSigmaDY_Valencia

double IntegrateDSigmaDY(int pdg, double Enu, double ymin, double ymax, RadCorrTomalak* radCorr, int N){
  // Doing numerical integration using Simpson's rule
  if (N % 2 != 0) throw std::runtime_error("N must be even to use Simpson's rule!");

  double h = (ymax - ymin)/N;
//  double f_min = DSigmaDY(pdg, Enu, ymin);
//  double f_max = DSigmaDY(pdg, Enu, ymax);

  double f_min = DSigmaDY_Tomalak(pdg, Enu, ymin, radCorr);
  double f_max = DSigmaDY_Tomalak(pdg, Enu, ymax, radCorr);


  double sum_odd = 0.0;
  double sum_even = 0.0;

  for(int i=1; i<N; i++){
     // odd i
     if (i % 2 != 0) 
        sum_odd = sum_odd + 4*DSigmaDY_Tomalak(pdg, Enu, ymin+i*h, radCorr);
     else // even i
        sum_even = sum_even + 2*DSigmaDY_Tomalak(pdg, Enu, ymin+i*h, radCorr);
  }
  double result = h/3.0 * (f_min + f_max + sum_odd + sum_even);
  return result;

} // end IntegrateDSigmaDY
