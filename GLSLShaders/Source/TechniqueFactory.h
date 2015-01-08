
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#ifndef TECHNIQUEFACTORY_H_
#define TECHNIQUEFACTORY_H_

#include "RenderingTechnique.h"

class TechniqueFactory
{
public:

    enum Technique
    {
        Technique_ToonShader,
        Technique_PlainColor,
        Technique_BarlaShader,
        Technique_FixedPipeline,
        Technique_Twist,
        Technique_MultiTexture,
        Technique_Bonus,

        Technique_COUNT,//Always keep last in enum.
    };

    //This static method returns a new instance of the 
    static RenderingTechnique* CreateTechnique(Technique aTech);
};

#endif //TECHNIQUEFACTORY_H_
