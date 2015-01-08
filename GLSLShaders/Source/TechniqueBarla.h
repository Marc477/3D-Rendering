
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#ifndef TECHNIQUEBARLA_H_
#define TECHNIQUEBARLA_H_

#include "RenderingTechnique.h"
#include "ShaderProgram.h"

//Simple toon shader rendering technique based
//on a 1D lookup. See implementation (.cpp) for details.
class TechniqueBarla : public RenderingTechnique
{
public:

    TechniqueBarla();
    virtual ~TechniqueBarla();

    virtual void PreRender();
    virtual void PostRender();
    virtual void ReceiveInput(char aKey);

protected:

    void InitTextures();
    void InitShader();

    unsigned int mCurrentLookup;
    unsigned int mLookups[3];

    ShaderProgram mShader;
};

#endif //TECHNIQUETOON_H_
