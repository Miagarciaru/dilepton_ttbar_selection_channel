import os
import subprocess

# List of root files to analyze

base_path = "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/"

sample_paths = [
    base_path + "mc20_13TeV.364302.Sherpa_222_NNPDF30NNLO_ggZllZqq.root",
    base_path + "mc20_13TeV.364303.Sherpa_222_NNPDF30NNLO_ggZvvZqq.root",
    base_path + "mc20_13TeV.364304.Sherpa_222_NNPDF30NNLO_ggWmlvWpqq.root",
    base_path + "mc20_13TeV.364305.Sherpa_222_NNPDF30NNLO_ggWplvWmqq.root",
    base_path + "mc20_13TeV.410155.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttW.root",
    base_path + "mc20_13TeV.410156.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttZnunu.root",
    base_path + "mc20_13TeV.410157.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttZqq.root",
    base_path + "mc20_13TeV.410218.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttee.root",
    base_path + "mc20_13TeV.410219.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttmumu.root",
    base_path + "mc20_13TeV.410220.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_tttautau.root",
    base_path + "mc20_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.root",
    base_path + "mc20_13TeV.410471.PhPy8EG_A14_ttbar_hdamp258p75_allhad.root",
    base_path + "mc20_13TeV.410472.PhPy8EG_A14_ttbar_hdamp258p75_dil.root",
    base_path + "mc20_13TeV.410644.PowhegPythia8EvtGen_A14_singletop_schan_lept_top.root",
    base_path + "mc20_13TeV.410645.PowhegPythia8EvtGen_A14_singletop_schan_lept_antitop.root",
    base_path + "mc20_13TeV.410646.PowhegPythia8EvtGen_A14_Wt_DR_inclusive_top.root",
    base_path + "mc20_13TeV.410647.PowhegPythia8EvtGen_A14_Wt_DR_inclusive_antitop.root",
    base_path + "mc20_13TeV.410648.PowhegPythia8EvtGen_A14_Wt_DR_dilepton_top.root",
    base_path + "mc20_13TeV.410649.PowhegPythia8EvtGen_A14_Wt_DR_dilepton_antitop.root",
    base_path + "mc20_13TeV.410658.PhPy8EG_A14_tchan_BW50_lept_top.root",
    base_path + "mc20_13TeV.410659.PhPy8EG_A14_tchan_BW50_lept_antitop.root",
    base_path + "mc20_13TeV.601352.PhPy8EG_tW_dyn_DR_incl_antitop.root",
    base_path + "mc20_13TeV.601353.PhPy8EG_tW_dyn_DR_dil_antitop.root",
    base_path + "mc20_13TeV.601354.PhPy8EG_tW_dyn_DR_dil_top.root",
    base_path + "mc20_13TeV.601355.PhPy8EG_tW_dyn_DR_incl_top.root",
    base_path + "mc20_13TeV.700320.Sh_2211_Zee_maxHTpTV2_BFilter.root",
    base_path + "mc20_13TeV.700321.Sh_2211_Zee_maxHTpTV2_CFilterBVeto.root",
    base_path + "mc20_13TeV.700322.Sh_2211_Zee_maxHTpTV2_CVetoBVeto.root",
    base_path + "mc20_13TeV.700323.Sh_2211_Zmumu_maxHTpTV2_BFilter.root",
    base_path + "mc20_13TeV.700324.Sh_2211_Zmumu_maxHTpTV2_CFilterBVeto.root",
    base_path + "mc20_13TeV.700325.Sh_2211_Zmumu_maxHTpTV2_CVetoBVeto.root",
    base_path + "mc20_13TeV.700338.Sh_2211_Wenu_maxHTpTV2_BFilter.root",
    base_path + "mc20_13TeV.700339.Sh_2211_Wenu_maxHTpTV2_CFilterBVeto.root",
    base_path + "mc20_13TeV.700340.Sh_2211_Wenu_maxHTpTV2_CVetoBVeto.root",
    base_path + "mc20_13TeV.700341.Sh_2211_Wmunu_maxHTpTV2_BFilter.root",
    base_path + "mc20_13TeV.700342.Sh_2211_Wmunu_maxHTpTV2_CFilterBVeto.root",
    base_path + "mc20_13TeV.700344.Sh_2211_Wtaunu_L_maxHTpTV2_BFilter.root",
    base_path + "mc20_13TeV.700345.Sh_2211_Wtaunu_L_maxHTpTV2_CFilterBVeto.root",
    base_path + "mc20_13TeV.700346.Sh_2211_Wtaunu_L_maxHTpTV2_CVetoBVeto.root",
    base_path + "mc20_13TeV.700347.Sh_2211_Wtaunu_H_maxHTpTV2_BFilter.root",
    base_path + "mc20_13TeV.700348.Sh_2211_Wtaunu_H_maxHTpTV2_CFilterBVeto.root",
    base_path + "mc20_13TeV.700349.Sh_2211_Wtaunu_H_maxHTpTV2_CVetoBVeto.root",
    base_path + "mc20_13TeV.700488.Sh_2211_WlvWqq.root",
    base_path + "mc20_13TeV.700489.Sh_2211_WlvZqq.root",
    base_path + "mc20_13TeV.700490.Sh_2211_WlvZbb.root",
    base_path + "mc20_13TeV.700493.Sh_2211_ZqqZll.root",
    base_path + "mc20_13TeV.700494.Sh_2211_ZbbZll.root",
    base_path + "mc20_13TeV.700495.Sh_2211_ZqqZvv.root",
    base_path + "mc20_13TeV.700496.Sh_2211_ZbbZvv.root",
    base_path + "mc20_13TeV.700600.Sh_2212_llll.root",
    base_path + "mc20_13TeV.700601.Sh_2212_lllv.root",
    base_path + "mc20_13TeV.700602.Sh_2212_llvv_os.root",
    base_path + "mc20_13TeV.700603.Sh_2212_llvv_ss.root",
    base_path + "mc20_13TeV.700604.Sh_2212_lvvv.root",
    base_path + "mc20_13TeV.700605.Sh_2212_vvvv.root",
    base_path + "mc20_13TeV.700792.Sh_2214_Ztautau_maxHTpTV2_BFilter.root"
]

# Root folder for storing output
output_root_folder = "output_analysis"
output_plots_folder = "distribution_plots"

# Ensure the root folder exists
os.makedirs(output_root_folder, exist_ok=True)
os.makedirs(output_plots_folder, exist_ok=True)

# Loop over each sample
for sample in sample_paths:
    # Get the sample name (useful for naming the output folder)
    sample_name = os.path.basename(sample).removesuffix('.root')
        
    # Construct the ROOT script command

    root_command = f'root -l -q \'ttbar_selection.C("{sample}", "{sample_name}")\''

    # Run the ROOT script
    subprocess.run(root_command, shell=True)
