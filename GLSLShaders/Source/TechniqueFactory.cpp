
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#include "TechniqueFactory.h"

#include <cstdlib>

#include "TechniqueToon.h"
#include "TechniqueBarla.h"
#include "TechniqueGeoFlat.h"
#include "TechniquePhong.h"
#include "TechniqueTwist.h"
#include "TechniqueMultiTex.h"
#include "TechniqueBonus.h"

RenderingTechnique* TechniqueFactory::CreateTechnique(Technique aTech)
{
    switch(aTech)
    {
    case Technique_ToonShader :
        return new TechniqueToon();
    case Technique_BarlaShader :
        return new TechniqueBarla();
    case Technique_PlainColor :
        return new TechniqueGeoFlat();
    case Technique_FixedPipeline :
        return new TechniquePhong();
    case Technique_Twist :
        return new TechniqueTwist();
    case Technique_MultiTexture :
        return new TechniqueMultiTex();
    case Technique_Bonus :
		return new TechniqueBonus();
    default:
        return NULL;
    }
}