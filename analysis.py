import os
import subprocess

# List of root files to analyze

sample_paths = [
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-data-samples/data15_allyear.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-data-samples/data16_allyear_A.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-data-samples/data16_allyear_B.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-data-samples/data16_allyear_C.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-data-samples/data16_allyear_D.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-data-samples/data16_allyear_E.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-data-samples/data16_allyear_F.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-data-samples/data16_allyear_G.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-data-samples/data16_allyear_H.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/ttbar.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/mc20_13TeV.410644.PowhegPythia8EvtGen_A14_singletop_schan_lept_top.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/mc20_13TeV.410645.PowhegPythia8EvtGen_A14_singletop_schan_lept_antitop.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/mc20_13TeV.410659.PhPy8EG_A14_tchan_BW50_lept_antitop.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/mc20_13TeV.601352.PhPy8EG_tW_dyn_DR_incl_antitop.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/mc20_13TeV.601353.PhPy8EG_tW_dyn_DR_dil_antitop.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/mc20_13TeV.601354.PhPy8EG_tW_dyn_DR_dil_top.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/mc20_13TeV.601355.PhPy8EG_tW_dyn_DR_incl_top.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/mc20_13TeV.410218.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttee.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/mc20_13TeV.410219.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttmumu.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ntuples-mc-samples/mc-samples/mc20_13TeV.410658.PhPy8EG_A14_tchan_BW50_lept_top.root"
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
