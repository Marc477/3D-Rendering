#include "TextureHandling.h"

#include "GLInclude.h"

unsigned int LoadTexture(const Image<ColorRGB> aImage)
{
    unsigned int RetVal;

    glGenTextures(1,&RetVal);
    glBindTexture(GL_TEXTURE_2D,RetVal);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        3,
        aImage.GetWidth(),
        aImage.GetHeight(),
        0,
        GL_RGB,
        GL_FLOAT,
        aImage.GetRasterData());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D,NULL);

    return RetVal;
}

//////////////////////////////////////////////////////////////////////////

void FreeTexture(unsigned int aTextureId)
{
    unsigned int TmpId = aTextureId;
    glDeleteTextures(1,&TmpId);
}