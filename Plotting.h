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
    //header = "hist_ScaleFactor_PILEUP", "Scale Factor for PILEUP; ; Events";
  }

  if(name_plot=="SF_BTAG"){
    SF_hist = hist_ScaleFactor_BTAG;
    //header = "hist_ScaleFactor_BTAG", "Scale Factor for BTAG; ; Events";
  }

  if(name_plot=="SF_ELE"){
    SF_hist = hist_ScaleFactor_ELE;
    //header = "hist_ScaleFactor_ELE", "Scale Factor for ELE; ; Events";
  }

  if(name_plot=="SF_MUON"){
    SF_hist = hist_ScaleFactor_MUON;
    //header = "hist_ScaleFactor_MUON", "Scale Factor for MUON; ; Events";
  }

  if(name_plot=="SF_PHOTON"){
    SF_hist = hist_ScaleFactor_PHOTON;
    //header = "hist_ScaleFactor_PHOTON", "Scale Factor for PHOTON; ; Events";
  }

  if(name_plot=="SF_TAU"){
    SF_hist = hist_ScaleFactor_TAU;
    //header = "hist_ScaleFactor_TAU", "Scale Factor for TAU; ; Events";
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
  
  TH1F *data_hist = new TH1F();
  TH1F *ttbar_hist = new TH1F();
  TString name_image = "distribution_plots/"+name_plot+".png";
  bool log_scale = false;
  TString header = "";
  
  if(name_plot=="met"){
    data_hist = data_hist_met;
    ttbar_hist = ttbar_hist_met;
    //header = "MET Comparison; MET [GeV]; Events";
  }

  if(name_plot=="lep_pt"){
    data_hist = data_hist_lep_pt;
    ttbar_hist = ttbar_hist_lep_pt;
    //header = "p_{T} distribution of the dilepton system; p_{T} [GeV]; Events";
    log_scale = true;
  }

  if(name_plot=="lep_eta"){
    data_hist = data_hist_lep_eta;
    ttbar_hist = ttbar_hist_lep_eta;
    //header = "|#eta| distribution of the dilepton system; |#eta_{e#mu}|; Events";
    //log_scale = true;
  }
    
  data_hist->SetMarkerStyle(20);
  data_hist->SetMarkerColor(kBlack);
  data_hist->SetStats(0);
  
  ttbar_hist->SetFillColorAlpha(kOrange, 0.45);
  ttbar_hist->SetStats(0);

  // ************************************
  ///// Plotting
  // ************************************
  
  TCanvas *can = new TCanvas("can","", 800, 600);

  can->Divide(1, 2);
  
  THStack* hstack = new THStack("hs", header);
  TLegend *leg = new TLegend(0.7, 0.70, 0.85, 0.85);
  // Adjust the pads for better visuals
  TPad* pad0 = (TPad*)can->cd(1);
  //pad0->SetPad(0.0, 0.3, 1.0, 1.0);  // Upper pad
  pad0->SetPad(0.0, 0.29, 1.0, 1.0);  // Upper pad
  //pad0->SetBottomMargin(0.02);       // Reduce bottom margin
 
  //  pad0->SetTickx(false);
  //  pad0->SetTicky(false);
  pad0->SetTopMargin(0.05);
  pad0->SetBottomMargin(0);
  pad0->SetLeftMargin(0.14);
  pad0->SetRightMargin(0.05);
  pad0->SetFrameBorderMode(0);
  pad0->SetTopMargin(0.06);
 
  
  TPad* pad1 = (TPad*)can->cd(2);

  pad1->SetPad(0.0, 0.0, 1.0, 0.29);  // Lower pad
  //pad1->SetTickx(false);
  //pad1->SetTicky(false);
  pad1->SetTopMargin(0.0); // Reduce top margin
  pad1->SetBottomMargin(0.5); // Increase bottom margin for axis labels
  pad1->SetLeftMargin(0.14);
  pad1->SetRightMargin(0.05);
  pad1->SetFrameBorderMode(0);
  
  // Draw the histograms on the upper pad
  pad0->cd();

  hstack->Add(ttbar_hist);
  hstack->SetMaximum(2.5*hstack->GetMaximum());
  if(log_scale==true){
    pad0->SetLogy();
    hstack->SetMinimum(0.1);
    hstack->SetMaximum(100*hstack->GetMaximum());
  }
  
  if(log_scale==false){
    hstack->SetMaximum(2.0*hstack->GetMaximum());
  }
  
  // statistical error histogram
  TH1F *h_err = (TH1F*) ttbar_hist->Clone();

  h_err->SetFillStyle(3004);
  //h_err->SetFillStyle(3354);
  h_err->SetFillColor(kGray+1);
  h_err->SetLineColor(kGray+1);
  h_err->SetMarkerStyle(1);

  hstack->SetMinimum(0.1);
  hstack->Draw("hist");
  //  h_err->Draw("e2same");
  h_err->Draw("e3same");
  data_hist->Draw("epsame");
    
  leg->AddEntry(data_hist, "Data", "lep");
  leg->AddEntry(ttbar_hist, "t#bar{t}","f");
  leg->AddEntry(h_err, "Stat. unc.","f");
  leg->SetBorderSize();
  leg->Draw();

  ATLASLabel(0.2, 0.85, "Work in Progress", kBlack);
  double dely = 0.05;
  string lumi_str = to_string(lumi*fraction);
  
  myText(0.2, 0.85-dely, kBlack, "#sqrt{s} = 13 TeV, 36 fb^{-1}");
  myText(0.2, 0.85-2*dely, kBlack, name_plot+" distribution");
  //myText(0.2, 0.*8-dely, kBlack, "for class: "+label_leg);
  //myText(0.2, 0.8-dely, kBlack, name_plot);

  // Draw the ratio plot on the lower pad
  pad1->cd();
  TH1F* h_mc_sum = dynamic_cast<TH1F*>(hstack->GetStack()->Last());
  TH1F* h_ratio = (TH1F*)data_hist->Clone("h_ratio");
  h_ratio->Divide(h_mc_sum);  // Get the ratio between data and the summed histogram from the stack

  h_ratio->SetMinimum(0);
  h_ratio->SetMaximum(2.2);
    
  h_ratio->SetLineColor(kBlack);
  h_ratio->SetMarkerStyle(20);
  h_ratio->SetTitle("");  // Remove title
  
  // Set Y-axis title and range
  h_ratio->GetYaxis()->SetTitle("Data/Pred");
  h_ratio->GetYaxis()->SetNdivisions(505);  // 5 divisions for better look
  h_ratio->GetYaxis()->SetTitleSize(0.12);
  h_ratio->GetYaxis()->SetTitleOffset(0.45);
  h_ratio->GetYaxis()->SetLabelSize(0.10);
  h_ratio->GetYaxis()->SetLabelOffset(0.01);
  
  // Set X-axis labels
  h_ratio->GetXaxis()->SetTitle(data_hist->GetXaxis()->GetTitle());
  h_ratio->GetXaxis()->SetTitleSize(0.15);
  h_ratio->GetXaxis()->SetLabelSize(0.13);
  h_ratio->GetXaxis()->SetTitleOffset(1.2);
  h_ratio->GetXaxis()->SetLabelOffset(0.035);

  
  h_ratio->Draw("EP");
  
  // Draw a horizontal line at ratio = 1
  TLine* line = new TLine(h_ratio->GetXaxis()->GetXmin(), 1, h_ratio->GetXaxis()->GetXmax(), 1);
  line->SetLineStyle(1);  // Dashed line 2
  line->Draw();

    
  can->Draw();
  can->Print(name_image);
}
