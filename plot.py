import os,sys
import ROOT
import PlotUtils

ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetTextFont(42)

LO = ROOT.TFile.Open("/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/LeadingOrder_noRadCorr.root", "READ")

NLO = ROOT.TFile.Open("/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/star.root", "READ")

FIG8 = ROOT.TFile.Open("/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/FIG8_EmailNumbers.root", "READ")

FIG8_Updated = ROOT.TFile.Open("/exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/FIG8_UpdatedEmailNumbers.root", "READ")

#histo_LO = LO.Get("kXSecMatrix_UFOF_XsecProj_PDG14")
#histo_NLO = NLO.Get("kXSecMatrix_UFOF_XsecProj_PDG14")
#histo_FIG8 = FIG8.Get("numu")
#histo_FIG8_Updated = FIG8_Updated.Get("numu")

#histo_LO = LO.Get("kXSecMatrix_UFOF_XsecProj_PDGminus14")
#histo_NLO = NLO.Get("kXSecMatrix_UFOF_XsecProj_PDGminus14")
#histo_FIG8 = FIG8.Get("numubar")
#histo_FIG8_Updated = FIG8_Updated.Get("numubar")

#histo_LO = LO.Get("kXSecMatrix_UFOF_XsecProj_PDG12")
#histo_NLO = NLO.Get("kXSecMatrix_UFOF_XsecProj_PDG12")
#histo_FIG8 = FIG8.Get("nue")
#histo_FIG8_Updated = FIG8_Updated.Get("nue")

histo_LO = LO.Get("kXSecMatrix_UFOF_XsecProj_PDGminus12")
histo_NLO = NLO.Get("kXSecMatrix_UFOF_XsecProj_PDGminus12")
histo_FIG8 = FIG8.Get("nuebar")
histo_FIG8_Updated = FIG8_Updated.Get("nuebar")


c = ROOT.TCanvas("c", "c", 800, 600)
c.cd()
c.SetGrid()
histo_LO.Scale(1e42)
histo_NLO.Scale(1e42) # the FIG8 doesn't have the scaling on it, so on the axis just label that it's 1e-42 

histo_LO.GetXaxis().SetTitle("Neutrino Energy [GeV]")
histo_LO.GetYaxis().SetTitle("Total Cross-Section 10^{42}, cm^{2}/GeV")
histo_LO.GetXaxis().SetRangeUser(0,7)
#histo_LO.GetYaxis().SetRangeUser(1.55, 1.65) #numu
#histo_LO.GetYaxis().SetRangeUser(1.3, 1.4) #numubar
#histo_LO.GetYaxis().SetRangeUser(9, 9.7) #nue
histo_LO.GetYaxis().SetRangeUser(3.8, 3.98) #nuebar

histo_LO.SetLineWidth(2)
histo_LO.SetLineColor(ROOT.kBlack)
histo_LO.Draw("HIST")

histo_NLO.SetLineWidth(2)
histo_NLO.SetLineColor(ROOT.kBlue)
histo_NLO.Draw("HIST SAME")


histo_FIG8.SetLineWidth(2)
histo_FIG8.SetLineColor(ROOT.kRed)
histo_FIG8.Draw("PL SAME")

histo_FIG8_Updated.SetLineWidth(2)
histo_FIG8_Updated.SetLineColor(ROOT.kGreen)
histo_FIG8_Updated.Draw("PL SAME")

#leg = ROOT.TLegend(0.2, 0.2, 0.6, 0.4)
leg = ROOT.TLegend(0.2, 0.6, 0.6, 0.8)
leg.AddEntry(histo_LO, "LO Cross-Section", "l")
leg.AddEntry(histo_NLO, "NLO Cross-Section", "l")
leg.AddEntry(histo_FIG8, "FIG8 Cross-Section PRD2020", "pl")
leg.AddEntry(histo_FIG8_Updated, "FIG8 Cross-Section Update in QCD", "pl")
leg.Draw("SAME")

print(histo_FIG8.GetN())
print(histo_FIG8_Updated.GetN())

#c.SaveAs("numu.png")
#c.SaveAs("numubar.png")
#c.SaveAs("nue.png")
c.SaveAs("nuebar.png")

