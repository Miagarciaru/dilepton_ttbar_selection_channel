#include "AtlasStyle/AtlasStyle.C"
#include "AtlasStyle/AtlasUtils.C"
#include "AtlasStyle/AtlasLabels.C"
#include <iostream>
#include <fstream>
#include <sstream>


struct ScaleFactor {
  std::string name_file;
  float xsec;
  float efficiency;
  float kfactor;
  float sumw;
  float SF_hist;
};

float luminosity = 36.0;
float fraction = 1.0;
/*
float XSEC = 729.77;
float fil_eff = 0.1053695;
float k_factor = 1.13975636159;
float sumw = 4.20575e+10;
float scale_val = (k_factor*fil_eff*XSEC*luminosity*1000.0)/sumw;
*/

void Get_Scale_val(std::unordered_map<std::string, ScaleFactor>& SF){

  std::string input_file = "info_file.txt";
  ifstream input(input_file.c_str());  
  std::string line;
  while(getline(input,line)){
    if (line.find("#") != line.npos ) continue; // a # is a comment and not read
    std::string name, XSEC, sum_w, eff, kfac;
    istringstream linestream(line);
    getline(linestream, name, '|');
    getline(linestream, XSEC, '|');
    getline(linestream, sum_w, '|');
    getline(linestream, eff, '|');
    getline(linestream, kfac);
    //if (DEBUG) cout << name << " " << xsec << " " << sumw << " " << eff << endl;
    ScaleFactor factors;
    factors.name_file = name;
    factors.xsec = atof(XSEC.c_str());
    factors.efficiency = atof(eff.c_str());
    factors.kfactor = atof(kfac.c_str());
    factors.sumw = atof(sum_w.c_str());
    factors.SF_hist = (atof(kfac.c_str())*atof(eff.c_str())*atof(XSEC.c_str())*luminosity*1000)/(atof(sum_w.c_str()));
    SF[name] = factors;
  }
  
}

// Function to get a list of histograms from a ROOT file
void GetHistogramsFromFiles(const std::vector<std::string>& files, const std::string& histName, std::vector<TH1F*>& dataHists,
			    std::vector<TH1F*>& ttbarHists, std::vector<TH1F*>& stopHists, const std::unordered_map<std::string, ScaleFactor>& SF){

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
    else if(file.find("ttbar") != std::string::npos){
      hist->Scale(SF.at(file.c_str()).SF_hist);
      ttbarHists.push_back(hist);
    }
    else{
      hist->Scale(SF.at(file.c_str()).SF_hist);
      stopHists.push_back(hist);
    }

    f->Close();
    
  }
}


// Function to plot a given histogram
void PlotHistogram(const std::string& histName, const std::vector<TH1F*>& dataHists, const std::vector<TH1F*>& ttbarHists, const std::vector<TH1F*>& stopHists){

  SetAtlasStyle();

  bool log_scale = false;
  
  TCanvas* can = new TCanvas(("c_" + histName).c_str(), histName.c_str(), 800, 600);
  THStack* hstack = new THStack(("hs_" + histName).c_str(), histName.c_str());

  // Create a combined data histogram
  TH1F* hist_data = nullptr;
  
  for(const auto& hist : dataHists){
    if(hist_data == nullptr){
      hist_data = dynamic_cast<TH1F*>(hist->Clone(("data_" + histName).c_str()));
      hist_data->SetDirectory(0); // Detach from file
    }
    else{
      hist_data->Add(hist);
    }
  }

  hist_data->SetMarkerStyle(20);
  hist_data->SetMarkerColor(kBlack);
  hist_data->SetMarkerSize(1.2);
  hist_data->SetLineWidth(2);
  hist_data->SetStats(0);

  
  TH1F* hist_ttbar = nullptr;
  for(const auto& hist : ttbarHists){
    /* // MC overflow
    if( TMath::Abs(hist->GetBinContent(hist->GetNbinsX()+1)) > 0 ){
      hist->AddBinContent(hist->GetNbinsX(), hist->GetBinContent(hist->GetNbinsX()+1));
    }
    */
    if(hist_ttbar == nullptr){
      hist_ttbar = dynamic_cast<TH1F*>(hist->Clone(("ttbar_" + histName).c_str()));
      hist_ttbar->SetDirectory(0); // Detach from file
    }
    else{
      hist_ttbar->Add(hist);
    }
    
    //hist->Scale(scale_val);
    //hist->SetFillColorAlpha(kOrange, 0.45);
    //hist_ttbar = hist;
    //hstack->Add(hist);
  }
  
  TH1F* hist_stop = nullptr;
  for(const auto& hist : stopHists){
    if(hist_stop == nullptr){
      hist_stop = dynamic_cast<TH1F*>(hist->Clone(("stop_" + histName).c_str()));
      hist_stop->SetDirectory(0); // Detach from file
    }
    else{
      hist_stop->Add(hist);
    }
  }

  // Add MC histograms to stack
  hist_ttbar->SetFillColorAlpha(kOrange, 0.45);
  hist_stop->SetFillColorAlpha(kBlue, 0.45);
  
  hstack->Add(hist_ttbar);
  hstack->Add(hist_stop);
  
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
  TH1F *h_err = (TH1F*) hist_ttbar->Clone();
  h_err->Add(hist_stop);
    
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
  hist_data->GetYaxis()->SetTitle(hist_data->GetYaxis()->GetTitle());
  hist_data->Draw("epsame");
    
  leg->AddEntry(hist_data, "Data", "lep");
  leg->AddEntry(hist_ttbar, "t#bar{t}","f");
  leg->AddEntry(hist_stop, "Single top","f");
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
  TH1F* h_ratio = (TH1F*)hist_data->Clone("h_ratio");
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
  h_ratio->GetXaxis()->SetTitle(hist_data->GetXaxis()->GetTitle());
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
  if (hist_data) delete hist_data;
  
}


// Main function to create plots for specific histograms
void CreateComparisonPlots(const std::vector<std::string>& files, const std::vector<std::string>& histNames) {
  //PlotHistogram(histName, dataHists, mcHists, stopHists);
  std::unordered_map<std::string, ScaleFactor> SF_val;
  Get_Scale_val(SF_val);

  for(const auto& variable_entry : SF_val) {
    const std::string& variable_name = variable_entry.first;
    const ScaleFactor& scf_val = variable_entry.second;

    cout << variable_name << "\t" << scf_val.xsec << "\t" << scf_val.sumw << "\t" << scf_val.efficiency << "\t" << scf_val.kfactor << "\t" << scf_val.SF_hist << endl;
  }

  for (const auto& histName : histNames) {
    std::vector<TH1F*> dataHists;
    std::vector<TH1F*> ttbarHists;
    std::vector<TH1F*> stopHists;
    GetHistogramsFromFiles(files, histName, dataHists, ttbarHists, stopHists, SF_val);

    PlotHistogram(histName, dataHists, ttbarHists, stopHists);

    // Clean up histograms
    for (auto hist : dataHists) delete hist;
    for (auto hist : ttbarHists) delete hist;
    for (auto hist : stopHists) delete hist;
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
