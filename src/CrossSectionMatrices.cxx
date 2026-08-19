#include "../include/CrossSectionMatrices.h"


CrossSectionMatrices::CrossSectionMatrices(){




} // end constructor


MatrixK CrossSectionMatrices::get(NeutrinoFlavour flavour){
   switch(flavour){
      case NeutrinoFlavour::NuMu:
         return _NuMu;
      case NeutrinoFlavour::NuMuBar:
         return _NuMuBar;
      case NeutrinoFlavour::NuE:
         return _NuE;
      case NeutrinoFlavour::NuEBar:
         return _NuEBar;
   } // end switch
}
