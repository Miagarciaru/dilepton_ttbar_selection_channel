#include "Plotting.h"

void Plotting(){

  // Files in the output analysis folder
  std::vector<string> files = {"data15_allyear", "data16_allyear_A", "data16_allyear_B", "data16_allyear_C", "data16_allyear_D", "data16_allyear_E", "data16_allyear_F", "data16_allyear_G", "data16_allyear_H", "ttbar"};

  // Specify histogram names to plot                                                                                                        
  std::vector<string> histNames = {"hist_met", "hist_lep_pt", "hist_lep_eta"};

  CreateComparisonPlots(files, histNames);
  
}
