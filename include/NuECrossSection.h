// NuECrossSection.h

#ifndef NUECROSSSECTION_H
#define NUECROSSSECTION_H

#include "RadCorrectionTomalak.h"

// Constants
constexpr double kElectronMass = 0.000511; // GeV
constexpr double kG_F = 1.1663787E-5; // GeV^-2, fermi coupling constant
constexpr double kPi = 3.14159265358979323846;
constexpr double kAlphaEM = 0.007297;

struct Couplings{
  double cLL;
  double cLR;
  bool isAntinu; 
};

// Function Headers
double GetY(double Ee, double Enu); // Ee and Enu in GeV

Couplings GetCouplings(int pdg);

Couplings GetCouplings_Tomalak(int pdg);

Couplings GetCouplings_OneLoop(int pdg);

double GetMandelstamInvariant_s(double Enu); // Enu in GeV


double DSigmaDY(int pdg, double Enu, double y); // Enu in GeV

double DSigmaDY_Valencia(int pdg, double Enu, double y); // Enu in GeV // func has the radiative correction put in

double DSigmaDY_Tomalak(int pdg, double Enu, double y, RadCorrTomalak* radCorr); // Enu in GeV

//double GetRadCorr_Tomalak(int pdg, double Enu, double y); // Enu in GeV

double f(int pdg, double Enu, double y); 

double IntegrateDSigmaDY(int pdg, double Enu, double ymin, double ymax, RadCorrTomalak* radCorr, int N=10); // for Simpson's rule, N must be even! Enu in GeV

#endif


