#include "definition_functions.h"
#include <TMath.h>

void ttbar_selection(){

  // Path of the merged samples

  std::cout.precision(5);
  //std::cout.setf(std::ios::scientific);

  auto start = chrono::steady_clock::now(); //Start the clock

  TString path="/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/";
  
  // Open the input files and access to the trees

  std::vector<TString> data_samples = {"data15_allyear.root", "data16_allyear_A.root", "data16_allyear_B.root", "data16_allyear_C.root", "data16_allyear_D.root", "data16_allyear_E.root", "data16_allyear_F.root", "data16_allyear_G.root", "data16_allyear_H.root"};
  
  std::vector<TString> ttbar_samples = {"ttbar.root"};
  
  TChain *chain_data = new TChain("analysis");
  TChain *chain_ttbar = new TChain("analysis");
  
  for(int ii=0; ii<data_samples.size(); ii++){
    chain_data->AddFile(path+data_samples[ii]);
  }

  for(int ii=0; ii<ttbar_samples.size(); ii++){
    chain_ttbar->AddFile(path+ttbar_samples[ii]);
  }
  
  std::cout << "Number of files in data chain: " << chain_data->GetListOfFiles()->GetEntries() << std::endl;
  std::cout << "Number of entries in data chain: " << chain_data->GetEntries()*fraction << std::endl;
  Long64_t nentries_data = chain_data->GetEntries();

  set_branch_address(chain_data);
  
  //float fraction = 0.01;

  int trigger_cut = 0;
  int good_lepton_n_cut = 0;
  int OP_charge_leptons_cut = 0;
  int type_leptons_cut = 0;
  int bjets_cut = 0;

  cout << "Processing data" << endl;
  
  for(int ii=0; ii < nentries_data*fraction; ii++){
    chain_data->GetEntry(ii);
    //int lep_index1 = -1;
    //int lep_index2 = -1;
    int bjet_index1 = -1;
    int bjet_index2 = -1;

    if( lepton_trigger_cut() == false ) continue;
    trigger_cut++;
    if( good_leptons_n_cut(glep_index1, glep_index2) == false ) continue;
    good_lepton_n_cut++;
    if( opposite_charge_leptons_cut(glep_index1, glep_index2) == false ) continue;
    OP_charge_leptons_cut++;
    if( flavour_leptons_cut(glep_index1, glep_index2) == false ) continue;
    type_leptons_cut++;
    if( selection_good_bjets_cut(bjet_index1, bjet_index2)==false ) continue;
    bjets_cut++;
    fill_histograms_for_data();
  } 
  
  cout << "The percentage of events passing the trigger cut is: " << trigger_cut/(nentries_data*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the good lepton number cut is: " << good_lepton_n_cut/(nentries_data*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the opposite charged leptons cut is: " << OP_charge_leptons_cut/(nentries_data*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the type lepton cut is: " << type_leptons_cut/(nentries_data*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the bjets number cut is: " << bjets_cut/(nentries_data*fraction)*100.0 << "%" << endl;

  cout << "Processing ttbar" << endl;

  std::cout << "Number of files in ttbar chain: " << chain_ttbar->GetListOfFiles()->GetEntries() << std::endl;
  std::cout << "Number of entries in ttbar chain: " << chain_ttbar->GetEntries()*fraction << std::endl;
  Long64_t nentries_ttbar = chain_ttbar->GetEntries();

  set_branch_address(chain_ttbar);
  chain_ttbar->SetBranchAddress("initial_events", &initial_events, &b_initial_events);
  chain_ttbar->SetBranchAddress("initial_sum_of_weights", &initial_sum_of_weights, &b_initial_sum_of_weights);
  chain_ttbar->SetBranchAddress("initial_sum_of_weights_squared", &initial_sum_of_weights_squared, &b_initial_sum_of_weights_squared);

  trigger_cut = 0;
  good_lepton_n_cut = 0;
  OP_charge_leptons_cut = 0;
  type_leptons_cut = 0;
  bjets_cut = 0;

  cout << "Processing ttbar" << endl;

  std::set<float> uniqueWeights;

  for(int ii=0; ii < nentries_ttbar*fraction; ii++){
    chain_ttbar->GetEntry(ii);
    
    //int lep_index1 = -1;
    //int lep_index2 = -1;
    int bjet_index1 = -1;
    int bjet_index2 = -1;

    float SF = ScaleFactor_PILEUP*ScaleFactor_BTAG*ScaleFactor_ELE*ScaleFactor_MUON*ScaleFactor_PHOTON*ScaleFactor_TAU;
    float weight = xsec*mcWeight*SF;
    
    uniqueWeights.insert(initial_sum_of_weights);
    
    if( lepton_trigger_cut() == false ) continue;
    trigger_cut++;
    if( good_leptons_n_cut(glep_index1, glep_index2) == false ) continue;
    good_lepton_n_cut++;
    if( opposite_charge_leptons_cut(glep_index1, glep_index2) == false ) continue;
    OP_charge_leptons_cut++;
    if( flavour_leptons_cut(glep_index1, glep_index2) == false ) continue;
    type_leptons_cut++;
    if( selection_good_bjets_cut(bjet_index1, bjet_index2)==false ) continue;
    bjets_cut++;
    fill_histograms_for_ttbar(weight);
  }
  
  cout << "The percentage of events passing the trigger cut is: " << trigger_cut/(nentries_data*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the good lepton number cut is: " << good_lepton_n_cut/(nentries_data*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the opposite charged leptons cut is: " << OP_charge_leptons_cut/(nentries_data*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the type lepton cut is: " << type_leptons_cut/(nentries_data*fraction)*100.0 << "%" << endl;
  cout << "The percentage of events passing the bjets number cut is: " << bjets_cut/(nentries_data*fraction)*100.0 << "%" << endl;

  float totalSumOfWeights = 0.0;
  for (const auto& weight : uniqueWeights){
    totalSumOfWeights += weight;
    //cout << weight << endl;
  }
  cout << "Total sum of weights: " << totalSumOfWeights << endl;
  
  scale_histograms(totalSumOfWeights);
  plot_distributions_comparison("met");
  plot_distributions_comparison("lep_pt");
  plot_distributions_comparison("lep_eta");
  
  auto end = chrono::steady_clock::now();
  auto elapsed = chrono::duration_cast<chrono::seconds>(end - start).count();
  auto time_in_min = elapsed/60.;
  cout << "Time taken: " << time_in_min << " min" << endl;

}
