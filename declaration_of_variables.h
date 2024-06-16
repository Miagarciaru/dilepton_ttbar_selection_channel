// *************************************
// Declaration of global variables
// *************************************

TString process_name = "";
float lumi = 36.0;
float fraction = 1.0;
float XSEC;
float fil_eff = 0.1053695;
float k_factor = 1.13975636159;

// *************************************
// Declaration of leaf variables
// *************************************

// Declaration of leaf types for truth_children branches for boosted tree variables

Float_t ScaleFactor_PILEUP;
Float_t mcWeight;
Float_t xsec;

Bool_t trigE;
Bool_t trigM;

Float_t ScaleFactor_BTAG;
Int_t jet_n;

vector<float> *jet_pt;
vector<float> *jet_eta;
vector<float> *jet_phi;
vector<float> *jet_e;
vector<bool> *jet_DL1d77_isBtagged;
vector<bool> *jet_jvt;

Int_t largeRJet_n;
vector<float> *largeRJet_pt;
vector<float> *largeRJet_eta;
vector<float> *largeRJet_phi;
vector<float> *largeRJet_e;
vector<float> *largeRJet_m;
vector<float> *largeRJet_D2;
Float_t ScaleFactor_ELE;
Float_t ScaleFactor_MUON;

Int_t lep_n;
vector<int> *lep_type;
vector<float> *lep_pt;
vector<float> *lep_eta;
vector<float> *lep_phi;
vector<float> *lep_e;
vector<int> *lep_charge;

vector<float> *lep_ptvarcone30;
vector<float> *lep_topoetcone20;
vector<float> *lep_z0;
vector<float> *lep_d0;
vector<float> *lep_d0sig;

vector<bool> *lep_isTight;
vector<bool> *lep_isTightID;
vector<bool> *lep_isTightIso;

Float_t ScaleFactor_PHOTON;
Int_t photon_n;
vector<float> *photon_pt;
vector<float> *photon_eta;
vector<float> *photon_phi;
vector<float> *photon_e;
vector<float> *photon_ptcone20;
vector<float> *photon_topoetcone40;
vector<bool> *photon_isTight;
vector<bool> *photon_isTightID;
vector<bool> *photon_isTightIso;

Float_t ScaleFactor_TAU;
Int_t tau_n;
vector<float> *tau_pt;
vector<float> *tau_eta;
vector<float> *tau_phi;
vector<float> *tau_e;
vector<float> *tau_charge;
vector<int> *tau_nTracks;
vector<bool> *tau_isTight;

vector<float> *tau_RNNJetScore;
vector<float> *tau_RNNEleScore;

Float_t met;
Float_t met_phi;
Float_t met_mpx;
Float_t met_mpy;

Float_t initial_events;
Float_t initial_sum_of_weights;
Float_t initial_sum_of_weights_squared;

int lep_index1;
int lep_index2;

int bjet_index1;
int bjet_index2;

// *************************************
// Declaration of branches
// *************************************

TBranch *b_ScaleFactor_PILEUP;
TBranch *b_mcWeight;
TBranch *b_xsec;

TBranch *b_trigE;
TBranch *b_trigM;

TBranch *b_ScaleFactor_BTAG;
TBranch *b_jet_n;

TBranch *b_jet_pt;
TBranch *b_jet_eta;
TBranch *b_jet_phi;
TBranch *b_jet_e;
TBranch *b_jet_DL1d77_isBtagged;
TBranch *b_jet_jvt;

TBranch *b_largeRJet_n;
TBranch *b_largeRJet_pt;
TBranch *b_largeRJet_eta;
TBranch *b_largeRJet_phi;
TBranch *b_largeRJet_e;
TBranch *b_largeRJet_m;
TBranch *b_largeRJet_D2;
TBranch *b_ScaleFactor_ELE;
TBranch *b_ScaleFactor_MUON;

TBranch *b_lep_n;
TBranch *b_lep_type;
TBranch *b_lep_pt;
TBranch *b_lep_eta;
TBranch *b_lep_phi;
TBranch *b_lep_e;
TBranch *b_lep_charge;

TBranch *b_lep_ptvarcone30;
TBranch *b_lep_topoetcone20;
TBranch *b_lep_z0;
TBranch *b_lep_d0;
TBranch *b_lep_d0sig;

TBranch *b_lep_isTight;
TBranch *b_lep_isTightID;
TBranch *b_lep_isTightIso;

TBranch *b_ScaleFactor_PHOTON;
TBranch *b_photon_n;
TBranch *b_photon_pt;
TBranch *b_photon_eta;
TBranch *b_photon_phi;
TBranch *b_photon_e;
TBranch *b_photon_ptcone20;
TBranch *b_photon_topoetcone40;
TBranch *b_photon_isTight;
TBranch *b_photon_isTightID;
TBranch *b_photon_isTightIso;

TBranch *b_ScaleFactor_TAU;
TBranch *b_tau_n;
TBranch *b_tau_pt;
TBranch *b_tau_eta;
TBranch *b_tau_phi;
TBranch *b_tau_e;
TBranch *b_tau_charge;
TBranch *b_tau_nTracks;
TBranch *b_tau_isTight;

TBranch *b_tau_RNNJetScore;
TBranch *b_tau_RNNEleScore;

TBranch *b_met;
TBranch *b_met_phi;
TBranch *b_met_mpx;
TBranch *b_met_mpy;

TBranch *b_initial_events;
TBranch *b_initial_sum_of_weights;
TBranch *b_initial_sum_of_weights_squared;

// *************************************
// Declaration of histograms
// *************************************

// Missing Transverse Energy histograms
TH1F *hist_met = new TH1F("hist_met", "Missing Transverse Energy distribution; E_{T}^{miss} [GeV]; Events", 30, 0, 200);

// Dilepton pT histograms
TH1F *hist_lep_pt = new TH1F("hist_lep_pt", "p_{T} distribution of the dilepton system; p_{T}^{e #mu} [GeV]; Events", 50, 0, 450);

// Dilepton eta histograms
TH1F *hist_lep_eta = new TH1F("hist_lep_eta", "|#eta| distribution of the dilepton system; |#eta_{e #mu}|; Events", 25, 0, 2.5);

// Scale factors histograms

TH1F *hist_ScaleFactor_PILEUP = new TH1F("hist_ScaleFactor_PILEUP", "Scale Factor for PILEUP; ; Events / 0.1 bins", 100, -1, 9);
TH1F *hist_ScaleFactor_BTAG = new TH1F("hist_ScaleFactor_BTAG", "Scale Factor for BTAG; ; Events / 0.05 bins", 100, -1, 4);
TH1F *hist_ScaleFactor_ELE = new TH1F("hist_ScaleFactor_ELE", "Scale Factor for ELE; ; Events / 0.05 bins", 100, -1, 4);
TH1F *hist_ScaleFactor_MUON = new TH1F("hist_ScaleFactor_MUON", "Scale Factor for MUON; ; Events / 0.05 bins", 100, -1, 4);
TH1F *hist_ScaleFactor_PHOTON = new TH1F("hist_ScaleFactor_PHOTON", "Scale Factor for PHOTON; ; Events / 0.05 bins", 100, -1, 4);
TH1F *hist_ScaleFactor_TAU = new TH1F("hist_ScaleFactor_TAU", "Scale Factor for TAU; ; Events / 0.05 bins", 100, -1, 4);
