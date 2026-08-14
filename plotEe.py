import ROOT
import PlotUtils
import os,sys

ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetTextFont(42)

f = sys.argv[1]
output_name = sys.argv[2] # input the specific enu that the rad correction is for

file = ROOT.TFile(f, "READ")

numu = file.Get("kEeDistr_PDG14")
numubar = file.Get("kEeDistr_PDGminus14")
nue = file.Get("kEeDistr_PDG12")
nuebar = file.Get("kEeDistr_PDGminus12")
total = file.Get("kEeDistr_TOTAL")


can = ROOT.TCanvas("can", "can", 900, 600)
numu.SetLineWidth(2)
numubar.SetLineWidth(2)
nue.SetLineWidth(2)
nuebar.SetLineWidth(2)
total.SetLineWidth(3)

numu.SetLineColor(ROOT.kBlue)
numubar.SetLineColor(ROOT.kBlue-3)
nue.SetLineColor(ROOT.kRed)
nuebar.SetLineColor(ROOT.kRed-3)
total.SetLineColor(ROOT.kBlack)



numu.GetYaxis().SetRangeUser(0,total.GetMaximum()+10)
numu.GetXaxis().SetRangeUser(0.8,7)
numu.SetTitle("")

numu.GetXaxis().SetTitle("Electron Energy [GeV]")
numu.GetYaxis().SetTitle("Number of Events")

numu.Draw("HIST")
numubar.Draw("HIST SAME")
nue.Draw("HIST SAME")
nuebar.Draw("HIST SAME")
total.Draw("HIST SAME")


leg = ROOT.TLegend(0.55,0.65,0.85, 0.85)
leg.AddEntry(total, "Total nu-e scattering events", "l")
leg.AddEntry(numu, "Muon Neutrinos", "l")
leg.AddEntry(numubar, "Muon Antineutrinos", "l")
leg.AddEntry(nue, "Electron Neutrinos", "l")
leg.AddEntry(nuebar, "Electron Antineutrinos", "l")
leg.SetTextSize(0.03)
leg.Draw("SAME")

can.SaveAs("EeDist_"+output_name+".png")
