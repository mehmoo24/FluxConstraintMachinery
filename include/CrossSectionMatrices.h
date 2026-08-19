// CrossSectionMatrices.h

#ifndef CROSSSECTIONMATRICES_H
#define CROSSSECTIONMATRICES_H

#include "NeutrinoFlavour.h"
#include "MatrixBuilder.h"

class CrossSectionMatrices{
   public:
      CrossSectionMatrices();

      MatrixK get(NeutrinoFlavour flavour);

   private:
      MatrixK _NuMu;
      MatrixK _NuMuBar;
      MatrixK _NuE;
      MatrixK _NuEBar;

} // end Class CrossSectionMatrices
