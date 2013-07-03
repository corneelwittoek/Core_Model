//---------------------------------------------------------------------------

#ifndef NCIMasterGrowthH
#define NCIMasterGrowthH
//---------------------------------------------------------------------------
#include "GrowthBase.h"
#include "NCIEffectsList.h"

class clTree;
class clTreePopulation;
class clShadingEffectBase;
class clDamageEffectBase;
class clSizeEffectBase;
class clNCITermBase;
class clCrowdingEffectBase;
class clPrecipitationEffectBase;
class clTemperatureEffectBase;
class clNitrogenEffectBase;

/**
* NCI growth - Version 3.0
*
* This is a growth shell object which applies the NCI (neighborhood competition
* index) function. The basic function is a maximum rate of growth that is
* reduced by a set of multiplicative effects. The set of effects used is up
* to the user.
*
* The amount of growth is in cm/year. For multi-year timesteps, the behavior
* will calculate total growth with a loop. Each loop iteration will increment
* DBH for one year. For each year, effects are recalculated with the previous
* year's new DBH value. All values for each year of growth are summed to get
* the growth for the timestep.
*
* The parameter file call string for this to be diameter-incrementing with
* auto-height updating is "NCIMasterGrowth"; for diameter-only incrementing, use
* "NCIMasterGrowth diam only". The namestring for this behavior is
* "ncigrowthshell". The XML string is "NCIMasterGrowth".
*
* Copyright 2012 Charles D. Canham.
* @author Lora E. Murphy
*
* <br>Edit history:
* <br>-----------------
* <br>November 27, 2012: Created (LEM)
*/
class clNCIMasterGrowth : virtual public clGrowthBase {
//note: need the virtual keyword to avoid base class ambiguity.

  public:

  /**
  * Constructor.
  * @param p_oSimManager Sim Manager object.
  */
   clNCIMasterGrowth(clSimManager *p_oSimManager);

  /**
  * Destructor.
  */
  ~clNCIMasterGrowth();

  /**
  * Returns the value in the tree's float data member that holds the value
  * that was calculated by PreGrowthCalcs().
  *
  * @param p_oTree Tree to which to apply growth.
  * @param p_oPop Tree population object.
  * @param fHeightGrowth Amount of height growth, in m (ignored).
  * @return Amount of diameter growth, in cm.
  */
  float CalcDiameterGrowthValue(clTree *p_oTree, clTreePopulation *p_oPop, float fHeightGrowth);

  /**
  * Calculates growth for all NCI trees.  The values are stashed in the
  * "Growth" tree float data member for later application.
  *
  * Steps:
  * <ol>
  * <li>Get all trees for this behavior.</li>
  * <li>For each tree, calculate NCI<sub>i</sub> by calling the function in the
  * function pointer NCI.  Stash the value in "Growth" for each tree.
  * On the way, pick up the max NCI<sub>i</sub> value for each species
  * (NCI<sub>max</sub>) if it is being used (if m_bUseNciMax = true).</li>
  * <li>Go through all the NCI trees again.  Calculate the amount of growth for
  * each using the equations above.  Use the function pointers to make sure
  * that the proper function forms are used.  Stash the end result in
  * "Growth".</li>
  * </ol>
  * This must be called first of any growth stuff, since it uses other trees'
  * DBHs to calculate NCI, and these must be before growth has been applied.
  *
  * Growth per timestep is calculated by looping over the number of years
  * per timestep and incrementing the DBH.
  *
  * @param p_oPop Tree population object.
  */
  void PreGrowthCalcs( clTreePopulation *p_oPop );

  /**
  * Does setup.
  * <ol>
  * <li>AssembleUniqueTypes() is called to create a list of unique behavior
  * types.</li>
  * <li>ReadParameterFile() is called to read the parameter file's data.</li>
  * <li>ValidateData() is called to validate the data.</li>
  * <li>GetTreeMemberCodes() is called to get tree data return codes.</li>
  * <li>SetFunctionPointers() is called to set up our function pointers.</li>
  * </ol>
  *
  * @param p_oDoc DOM tree of parsed input tree.
  */
  void DoShellSetup(xercesc::DOMDocument *p_oDoc);

  /**
  * Captures the namestring passed to this behavior. This is overridden from
  * clBehaviorBase so we can capture the namestring passed. Since this class
  * can create multiple kinds of behaviors that function differently, this will
  * capture what kind of behavior this is supposed to be.
  *
  * @param sNameString Behavior's namestring.
  */
  void SetNameData(std::string sNameString);

  protected:

  clShadingEffectBase *mp_oShadingEffect;
  clDamageEffectBase *mp_oDamageEffect;
  clSizeEffectBase *mp_oSizeEffect;
  clCrowdingEffectBase *mp_oCrowdingEffect;
  clNCITermBase *mp_oNCITerm;
  clPrecipitationEffectBase *mp_oPrecipEffect;
  clTemperatureEffectBase *mp_oTempEffect;
  clNitrogenEffectBase *mp_oNEffect;

  /**Search query for behavior trees.*/
  std::string m_sQuery;

  /**Maximum growth value. Array sized number of species.*/
  float *mp_fMaxPotentialValue;

  short int **mp_iGrowthCodes; /**<Holds return data codes for the "Growth"
  tree data member.  Array size is number of species to which this behavior
  applies by 2 (saplings and adults).*/
  short int m_iNumTotalSpecies; /**<Total number of species - for the destructor */

  /**
  * Makes sure all input data is valid. Max growth for each species must be > 0.
  * @throws modelErr if max growth for any species is < 0.
  */
  void ValidateData();

  /**
  * Gets the return codes for needed tree data members.
  * @throws modelErr if a code comes back -1 for any species/type combo to
  * which this behavior is applied.
  */
  void GetTreeMemberCodes();

  /**
  * Reads data from the parameter file.
  * @param p_oDoc DOM tree of parsed input tree.
  * @throws modelErr if this behavior has been applied to any types except
  * sapling and adult, or if any of the effect terms is not recognized.
  */
  void ReadParameterFile( xercesc::DOMDocument *p_oDoc );

  /**
   * Formats the string in m_sQuery.  This value will be used in Action() to
   * pass to clTreePopulation::Find() in order to get the trees to act on.
   */
  void FormatQueryString();
};
//---------------------------------------------------------------------------
#endif
