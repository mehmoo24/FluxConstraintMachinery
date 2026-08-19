//NeutrinoFlavour.h

#ifndef NEUTRINOFLAVOUR_H
#define NEUTRINOFLAVOUR_H

// we're creating an enum class: the variable can only represent one value from the specified list of values
enum class NeutrinoFlavour{
   NuMu,
   NuMuBar,
   NuE,
   NuEBar
};

// can use this like: NeutrinoFlavour n = NeutrinoFlavour::NuMu
// Don't create member functions of enum class
// Instead create free functions that take the enum as an argument

// func defined inside header so needs to be constexpr otherwise will run   into linker issues
constexpr int GetPDG(NeutrinoFlavour flavour){
   switch(flavour){
      case NeutrinoFlavour::NuMu:
         return 14;
      case NeutrinoFlavour::NuMuBar:
         return -14;
      case NeutrinoFlavour::NuE:
         return 12;
      case NeutrinoFlavour::NuEBar:
         return -12;
   }
   return 0;
}


#endif
