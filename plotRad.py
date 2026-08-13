import ROOT
import PlotUtils
import os,sys

ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetTextFont(42)

f = sys.argv[1]
energy = sys.argv[2] # input the specific enu that the rad correction is for

file = ROOT.TFile(f, "READ")

numu = file.Get("numu_rad")
numubar = file.Get("numubar_rad")
nue = file.Get("nue_rad")
nuebar = file.Get("nuebar_rad")


can = ROOT.TCanvas("can", "can", 900, 600)
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

min = a
if min>b:
   min = b
if min>c:
   min = c
if min>d:
   min = d

numu.GetYaxis().SetRangeUser(-0.067,max)
numu.GetXaxis().SetRangeUser(0,1)
numu.SetTitle("")

numu.GetXaxis().SetTitle("y = T (Electron Kinetic Energy) /"+energy+" GeV (Enu)")
numu.GetYaxis().SetTitle("Radiative Correction")

numu.Draw("HIST")
numubar.Draw("HIST SAME")
nue.Draw("HIST SAME")
nuebar.Draw("HIST SAME")



leg = ROOT.TLegend(0.15,0.15,0.45, 0.4)
leg.AddEntry(numu, "Muon Neutrinos", "l")
leg.AddEntry(numubar, "Muon Antineutrinos", "l")
leg.AddEntry(nue, "Electron Neutrinos", "l")
leg.AddEntry(nuebar, "Electron Antineutrinos", "l")
leg.SetTextSize(0.03)
leg.Draw("SAME")

can.SaveAs("RadCorr_Enu"+energy+"GeV"+".png")
