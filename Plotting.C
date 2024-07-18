#include "Plotting.h"

void Plotting(){

  // Files in the output analysis folder
  std::vector<string> files_data = {"data15_allyear", "data16_allyear_A", "data16_allyear_B", "data16_allyear_C", "data16_allyear_D", "data16_allyear_E", "data16_allyear_F", "data16_allyear_G", "data16_allyear_H"};

  std::vector<string> files_ttbar = {
    "mc20_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad",
    //"mc20_13TeV.410471.PhPy8EG_A14_ttbar_hdamp258p75_allhad",
    //"mc20_13TeV.410472.PhPy8EG_A14_ttbar_hdamp258p75_dil"
  };
  
  std::vector<string> files_single_top = {
    "mc20_13TeV.410644.PowhegPythia8EvtGen_A14_singletop_schan_lept_top",
    "mc20_13TeV.410645.PowhegPythia8EvtGen_A14_singletop_schan_lept_antitop",
    "mc20_13TeV.410646.PowhegPythia8EvtGen_A14_Wt_DR_inclusive_top",
    "mc20_13TeV.410647.PowhegPythia8EvtGen_A14_Wt_DR_inclusive_antitop",
    "mc20_13TeV.410648.PowhegPythia8EvtGen_A14_Wt_DR_dilepton_top",
    "mc20_13TeV.410649.PowhegPythia8EvtGen_A14_Wt_DR_dilepton_antitop",
    "mc20_13TeV.410658.PhPy8EG_A14_tchan_BW50_lept_top",
    "mc20_13TeV.410659.PhPy8EG_A14_tchan_BW50_lept_antitop",
    //"mc20_13TeV.601352.PhPy8EG_tW_dyn_DR_incl_antitop",
    "mc20_13TeV.601353.PhPy8EG_tW_dyn_DR_dil_antitop",
    "mc20_13TeV.601354.PhPy8EG_tW_dyn_DR_dil_top"
    //"mc20_13TeV.601355.PhPy8EG_tW_dyn_DR_incl_top"
  };

  std::vector<string> files_diboson = {
    "mc20_13TeV.700600.Sh_2212_llll",
    "mc20_13TeV.700601.Sh_2212_lllv",
    "mc20_13TeV.700602.Sh_2212_llvv_os",
    "mc20_13TeV.700603.Sh_2212_llvv_ss",
    "mc20_13TeV.700604.Sh_2212_lvvv",
    "mc20_13TeV.700605.Sh_2212_vvvv",
    "mc20_13TeV.700488.Sh_2211_WlvWqq",
    "mc20_13TeV.700489.Sh_2211_WlvZqq",
    "mc20_13TeV.700490.Sh_2211_WlvZbb",
    "mc20_13TeV.700493.Sh_2211_ZqqZll",
    "mc20_13TeV.700494.Sh_2211_ZbbZll",
    "mc20_13TeV.700495.Sh_2211_ZqqZvv",
    "mc20_13TeV.700496.Sh_2211_ZbbZvv",
    "mc20_13TeV.364302.Sherpa_222_NNPDF30NNLO_ggZllZqq",
    "mc20_13TeV.364303.Sherpa_222_NNPDF30NNLO_ggZvvZqq",
    "mc20_13TeV.364304.Sherpa_222_NNPDF30NNLO_ggWmlvWpqq",
    "mc20_13TeV.364305.Sherpa_222_NNPDF30NNLO_ggWplvWmqq"
  };

  std::vector<string> files_W_jets = {
    "mc20_13TeV.700338.Sh_2211_Wenu_maxHTpTV2_BFilter",
    "mc20_13TeV.700339.Sh_2211_Wenu_maxHTpTV2_CFilterBVeto",
    "mc20_13TeV.700340.Sh_2211_Wenu_maxHTpTV2_CVetoBVeto",
    "mc20_13TeV.700341.Sh_2211_Wmunu_maxHTpTV2_BFilter",
    "mc20_13TeV.700342.Sh_2211_Wmunu_maxHTpTV2_CFilterBVeto",
    //"mc20_13TeV.700343.Sh_2211_Wmunu_maxHTpTV2_CVetoBVeto",
    "mc20_13TeV.700344.Sh_2211_Wtaunu_L_maxHTpTV2_BFilter",
    "mc20_13TeV.700345.Sh_2211_Wtaunu_L_maxHTpTV2_CFilterBVeto",
    "mc20_13TeV.700346.Sh_2211_Wtaunu_L_maxHTpTV2_CVetoBVeto",
    "mc20_13TeV.700347.Sh_2211_Wtaunu_H_maxHTpTV2_BFilter",
    "mc20_13TeV.700348.Sh_2211_Wtaunu_H_maxHTpTV2_CFilterBVeto",
    "mc20_13TeV.700349.Sh_2211_Wtaunu_H_maxHTpTV2_CVetoBVeto"
  };
  
  std::vector<string> files_Z_jets = {
    "mc20_13TeV.700320.Sh_2211_Zee_maxHTpTV2_BFilter",
    "mc20_13TeV.700321.Sh_2211_Zee_maxHTpTV2_CFilterBVeto",
    "mc20_13TeV.700322.Sh_2211_Zee_maxHTpTV2_CVetoBVeto",
    "mc20_13TeV.700323.Sh_2211_Zmumu_maxHTpTV2_BFilter",
    "mc20_13TeV.700324.Sh_2211_Zmumu_maxHTpTV2_CFilterBVeto",
    "mc20_13TeV.700325.Sh_2211_Zmumu_maxHTpTV2_CVetoBVeto",
    "mc20_13TeV.700792.Sh_2214_Ztautau_maxHTpTV2_BFilter"
  };
  
  std::vector<string> files_tt_others = {
    "mc20_13TeV.410155.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttW",
    "mc20_13TeV.410156.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttZnunu",
    "mc20_13TeV.410157.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttZqq",
    "mc20_13TeV.410218.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttee",
    "mc20_13TeV.410219.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttmumu",
    "mc20_13TeV.410220.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_tttautau"
  };
  
  
  // Specify histogram names to plot                                                                                                        
  std::vector<string> histNames = {"hist_met", "hist_lep_pt", "hist_lep_eta"};

  CreateComparisonPlots(files_data, files_ttbar, files_single_top, files_diboson, files_W_jets, files_Z_jets, files_tt_others, histNames);
  
}
