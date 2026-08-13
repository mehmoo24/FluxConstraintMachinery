import ROOT
import PlotUtils
import os,sys

ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetTextFont(42)

f = sys.argv[1]
output_name = sys.argv[2]

file = ROOT.TFile(f, "READ")

numu = file.Get("flux_E_cvweighted_numu")
numubar = file.Get("flux_E_cvweighted_numubar")
nue = file.Get("flux_E_cvweighted_nue")
nuebar = file.Get("flux_E_cvweighted_nuebar")


can = ROOT.TCanvas("can", "can", 800, 600)
numu.SetLineWidth(2)
numubar.SetLineWidth(2)
nue.SetLineWidth(2)
nuebar.SetLineWidth(2)

numu.SetLineColor(ROOT.kBlue)
numubar.SetLineColor(ROOT.kBlue-3)
nue.SetLineColor(ROOT.kRed)
nuebar.SetLineColor(ROOT.kRed-3)

numu.GetXaxis().SetRangeUser(0,20)

# get max y value
a = numu.GetMaximum()
b = numubar.GetMaximum()
c = nue.GetMaximum()
d = nuebar.GetMaximum()

max = a
if b>max:
   max = b

if c>max:
   max = c

if d>max:
   max = d

numu.GetYaxis().SetRangeUser(0,max)

numu.Draw("HIST")
numubar.Draw("HIST SAME")
nue.Draw("HIST SAME")
nuebar.Draw("HIST SAME")



leg = ROOT.TLegend(0.55,0.55,0.85, 0.8)
leg.AddEntry(numu, "Muon Neutrinos", "l")
leg.AddEntry(numubar, "Muon Antineutrinos", "l")
leg.AddEntry(nue, "Electron Neutrinos", "l")
leg.AddEntry(nuebar, "Electron Antineutrinos", "l")
leg.SetTextSize(0.03)
leg.Draw("SAME")

can.SaveAs(output_name+".png")
