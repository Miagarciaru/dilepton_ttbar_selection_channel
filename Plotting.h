#include "AtlasStyle/AtlasStyle.C"
#include "AtlasStyle/AtlasUtils.C"
#include "AtlasStyle/AtlasLabels.C"
#include "declaration_of_variables.h"

void plot_scale_factors(TString name_plot);
void plot_distributions_comparison(TString name_plot);


void plot_scale_factors(TString name_plot){

  SetAtlasStyle();
  
  //TLegend *leg = new TLegend(0.7, 0.75, 0.85, 0.85);
  TH1F *SF_hist = new TH1F();
  
  TString name_image = "distribution_plots/"+name_plot+".png";
  //bool log_scale = false;
  TString header = "";
  
  if(name_plot=="SF_PILEUP"){
    SF_hist = hist_ScaleFactor_PILEUP;
    header = "hist_ScaleFactor_PILEUP", "Scale Factor for PILEUP; ; Events";
  }

  if(name_plot=="SF_BTAG"){
    SF_hist = hist_ScaleFactor_BTAG;
    header = "hist_ScaleFactor_BTAG", "Scale Factor for BTAG; ; Events";
  }

  if(name_plot=="SF_ELE"){
    SF_hist = hist_ScaleFactor_ELE;
    header = "hist_ScaleFactor_ELE", "Scale Factor for ELE; ; Events";
  }

  if(name_plot=="SF_MUON"){
    SF_hist = hist_ScaleFactor_MUON;
    header = "hist_ScaleFactor_MUON", "Scale Factor for MUON; ; Events";
  }

  if(name_plot=="SF_PHOTON"){
    SF_hist = hist_ScaleFactor_PHOTON;
    header = "hist_ScaleFactor_PHOTON", "Scale Factor for PHOTON; ; Events";
  }

  if(name_plot=="SF_TAU"){
    SF_hist = hist_ScaleFactor_TAU;
    header = "hist_ScaleFactor_TAU", "Scale Factor for TAU; ; Events";
  }
  
  TCanvas *can = new TCanvas("can","", 800, 600);
  //  SF_hist->
  SF_hist->Draw();
  ATLASLabel(0.55, 0.8, "Work in Progress", kBlack);
  myText(0.55, 0.75, kBlack, name_plot);
  //myText(0.2, 0.8-dely, kBlack, "for class: "+label_leg);
  //myText(0.2, 0.8-2*dely, kBlack, name_plot);

  can->SaveAs(name_image);
  
}


void plot_distributions_comparison(TString name_plot){

  SetAtlasStyle();
  
  TLegend *leg = new TLegend(0.7, 0.75, 0.85, 0.85);
  TH1F *data_hist = new TH1F();
  TH1F *ttbar_hist = new TH1F();
  TString name_image = "distribution_plots/"+name_plot+".png";
  bool log_scale = false;
  TString header = "";
  
  if(name_plot=="met"){
    data_hist = data_hist_met;
    ttbar_hist = ttbar_hist_met;
    header = "MET Comparison; MET [GeV]; Events";
  }

  if(name_plot=="lep_pt"){
    data_hist = data_hist_lep_pt;
    ttbar_hist = ttbar_hist_lep_pt;
    header = "p_{T} distribution of the dilepton system; p_{T} [GeV]; Events";
    log_scale = true;
  }

  if(name_plot=="lep_eta"){
    data_hist = data_hist_lep_eta;
    ttbar_hist = ttbar_hist_lep_eta;
    header = "|#eta| distribution of the dilepton system; |#eta_{e#mu}|; Events";
    //log_scale = true;
  }
    
  data_hist->SetMarkerStyle(20);
  data_hist->SetMarkerColor(kBlack);
  data_hist->SetStats(0);
  
  ttbar_hist->SetFillColorAlpha(kOrange, 0.45);
  ttbar_hist->SetStats(0);

  ///// Plotting
  
  TCanvas *can = new TCanvas("can","", 800, 600);
  
  THStack* hstack = new THStack("hs", header);

  hstack->Add(ttbar_hist);
  hstack->SetMaximum(2.5*hstack->GetMaximum());
  if(log_scale==true){
    can->SetLogy();
    hstack->SetMinimum(0.1);
    hstack->SetMaximum(100*hstack->GetMaximum());
  }
  
  if(log_scale==false){
    hstack->SetMaximum(2.0*hstack->GetMaximum());
  }
  
  // statistical error histogram
  TH1F *h_err = (TH1F*) ttbar_hist->Clone();

  h_err->SetFillStyle(3004);
  h_err->SetFillColor(kBlack);
  h_err->SetLineColor(0);
  h_err->SetMarkerStyle(1);

  hstack->Draw("hist");
  h_err->Draw("e2same");
  data_hist->Draw("epsame");
    
  leg->AddEntry(data_hist, "Data", "lep");
  leg->AddEntry(ttbar_hist, "t#bar{t}","f");
  leg->SetBorderSize();
  leg->Draw();

  ATLASLabel(0.2, 0.85, "Work in Progress", kBlack);
  double dely = 0.05;
  string lumi_str = to_string(lumi*fraction);
  
  myText(0.2, 0.85-dely, kBlack, "#sqrt{s} = 13 TeV, 36 fb^{-1}");
  myText(0.2, 0.85-2*dely, kBlack, name_plot+" distribution");
  //myText(0.2, 0.*8-dely, kBlack, "for class: "+label_leg);
  //myText(0.2, 0.8-dely, kBlack, name_plot);
     
  can->Draw();
  can->Print(name_image);
}
