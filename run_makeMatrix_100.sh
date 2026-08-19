#file_input = sys.argv[1] # MUST BE NEUTRINO
#file_histo = sys.argv[2]
#title = sys.argv[3]
#file_inputB = sys.argv[4] # MUST BE ANTINEUTRINO
#vertErrorBandName = sys.argv[5]

python makeMatrix_100.py /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/EeDistribution_ME_FHC.root kEeDistr_TOTAL EeDistribution_ME_FHC  /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/EeDistribution_ME_FHC.root Flux

python makeMatrix_100.py /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/EeDistribution_ME_RHC.root kEeDistr_TOTAL EeDistribution_ME_RHC  /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/EeDistribution_ME_RHC.root Flux

python makeMatrix_100.py /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/EeDistribution_LE_FHC.root kEeDistr_TOTAL EeDistribution_LE_FHC  /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/EeDistribution_LE_FHC.root Flux

python makeMatrix_100.py /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/EeDistribution_LE_RHC.root kEeDistr_TOTAL EeDistribution_LE_RHC  /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/EeDistribution_LE_RHC.root Flux
