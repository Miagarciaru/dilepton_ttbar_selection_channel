import os
import subprocess

# List of root files to analyze

sample_paths = [
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/data15_allyear.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/data16_allyear_A.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/data16_allyear_B.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/data16_allyear_C.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/data16_allyear_D.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/data16_allyear_E.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/data16_allyear_F.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/data16_allyear_G.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/data16_allyear_H.root",
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ttbar.root"
]

"""
sample_paths = [
    "/eos/user/g/garciarm/ntuple-production-samples/data15-16-ttbar/ttbar.root"
]
"""
# Root folder for storing output
output_root_folder = "output_analysis"

# Ensure the root folder exists
os.makedirs(output_root_folder, exist_ok=True)

# Loop over each sample
for sample in sample_paths:
    # Get the sample name (useful for naming the output folder)
    sample_name = os.path.basename(sample).removesuffix('.root')
        
    # Construct the ROOT script command

    root_command = f'root -l -q \'ttbar_selection.C("{sample}", "{sample_name}")\''

    # Run the ROOT script
    subprocess.run(root_command, shell=True)
