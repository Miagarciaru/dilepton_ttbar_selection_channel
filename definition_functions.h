#include "Plotting.h"

// *************************************
// Declaration of functions
// *************************************

void fill_hist_scale_factors();
void fill_histograms_for_data();
void fill_histograms_for_ttbar(float weight);
void scale_histograms(float sumw);
bool selection_good_bjets_cut(int &bjet_index1, int &bjet_index2);
bool opposite_charge_leptons_cut(int lep_index1, int lep_index2);
bool flavour_leptons_cut(int lep_index1, int lep_index2);
bool lepton_trigger_cut();
bool good_leptons_n_cut(int &lep_index1, int &lep_index2);
void deltaR(float &dR, float jet1_pt, float jet1_eta, float jet1_phi, float jet1_m, float jet2_pt, float jet2_eta, float jet2_phi, float jet2_m);
void set_branch_address(TChain *inChain);


// *************************************
// Definition of the functions declared above
// *************************************

void fill_hist_scale_factors(){

  hist_ScaleFactor_PILEUP->Fill(ScaleFactor_PILEUP);
  hist_ScaleFactor_BTAG->Fill(ScaleFactor_BTAG);
  hist_ScaleFactor_ELE->Fill(ScaleFactor_ELE);
  hist_ScaleFactor_MUON->Fill(ScaleFactor_MUON);
  hist_ScaleFactor_PHOTON->Fill(ScaleFactor_PHOTON);
  hist_ScaleFactor_TAU->Fill(ScaleFactor_TAU);

}


void fill_histograms_for_data(){

  TLorentzVector lep1 = TLorentzVector();  
  TLorentzVector lep2 = TLorentzVector();
  TLorentzVector dilep = TLorentzVector();

  lep1.SetPtEtaPhiE(lep_pt->at(glep_index1), lep_eta->at(glep_index1), lep_phi->at(glep_index1), lep_e->at(glep_index1));
  lep2.SetPtEtaPhiE(lep_pt->at(glep_index2), lep_eta->at(glep_index2), lep_phi->at(glep_index2), lep_e->at(glep_index2));
  dilep = lep1 + lep2;
  
  data_hist_met->Fill(met);
  data_hist_lep_pt->Fill(dilep.Pt());
  data_hist_lep_eta->Fill(TMath::Abs(dilep.Eta()));
}

void fill_histograms_for_ttbar(float weight){
  
  TLorentzVector lep1 = TLorentzVector();  
  TLorentzVector lep2 = TLorentzVector();
  TLorentzVector dilep = TLorentzVector();

  lep1.SetPtEtaPhiE(lep_pt->at(glep_index1), lep_eta->at(glep_index1), lep_phi->at(glep_index1), lep_e->at(glep_index1));
  lep2.SetPtEtaPhiE(lep_pt->at(glep_index2), lep_eta->at(glep_index2), lep_phi->at(glep_index2), lep_e->at(glep_index2));
  dilep = lep1 + lep2;
  
  ttbar_hist_met->Fill(met, weight);
  ttbar_hist_lep_pt->Fill(dilep.Pt(), weight);
  ttbar_hist_lep_eta->Fill(TMath::Abs(dilep.Eta()), weight);
}

void scale_histograms(float sumw){
  //float scale_val = xsec*lumi*1000/sumw;
  float scale_val = lumi*fraction*1000.0/sumw;
  ttbar_hist_met->Scale(scale_val);
  ttbar_hist_lep_pt->Scale(scale_val);
  ttbar_hist_lep_eta->Scale(scale_val);
}

bool selection_good_bjets_cut(int &bjet_index1, int &bjet_index2){
  
    //Preselection of good b-jets
  
    int goodbjet_n = 0;
    int goodbjet_index[jet_n];
    int bjet_index = 0;
  
    for(unsigned int ii=0; ii<jet_n; ii++){
      if( jet_pt->at(ii) <= 25. || TMath::Abs(jet_eta->at(ii)) >= 2.5 ) continue;

        // JVT cleaning
      if ( jet_pt->at(ii) < 60. ){
	if( jet_jvt->size()==0 ) continue;
	if( TMath::Abs(jet_eta->at(ii)) < 2.4 && jet_jvt->at(ii)==false ) continue;
      }
        
      // cut on b-tagged
      if( jet_DL1d77_isBtagged->size()==0) continue;
      if( jet_DL1d77_isBtagged->at(ii)==true ){
	if(TMath::Abs(jet_eta->at(ii)) >= 2.5) continue;
	goodbjet_n++;
	goodbjet_index[bjet_index] = ii;
	bjet_index++;
      }
    }
    
    if( goodbjet_n<2 ){ return false;}
    bjet_index1 = goodbjet_index[0];
    bjet_index2 = goodbjet_index[1];    

    return true;
}

//opposite charge leptons
bool opposite_charge_leptons_cut(int lep_index1, int lep_index2){
  if( lep_charge->at(lep_index1)*lep_charge->at(lep_index2) > 0){ return false;} //Leptons of same charges return false
    return true;
}

//two different-flavour leptons
bool flavour_leptons_cut(int lep_index1, int lep_index2){
  if( lep_type->at(lep_index1) == lep_type->at(lep_index2)){ return false;} //leptons of same flavours return false
    return true;
}

// trigger cuts
bool lepton_trigger_cut(){
  if( (trigE==true) || (trigM==true) ){ return true; }
  return false;
}

bool good_leptons_n_cut(int &lep_index1, int &lep_index2){
 
  // Preselection of good leptons
  int goodlep_index[lep_n];
  int goodlep_n = 0;
  int lep_index =0;
      
  for(unsigned int ii=0; ii<lep_n; ii++){
        
    TLorentzVector leptemp = TLorentzVector();  
    leptemp.SetPtEtaPhiE(lep_pt->at(ii), lep_eta->at(ii), lep_phi->at(ii), lep_e->at(ii));
        
    // Lepton is Tight
    if( lep_isTight->at(ii) == false ) continue;
    if( lep_isTightID->at(ii) == false ) continue;
    if( lep_isTightIso->at(ii) == false ) continue;
    // standard lepton isolation requirement => strict isolation
    if( lep_pt->at(ii) <= 25. ) continue;
    if( lep_ptvarcone30->at(ii)/lep_pt->at(ii) >= 0.1 ) continue; 
    if( lep_topoetcone20->at(ii)/lep_pt->at(ii) >= 0.1 ) continue;
    // electron selection
    if( lep_type->at(ii)==11 && TMath::Abs(lep_eta->at(ii))<2.47 && (TMath::Abs(lep_eta->at(ii))<1.37 || TMath::Abs(lep_eta->at(ii))>1.52) ){

      //Condition for transverse impact parameter
      if( TMath::Abs(lep_d0sig->at(ii)>=5) ) continue;
      if( TMath::Abs(lep_z0->at(ii)*TMath::Sin(leptemp.Theta()))>=0.5 ) continue;    
      goodlep_n++;
      goodlep_index[lep_index] = ii;
      lep_index++;
    }
            
    // muon selection
    if( lep_type->at(ii)==13 && TMath::Abs(lep_eta->at(ii))<2.5 ){
      //Condition for transverse impact parameter      
      if( TMath::Abs(lep_d0sig->at(ii)>=3) ) continue; 
      if( TMath::Abs(lep_z0->at(ii)*TMath::Sin(leptemp.Theta()))>=0.5 ) continue;
      
      goodlep_n++;
      goodlep_index[lep_index] = ii;
      lep_index++;
    }
  }
  
  if(goodlep_n!=2){ return false;}
  //Exactly two good leptons, leading lepton with pT > 22 GeV and the subleading lepton with pT > 15 GeV
  lep_index1 = goodlep_index[0];
  lep_index2 = goodlep_index[1];
  
  return true;
}


void deltaR(float &dR, float jet1_pt, float jet1_eta, float jet1_phi, float jet1_m, float jet2_pt, float jet2_eta, float jet2_phi, float jet2_m){

  TLorentzVector jet1  = TLorentzVector();
  TLorentzVector jet2  = TLorentzVector();
      
  jet1.SetPtEtaPhiM(jet1_pt/1000., jet1_eta, jet1_phi, jet1_m/1000.);
  jet2.SetPtEtaPhiM(jet2_pt/1000., jet2_eta, jet2_phi, jet2_m/1000.);

  dR = jet1.DeltaR(jet2);
}


// This function saves the branches info for a given tree in the variables defined above
void set_branch_address(TChain *inChain){

  inChain->SetBranchAddress("ScaleFactor_PILEUP", &ScaleFactor_PILEUP, &b_ScaleFactor_PILEUP);
  inChain->SetBranchAddress("mcWeight", &mcWeight, &b_mcWeight);
  inChain->SetBranchAddress("xsec", &xsec, &b_xsec);
  inChain->SetBranchAddress("trigE", &trigE, &b_trigE);
  inChain->SetBranchAddress("trigM", &trigM, &b_trigM);
  inChain->SetBranchAddress("ScaleFactor_BTAG", &ScaleFactor_BTAG, &b_ScaleFactor_BTAG);
  inChain->SetBranchAddress("jet_n", &jet_n, &b_jet_n);
  inChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
  inChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
  inChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
  inChain->SetBranchAddress("jet_e", &jet_e, &b_jet_e);
  inChain->SetBranchAddress("jet_DL1d77_isBtagged", &jet_DL1d77_isBtagged, &b_jet_DL1d77_isBtagged);
  inChain->SetBranchAddress("jet_jvt", &jet_jvt, &b_jet_jvt);
  inChain->SetBranchAddress("largeRJet_n", &largeRJet_n, &b_largeRJet_n);
  inChain->SetBranchAddress("largeRJet_pt", &largeRJet_pt, &b_largeRJet_pt);
  inChain->SetBranchAddress("largeRJet_eta", &largeRJet_eta, &b_largeRJet_eta);
  inChain->SetBranchAddress("largeRJet_phi", &largeRJet_phi, &b_largeRJet_phi);
  inChain->SetBranchAddress("largeRJet_e", &largeRJet_e, &b_largeRJet_e);
  inChain->SetBranchAddress("largeRJet_m", &largeRJet_m, &b_largeRJet_m);
  inChain->SetBranchAddress("largeRJet_D2", &largeRJet_D2, &b_largeRJet_D2);
  inChain->SetBranchAddress("ScaleFactor_ELE", &ScaleFactor_ELE, &b_ScaleFactor_ELE);
  inChain->SetBranchAddress("ScaleFactor_MUON", &ScaleFactor_MUON, &b_ScaleFactor_MUON);
  inChain->SetBranchAddress("lep_n", &lep_n, &b_lep_n);
  inChain->SetBranchAddress("lep_type", &lep_type, &b_lep_type);
  inChain->SetBranchAddress("lep_pt", &lep_pt, &b_lep_pt);
  inChain->SetBranchAddress("lep_eta", &lep_eta, &b_lep_eta);
  inChain->SetBranchAddress("lep_phi", &lep_phi, &b_lep_phi);
  inChain->SetBranchAddress("lep_e", &lep_e, &b_lep_e);
  inChain->SetBranchAddress("lep_charge", &lep_charge, &b_lep_charge);
  inChain->SetBranchAddress("lep_ptvarcone30", &lep_ptvarcone30, &b_lep_ptvarcone30);
  inChain->SetBranchAddress("lep_topoetcone20", &lep_topoetcone20, &b_lep_topoetcone20);
  inChain->SetBranchAddress("lep_z0", &lep_z0, &b_lep_z0);
  inChain->SetBranchAddress("lep_d0", &lep_d0, &b_lep_d0);
  inChain->SetBranchAddress("lep_d0sig", &lep_d0sig, &b_lep_d0sig);
  inChain->SetBranchAddress("lep_isTight", &lep_isTight, &b_lep_isTight);
  inChain->SetBranchAddress("lep_isTightID", &lep_isTightID, &b_lep_isTightID);
  inChain->SetBranchAddress("lep_isTightIso", &lep_isTightIso, &b_lep_isTightIso);
  inChain->SetBranchAddress("ScaleFactor_PHOTON", &ScaleFactor_PHOTON, &b_ScaleFactor_PHOTON);
  inChain->SetBranchAddress("photon_n", &photon_n, &b_photon_n);
  inChain->SetBranchAddress("photon_pt", &photon_pt, &b_photon_pt);
  inChain->SetBranchAddress("photon_eta", &photon_eta, &b_photon_eta);
  inChain->SetBranchAddress("photon_phi", &photon_phi, &b_photon_phi);
  inChain->SetBranchAddress("photon_e", &photon_e, &b_photon_e);
  inChain->SetBranchAddress("photon_ptcone20", &photon_ptcone20, &b_photon_ptcone20);
  inChain->SetBranchAddress("photon_topoetcone40", &photon_topoetcone40, &b_photon_topoetcone40);
  inChain->SetBranchAddress("photon_isTight", &photon_isTight, &b_photon_isTight);
  inChain->SetBranchAddress("photon_isTightID", &photon_isTightID, &b_photon_isTightID);
  inChain->SetBranchAddress("photon_isTightIso", &photon_isTightIso, &b_photon_isTightIso);
  inChain->SetBranchAddress("ScaleFactor_TAU", &ScaleFactor_TAU, &b_ScaleFactor_TAU);
  inChain->SetBranchAddress("tau_n", &tau_n, &b_tau_n);
  inChain->SetBranchAddress("tau_pt", &tau_pt, &b_tau_pt);
  inChain->SetBranchAddress("tau_eta", &tau_eta, &b_tau_eta);
  inChain->SetBranchAddress("tau_phi", &tau_phi, &b_tau_phi);
  inChain->SetBranchAddress("tau_e", &tau_e, &b_tau_e);
  inChain->SetBranchAddress("tau_charge", &tau_charge, &b_tau_charge);
  inChain->SetBranchAddress("tau_nTracks", &tau_nTracks, &b_tau_nTracks);
  inChain->SetBranchAddress("tau_isTight", &tau_isTight, &b_tau_isTight);
  inChain->SetBranchAddress("tau_RNNJetScore", &tau_RNNJetScore, &b_tau_RNNJetScore);
  inChain->SetBranchAddress("tau_RNNEleScore", &tau_RNNEleScore, &b_tau_RNNEleScore);
  inChain->SetBranchAddress("met", &met, &b_met);
  inChain->SetBranchAddress("met_phi", &met_phi, &b_met_phi);
  inChain->SetBranchAddress("met_mpx", &met_mpx, &b_met_mpx);
  inChain->SetBranchAddress("met_mpy", &met_mpy, &b_met_mpy);
}
