#ifndef TEXTUREHANDLING_H_
#define TEXTUREHANDLING_H_

#include "ColorRGB.h"
#include "Image.h"

//Converts aImage to a 2D texture and loads it in OpenGL.
//aImage can be safely deleted from the application after
//the texture has been created.
//
//The returned value is the OpenGL identifier for the texture.
unsigned int LoadTexture(const Image<ColorRGB> aImage);

//Deletes the OpenGL texture designated by aTextureId from
//memory. Do no reuse aTextureId after deleting the texture.
void FreeTexture(unsigned int aTextureId);

#endif //TEXTUREHANDLING_H_