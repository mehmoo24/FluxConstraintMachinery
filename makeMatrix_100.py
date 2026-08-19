# We want to make a standard flux plot with the CV flux and the RMS
import math
import ROOT
from ROOT import gROOT
from ROOT import gStyle
from ROOT import TGaxis, gPad, TLine
from ROOT import TCanvas, TFile, TProfile, TNtuple, TH1F, TH2F, TColor, TLegend
import os,sys
from ROOT import PlotUtils
from PlotUtils import MnvH2D
from PlotUtils import MnvH1D
ROOT.gROOT.SetBatch(True) # to prevent canvas from showing up interctively
mnv = PlotUtils.MnvPlotter()
from array import array


file_input = sys.argv[1] # MUST BE NEUTRINO
file_histo = sys.argv[2] 
title = sys.argv[3]
file_inputB = sys.argv[4] # MUST BE ANTINEUTRINO
vertErrorBandName = sys.argv[5]

file_A = ROOT.TFile.Open(file_input)
flux_A = file_A.Get(file_histo)

nbins = flux_A.GetNbinsX()
x_min = flux_A.GetXaxis().GetXmin()
x_max = flux_A.GetXaxis().GetXmax()

file_B = ROOT.TFile.Open(file_inputB)
flux_B = file_B.Get(file_histo) # assuming flux histo will be named the same in both files


bins_A = flux_A.GetXaxis().GetXbins()
bins_B = flux_B.GetXaxis().GetXbins()

print(len(bins_A))

bins_array = array('d', bins_A)

cov_matrix = ROOT.PlotUtils.MnvH2D("Cov", "Cov", flux_A.GetNbinsX(), bins_array, flux_A.GetNbinsX(), bins_array)
corr_matrix = ROOT.PlotUtils.MnvH2D("Corr", "Corr", flux_A.GetNbinsX(), bins_array, flux_A.GetNbinsX(), bins_array)

cov_matrix_fluxA = ROOT.PlotUtils.MnvH2D("Cov_A", "Cov_A", flux_A.GetNbinsX(), bins_array, flux_A.GetNbinsX(), bins_array)
cov_matrix_fluxB = ROOT.PlotUtils.MnvH2D("Cov_B", "Cov_B", flux_A.GetNbinsX(), bins_array, flux_A.GetNbinsX(), bins_array)

#cov_matrix = ROOT.PlotUtils.MnvH2D("Cov", "Cov", nbins, x_min, x_max, nbins, x_min, x_max)
#corr_matrix = ROOT.PlotUtils.MnvH2D("Corr", "Corr", nbins, x_min, x_max, nbins, x_min, x_max)
#cov_matrix_fluxA = ROOT.PlotUtils.MnvH2D("Cov_A", "Cov_A", nbins, x_min, x_max, nbins, x_min, x_max)
#cov_matrix_fluxB = ROOT.PlotUtils.MnvH2D("Cov_B", "Cov_B", nbins, x_min, x_max, nbins, x_min, x_max)

#num_univ = 1000
num_univ = 100
#num_univ = 3

bandA = flux_A.GetVertErrorBand(vertErrorBandName)
bandB = flux_B.GetVertErrorBand(vertErrorBandName)
for x in range(1, nbins+1):
    for y in range(1, nbins+1):
        val = 0.
        valA = 0.
        valB = 0.
        for univ in range(0,num_univ):
            A_u = bandA.GetHist(univ).GetBinContent(x)
            A_CV = flux_A.GetCVHistoWithStatError().GetBinContent(x)
            B_u = bandB.GetHist(univ).GetBinContent(y)
            B_CV = flux_B.GetCVHistoWithStatError().GetBinContent(y)
            val = val + ((A_u - A_CV) * (B_u - B_CV))

            # flux A cov matrix
            A_uy = bandA.GetHist(univ).GetBinContent(y)
            A_CVy = flux_A.GetCVHistoWithStatError().GetBinContent(y)
            valA = valA + ((A_u - A_CV) * (A_uy - A_CVy))

            # flux B cov matrix
            B_ux = bandB.GetHist(univ).GetBinContent(x)
            B_CVx = flux_B.GetCVHistoWithStatError().GetBinContent(x)
            valB = valB + ((B_ux - B_CVx) * (B_u - B_CV))

        val = val / num_univ
        valA = valA / num_univ
        valB = valB / num_univ
        cov_matrix.SetBinContent(x,y,val)
        cov_matrix_fluxA.SetBinContent(x,y,valA)
        cov_matrix_fluxB.SetBinContent(x,y,valB)


# normalizing to get correlation
for x in range(1, nbins+1):
    for y in range(1, nbins+1):
        var_A = cov_matrix_fluxA.GetBinContent(x,x)
        var_B = cov_matrix_fluxB.GetBinContent(y,y)
        if not(var_A * var_B == 0):
            value = cov_matrix.GetBinContent(x,y) / math.sqrt(var_A * var_B)
        else:
            value = 0.0
        corr_matrix.SetBinContent(x,y,value)
        



print(type(cov_matrix))
print("yes!")
file_output = ROOT.TFile("cov_matrix_"+title+".root", "RECREATE")
file_output.cd()
cov_matrix.Write("cross_cov_matrix", ROOT.TObject.kOverwrite)
corr_matrix.Write("cross_corr_matrix", ROOT.TObject.kOverwrite)
cov_matrix_fluxA.Write("self_cov_matrix_neutrino")
cov_matrix_fluxB.Write("self_cov_matrix_antineutrino")
file_output.Close()

c = ROOT.TCanvas("c", "c", 1000, 1000)
corr_matrix.SetTitle("Corr Matrix: "+title)
corr_matrix.GetXaxis().SetTitle("Electron Energy [GeV]")
corr_matrix.GetYaxis().SetTitle("Electron Energy [GeV]")
corr_matrix.Draw("COLZ")

print("nbins: "+str(nbins))

c.Update()
c.SaveAs("Corr_"+title +".png")
