#include "Plotting.h"

void Plotting(){

  // Files in the output analysis folder
  std::vector<string> files = {"data15_allyear", "data16_allyear_A", "data16_allyear_B", "data16_allyear_C", "data16_allyear_D", "data16_allyear_E", "data16_allyear_F", "data16_allyear_G", "data16_allyear_H", "ttbar", "mc20_13TeV.410644.PowhegPythia8EvtGen_A14_singletop_schan_lept_top", "mc20_13TeV.410645.PowhegPythia8EvtGen_A14_singletop_schan_lept_antitop", "mc20_13TeV.410658.PhPy8EG_A14_tchan_BW50_lept_top", "mc20_13TeV.410659.PhPy8EG_A14_tchan_BW50_lept_antitop", "mc20_13TeV.601352.PhPy8EG_tW_dyn_DR_incl_antitop", "mc20_13TeV.601353.PhPy8EG_tW_dyn_DR_dil_antitop", "mc20_13TeV.601354.PhPy8EG_tW_dyn_DR_dil_top", "mc20_13TeV.601355.PhPy8EG_tW_dyn_DR_incl_top", "mc20_13TeV.410218.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttee", "mc20_13TeV.410219.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttmumu"};

  // Specify histogram names to plot                                                                                                        
  std::vector<string> histNames = {"hist_met", "hist_lep_pt", "hist_lep_eta"};

  CreateComparisonPlots(files, histNames);
  
}
