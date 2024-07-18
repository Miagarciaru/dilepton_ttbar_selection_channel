#include "definition_functions.h"
#include <TMath.h>

void ttbar_selection(TString sample_path, TString sample_name){

  // Path of the merged samples

  //std::cout.precision(5);
  //std::cout.setf(std::ios::scientific);

  cout << "----------------------------------------------------------------------------------------------------------------" << endl;
  cout << "Processing file " << sample_name << endl;
  cout << "----------------------------------------------------------------------------------------------------------------" << endl;

  auto start = chrono::steady_clock::now(); //Start the clock

  // Open the input files and access to the trees

  TChain *fchain = new TChain("analysis");
  fchain->AddFile(sample_path);
  
  Long64_t nentries = fchain->GetEntries();

  set_branch_address(fchain);
  
  int trigger_cut = 0;
  int good_lepton_n_cut = 0;
  int OP_charge_leptons_cut = 0;
  int type_leptons_cut = 0;
  int bjets_cut = 0;
  
  bool is_data = sample_name.Contains("data");

  if(is_data==false){
    fchain->SetBranchAddress("initial_events", &initial_events, &b_initial_events);
    fchain->SetBranchAddress("initial_sum_of_weights", &initial_sum_of_weights, &b_initial_sum_of_weights);
    fchain->SetBranchAddress("initial_sum_of_weights_squared", &initial_sum_of_weights_squared, &b_initial_sum_of_weights_squared);
  }

  std::set<float> uniqueWeights;
  
  for(int ii=0; ii < nentries*fraction; ii++){

    if(ii%15000000==0){
      cout << "Processing entry " << ii << " of " << nentries << " total number of entries (Percentage of progress: " << (ii*1.0/nentries)*100 << " %)" << endl;
    }
    
    fchain->GetEntry(ii);
    
    int bjet_index1 = -1;
    int bjet_index2 = -1;
    
    //float SF = ScaleFactor_PILEUP*ScaleFactor_BTAG*ScaleFactor_ELE*ScaleFactor_MUON*ScaleFactor_PHOTON*ScaleFactor_TAU;
    float SF = ScaleFactor_PILEUP*ScaleFactor_BTAG*ScaleFactor_ELE*ScaleFactor_MUON*ScaleFactor_TAU;
    //float SF = TMath::Abs(ScaleFactor_PILEUP*ScaleFactor_BTAG*ScaleFactor_ELE*ScaleFactor_MUON*ScaleFactor_TAU);

    if( SF<0 ) continue;
    
    float weight = mcWeight*SF;
    if((ii==0) &&(is_data==false)) XSEC=xsec;    
    if(is_data==false) uniqueWeights.insert(initial_sum_of_weights);
    if(is_data==true) weight=1.0;
    
    if( lepton_trigger_cut() == false ) continue;
    trigger_cut++;
    if( good_leptons_n_cut() == false ) continue;
    good_lepton_n_cut++;
    if( opposite_charge_leptons_cut() == false ) continue;
    OP_charge_leptons_cut++;
    if( flavour_leptons_cut() == false ) continue;
    type_leptons_cut++;
    if( selection_good_bjets_cut()==false ) continue;
    bjets_cut++;
    if( weight<0 ){
      cout << "The event weight for the entry " << ii << " is negative: (mcWeight - weight - SF) -> " << mcWeight << "\t" << weight << "\t" << SF << endl;
    }
    fill_histograms(weight);
    
    if(is_data==false) fill_hist_scale_factors();
  }

  mc_under_and_overflow();
  
  // Create ROOT file
  TString output_name = "output_analysis/"+sample_name+".root";
  TFile* outFile = new TFile(output_name, "RECREATE");
    
  // Write histograms to file
  hist_met->Write();
  hist_lep_pt->Write();
  hist_lep_eta->Write();

  if(is_data==false){
    hist_ScaleFactor_PILEUP->Write();
    hist_ScaleFactor_BTAG->Write();
    hist_ScaleFactor_ELE->Write();
    hist_ScaleFactor_MUON->Write();
    hist_ScaleFactor_PHOTON->Write();
    hist_ScaleFactor_TAU->Write();
  }
  
  // Close the file
  outFile->Close();

  // Clean up
  delete hist_met;
  delete hist_lep_pt;
  delete hist_lep_eta;
  delete hist_ScaleFactor_PILEUP;
  delete hist_ScaleFactor_BTAG;
  delete hist_ScaleFactor_ELE;
  delete hist_ScaleFactor_MUON;
  delete hist_ScaleFactor_PHOTON;
  delete hist_ScaleFactor_TAU;
  
  cout << "The percentage of events passing the trigger cut is: " << trigger_cut/(nentries*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the good lepton number cut is: " << good_lepton_n_cut/(nentries*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the opposite charged leptons cut is: " << OP_charge_leptons_cut/(nentries*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the type lepton cut is: " << type_leptons_cut/(nentries*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the bjets number cut is: " << bjets_cut/(nentries*fraction)*100.0 << "%" << endl;

  if(is_data==false){
    float totalSumOfWeights = 0.0;
    int file = 1;
    for (const auto& weight : uniqueWeights){
      totalSumOfWeights += weight;
      cout << "File: " << file << "\t  \t" << weight << endl;
      file++;
    }
    
    cout << "The xsec is: " << XSEC << " pb" << endl;
    cout << "The fraction is: " << fraction << endl;
    cout << "Total sum of weights is: " << totalSumOfWeights << endl;
  }
  
  auto end = chrono::steady_clock::now();
  auto elapsed = chrono::duration_cast<chrono::seconds>(end - start).count();
  auto time_in_min = elapsed/60.;
  cout << "Time taken: " << time_in_min << " min" << endl;

}
