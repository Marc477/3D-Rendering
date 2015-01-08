
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#ifndef TECHNIQUETWIST_H_
#define TECHNIQUETWIST_H_

#include "RenderingTechnique.h"
#include "ShaderProgram.h"

class TechniqueTwist : public RenderingTechnique
{
public:

    TechniqueTwist();
    virtual ~TechniqueTwist();

    virtual void PreRender();
    virtual void PostRender();
    virtual void ReceiveInput(char aKey);

protected:

    void InitShader();

    ShaderProgram mShader;

	float level;
};

#endif
