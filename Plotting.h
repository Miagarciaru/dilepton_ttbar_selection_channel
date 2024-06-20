#include "AtlasStyle/AtlasStyle.C"
#include "AtlasStyle/AtlasUtils.C"
#include "AtlasStyle/AtlasLabels.C"

float luminosity = 36.0;
float fraction = 1.0;
float XSEC = 729.77;
float fil_eff = 0.1053695;
float k_factor = 1.13975636159;
float sumw = 4.20575e+10;
float scale_val = (k_factor*fil_eff*XSEC*luminosity*1000.0)/sumw;

// Function to get a list of histograms from a ROOT file
void GetHistogramsFromFiles(const std::vector<std::string>& files, const std::string& histName, std::vector<TH1F*>& dataHists,
			    std::vector<TH1F*>& mcHists){

  string path_folder = "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/validation/output_analysis_wo_SFPhotons/";

  cout << histName << endl;
  for(const auto& file : files){

    string path_root_file = path_folder+file+".root";
    TFile* f = TFile::Open(path_root_file.c_str());
    TH1F* hist = dynamic_cast<TH1F*>(f->Get(histName.c_str()));
    hist->SetDirectory(0);
    
    if(file.find("data") != std::string::npos){
      dataHists.push_back(hist);
    }
    else{
      mcHists.push_back(hist);
    }

    f->Close();
    
  }
}


// Function to plot a given histogram
void PlotHistogram(const std::string& histName, const std::vector<TH1F*>& dataHists, const std::vector<TH1F*>& mcHists) {

  SetAtlasStyle();

  bool log_scale = false;
  float scale_val = 0;
  
  TCanvas* can = new TCanvas(("c_" + histName).c_str(), histName.c_str(), 800, 600);
  THStack* hstack = new THStack(("hs_" + histName).c_str(), histName.c_str());

  // Create a combined data histogram
  TH1F* data_hist = nullptr;
  for(const auto& hist : dataHists){
    if(data_hist == nullptr){
      data_hist = dynamic_cast<TH1F*>(hist->Clone(("data_" + histName).c_str()));
      data_hist->SetDirectory(0); // Detach from file
    }
    else{
      data_hist->Add(hist);
    }
  }

  data_hist->SetMarkerStyle(20);
  data_hist->SetMarkerColor(kBlack);
  data_hist->SetMarkerSize(1.2);
  data_hist->SetLineWidth(2);
  data_hist->SetStats(0);

  // Add MC histograms to stack
  TH1F* ttbar_hist = nullptr;
  for(const auto& hist : mcHists){
    /* // MC overflow
    if( TMath::Abs(hist->GetBinContent(hist->GetNbinsX()+1)) > 0 ){
      hist->AddBinContent(hist->GetNbinsX(), hist->GetBinContent(hist->GetNbinsX()+1));
    }
    */
    hist->Scale(scale_val);
    hist->SetFillColorAlpha(kOrange, 0.45);
    ttbar_hist = hist;
    hstack->Add(hist);
  }
  cout << "The scaling values is " << scale_val << endl;
  
  can->Divide(1, 2);
  
  TLegend *leg = new TLegend(0.7, 0.70, 0.85, 0.85);

  // Adjust the pads for better visuals
  TPad* pad0 = (TPad*)can->cd(1);
  
  pad0->SetPad(0.0, 0.29, 1.0, 1.0);  // Upper pad
 
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

  hstack->SetMaximum(2.5*hstack->GetMaximum());

  if(histName.find("lep_pt") != std::string::npos) log_scale = true;
  
  if(log_scale==true){
    pad0->SetLogy();
    hstack->SetMinimum(0.1);
    hstack->SetMaximum(100*hstack->GetMaximum());
  }
  
  if(log_scale==false){
    hstack->SetMaximum(2.0*hstack->GetMaximum());
    //hstack->SetMaximum(1.4*data_hist->GetMaximum());
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
  //h_err->Draw("e2same");
  h_err->Draw("e3same");
  //data_hist->SetTitle("Entries");
  data_hist->GetYaxis()->SetTitle(data_hist->GetYaxis()->GetTitle());
  data_hist->Draw("epsame");
    
  leg->AddEntry(data_hist, "Data", "lep");
  leg->AddEntry(ttbar_hist, "t#bar{t}","f");
  leg->AddEntry(h_err, "Stat. unc.","f");
  leg->SetBorderSize();
  leg->Draw();

  ATLASLabel(0.2, 0.85, "Work in Progress", kBlack);
  double dely = 0.05;
  string lumi_str = to_string(luminosity*fraction);
  //string energy_text = "#sqrt{s} = 13 TeV, 36 fb^{-1}";

  myText(0.2, 0.85-dely, kBlack, "#sqrt{s} = 13 TeV, 36 fb^{-1}");
  myText(0.2, 0.85-2*dely, kBlack, histName.c_str());
  
  // Draw the ratio plot on the lower pad
  pad1->cd();
  TH1F* h_mc_sum = dynamic_cast<TH1F*>(hstack->GetStack()->Last());
  TH1F* h_ratio = (TH1F*)data_hist->Clone("h_ratio");
  h_ratio->Divide(h_mc_sum);  // Get the ratio between data and the summed histogram from the stack

  h_ratio->SetMinimum(0);
  h_ratio->SetMaximum(2.2);
    
  h_ratio->SetLineColor(kBlack);
  h_ratio->SetMarkerStyle(20);
  //  h_ratio->SetTitle("");  // Remove title
  
  // Set Y-axis title and range
  h_ratio->GetYaxis()->SetTitle("Data/Pred");
  h_ratio->GetYaxis()->CenterTitle();
  h_ratio->GetYaxis()->SetNdivisions(505);  // 505 divisions for better look
  //h_ratio->GetYaxis()->SetNdivisions(504, false);  // 505 divisions for better look
  h_ratio->GetYaxis()->SetTitleSize(0.10);
  h_ratio->GetYaxis()->SetTitleOffset(0.45);
  h_ratio->GetYaxis()->SetLabelSize(0.13);
  h_ratio->GetYaxis()->SetLabelOffset(0.01);
  
  // Set X-axis labels
  h_ratio->GetXaxis()->SetTitle(data_hist->GetXaxis()->GetTitle());
  h_ratio->GetXaxis()->SetTitleSize(0.15);
  h_ratio->GetXaxis()->SetLabelSize(0.13);
  h_ratio->GetXaxis()->SetTitleOffset(1.2);
  h_ratio->GetXaxis()->SetLabelOffset(0.035);

  h_ratio->Draw("EP");
  //h_ratio->Draw("0E1");
    
  // Draw a horizontal line at ratio = 1
  TLine* line = new TLine(h_ratio->GetXaxis()->GetXmin(), 1, h_ratio->GetXaxis()->GetXmax(), 1);
  line->SetLineStyle(2);  // Dashed line 2
  line->Draw();

  // Save plot
  std::string outputFileName = "distribution_plots/" + histName + ".png";
  can->SaveAs(outputFileName.c_str());

  // Clean up
  delete can;
  delete hstack;
  delete leg;
  if (data_hist) delete data_hist;
  
}


// Main function to create plots for specific histograms
void CreateComparisonPlots(const std::vector<std::string>& files, const std::vector<std::string>& histNames) {
    for (const auto& histName : histNames) {
        std::vector<TH1F*> dataHists;
        std::vector<TH1F*> mcHists;
        GetHistogramsFromFiles(files, histName, dataHists, mcHists);
        PlotHistogram(histName, dataHists, mcHists);
        
        // Clean up histograms
        for (auto hist : dataHists) delete hist;
        for (auto hist : mcHists) delete hist;
    }
}

/*
void plot_scale_factors(TString name_plot){

  SetAtlasStyle();
  
  //TLegend *leg = new TLegend(0.7, 0.75, 0.85, 0.85);
  TH1F *SF_hist = new TH1F();
  
  TString name_image = "distribution_plots/"+name_plot+".png";
  //bool log_scale = false;
  TString header = "";
  
  if(name_plot=="SF_PILEUP"){
    SF_hist = hist_ScaleFactor_PILEUP;
  }

  if(name_plot=="SF_BTAG"){
    SF_hist = hist_ScaleFactor_BTAG;
  }

  if(name_plot=="SF_ELE"){
    SF_hist = hist_ScaleFactor_ELE;
  }

  if(name_plot=="SF_MUON"){
    SF_hist = hist_ScaleFactor_MUON;
  }

  if(name_plot=="SF_PHOTON"){
    SF_hist = hist_ScaleFactor_PHOTON;
  }

  if(name_plot=="SF_TAU"){
    SF_hist = hist_ScaleFactor_TAU;
  }
  
  TCanvas *can = new TCanvas("can","", 800, 600);

  SF_hist->Draw();
  ATLASLabel(0.55, 0.8, "Work in Progress", kBlack);
  myText(0.55, 0.75, kBlack, name_plot);
  //myText(0.2, 0.8-dely, kBlack, "for class: "+label_leg);
  //myText(0.2, 0.8-2*dely, kBlack, name_plot);

  can->SaveAs(name_image);
  
}
*/
