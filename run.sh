./make_xsec_matrices Fluxes_Quinn_LE_FHC.root    /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/ EeDistribution_LE_FHC

#./make_xsec_matrices Fluxes_Quinn_LE_RHC.root    /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/ EeDistribution_LE_RHC

#./make_xsec_matrices Fluxes_FRW_ME_FHC.root    /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/ EeDistribution_ME_FHC

#./make_xsec_matrices Fluxes_FRW_ME_RHC.root    /exp/minerva/app/users/mmehmood/MAT_AL9/ComputeFluxConstraint/ EeDistribution_ME_RHC


# make the plots
python plotEe.py EeDistribution_LE_FHC.root LE_FHC
#python plotEe.py EeDistribution_LE_RHC.root LE_RHC
#python plotEe.py EeDistribution_ME_FHC.root ME_FHC
#python plotEe.py EeDistribution_ME_RHC.root ME_RHC
