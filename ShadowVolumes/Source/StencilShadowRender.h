
/*
  Marc-Antoine Desbiens
  November 2012
*/

#ifndef STENCILSHADOWRENDER_H_
#define STENCILSHADOWRENDER_H_

#include <iostream>
#include "Model.h"
#include "Utilities/GLInclude.h"
#include "Utilities/Shader.h"
#include "Utilities/ShaderProgram.h"

//Called once when the program launches.
void InitRendering();

//Called once every frame
void RenderScene(Model& aShadowCaster, Model& aSceneGeometry);

#endif //STENCILSHADOWRENDER_H_
