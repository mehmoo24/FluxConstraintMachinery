import os,sys
import ROOT
import PlotUtils

ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetTextFont(42)

LO = ROOT.TFile.Open("/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/LeadingOrder_noRadCorr.root", "READ")

NLO = ROOT.TFile.Open("/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/star.root", "READ")


histo_LO = LO.Get("kXSecMatrix_UFOF_PDG12").Clone()
histo_NLO = NLO.Get("kXSecMatrix_UFOF_PDG12").Clone()

histo_LO.SetDirectory(0) # ownership not by any TFile
histo_NLO.SetDirectory(0) # ownership not by any TFile

#histo_LO.Divide(histo_NLO)

#for i in range(1,histo_LO.GetNbinsX()+1):
#   for j in range(1, histo_LO.GetNbinsY()+1):
#      bin = histo_LO.GetBin(i,j)
#      content = histo_NLO.GetBinContent(bin)
#      if content == 0: # if the denominator was 0, just set the bin content of the ratio in that bin to be 0
#         histo_LO.SetBinContent(bin, 0)


c = ROOT.TCanvas("c", "c", 800, 600)
c.cd()
#c.SetGrid()
#histo_LO.Scale(1e42)
#histo_NLO.Scale(1e42) # the FIG8 doesn't have the scaling on it, so on the axis just label that it's 1e-42 

histo_LO.GetXaxis().SetTitle("Neutrino Energy [GeV]")
histo_LO.GetYaxis().SetTitle("Electron Energy [GeV]")
#histo_LO.GetXaxis().SetRangeUser(0,7)
#histo_LO.GetYaxis().SetRangeUser(1.5, 1.6) #numu
#histo_LO.GetYaxis().SetRangeUser(1.3, 1.4) #numubar
#histo_LO.GetYaxis().SetRangeUser(9, 9.7) #nue
#histo_LO.GetYaxis().SetRangeUser(3.8, 3.98) #nuebar

histo_LO.GetZaxis().SetRangeUser(0.95,1.05)
histo_LO.Draw("COLZ")

#c.SaveAs("numu.png")
#c.SaveAs("numubar.png")
#c.SaveAs("nue.png")
c.SaveAs("mtx_ratio.png")

