//Loglikelihood.h
#ifndef LOGLIKELIHOOD_H
#define LOGLIKELIHOOD_H


long long factorial(int num);

double CalculateNormLogLikelihood(TH1D* measured, TH1D* expected);

double CalculateTotalLogLikelihood(TH1D* measured, TH1D* univ);

double SumOfBins(TH1D* univ);





#endif
