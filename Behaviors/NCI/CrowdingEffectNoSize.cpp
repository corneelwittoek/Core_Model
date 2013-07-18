#include "CrowdingEffectNoSize.h"
#include "ParsingFunctions.h"
#include "Tree.h"
#include "TreePopulation.h"
#include "BehaviorBase.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////
clCrowdingEffectNoSize::clCrowdingEffectNoSize() {
  mp_fC = NULL;
  mp_fD = NULL;
}

//////////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////////
clCrowdingEffectNoSize::~clCrowdingEffectNoSize() {
  delete[] mp_fC;
  delete[] mp_fD;
}

//////////////////////////////////////////////////////////////////////////////
// CalculateCrowdingEffect
//////////////////////////////////////////////////////////////////////////////
float clCrowdingEffectNoSize::CalculateCrowdingEffect(clTree *p_oTree, float fDiam, float fNCI) {
  float fCrowdingEffect;
  int iSpecies = p_oTree->GetSpecies();
  //Avoid a domain error - if NCI is 0, return 1
  if ( fNCI > 0 ) {
    fCrowdingEffect = exp( -mp_fC[iSpecies] * pow( fNCI, mp_fD[iSpecies]));
    if ( fCrowdingEffect < 0 ) fCrowdingEffect = 0;
    if ( fCrowdingEffect > 1 ) fCrowdingEffect = 1;
    return fCrowdingEffect;
  }
  return 1.0;
}

//////////////////////////////////////////////////////////////////////////////
// DoSetup
//////////////////////////////////////////////////////////////////////////////
void clCrowdingEffectNoSize::DoSetup(clTreePopulation *p_oPop, clBehaviorBase *p_oNCI, xercesc::DOMElement *p_oElement) {
  floatVal * p_fTempValues; //for getting species-specific values
  int iNumBehaviorSpecies = p_oNCI->GetNumBehaviorSpecies(),
      iNumTotalSpecies = p_oPop->GetNumberOfSpecies(), i;

  mp_fC = new float[iNumTotalSpecies];
  mp_fD = new float[iNumTotalSpecies];

  //Set up our floatVal array that will extract values only for the species
  //assigned to this behavior
  p_fTempValues = new floatVal[iNumBehaviorSpecies];
  for ( i = 0; i < iNumBehaviorSpecies; i++ ) p_fTempValues[i].code = p_oNCI->GetBehaviorSpecies(i);

  //Crowding Slope (C)
  FillSpeciesSpecificValue( p_oElement, "nciCrowdingC", "nccVal", p_fTempValues,
      iNumBehaviorSpecies, p_oPop, true );
  //Transfer to the appropriate array buckets
  for ( i = 0; i < iNumBehaviorSpecies; i++ )
    mp_fC[p_fTempValues[i].code] = p_fTempValues[i].val;

  //Crowding Steepness (D)
  FillSpeciesSpecificValue( p_oElement, "nciCrowdingD", "ncdVal", p_fTempValues,
      iNumBehaviorSpecies, p_oPop, true );
  //Transfer to the appropriate array buckets
  for ( i = 0; i < iNumBehaviorSpecies; i++ )
    mp_fD[p_fTempValues[i].code] = p_fTempValues[i].val;

  delete[] p_fTempValues;
}
