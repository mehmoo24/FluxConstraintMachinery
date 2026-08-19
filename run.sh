POT_FHC_ME=1.16e21 # Valencia paper
POT_RHC_ME=1.12e21 # Luis' paper
POT_FHC_LE=3.32e+20 # from tuples
POT_RHC_LE=1.03e+20 # from tuples

./make_xsec_matrices Fluxes_Quinn_LE_FHC.root    /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/ EeDistribution_LE_FHC ${POT_FHC_LE}

./make_xsec_matrices Fluxes_Quinn_LE_RHC.root    /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/ EeDistribution_LE_RHC ${POT_RHC_LE}

./make_xsec_matrices Fluxes_FRW_ME_FHC.root    /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/ EeDistribution_ME_FHC ${POT_FHC_ME}

./make_xsec_matrices Fluxes_FRW_ME_RHC.root    /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/ EeDistribution_ME_RHC ${POT_RHC_ME}


# make the plots
#python plotEe.py EeDistribution_LE_FHC.root LE_FHC
#python plotEe.py EeDistribution_LE_RHC.root LE_RHC
#python plotEe.py EeDistribution_ME_FHC.root ME_FHC
#python plotEe.py EeDistribution_ME_RHC.root ME_RHC
