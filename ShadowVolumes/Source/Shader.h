/*
 *  Shader.h
 *  TP2
 *
 *  Created by Olivier Vaillancourt on 10-10-25.
 *  Copyright 2010 Université de Sherbrooke. All rights reserved.
 *
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <string>

#include "GLInclude.h"

using namespace std;

class Shader
{
public:
    
    enum ShaderType
    {
        Shader_Vertex,
        Shader_Fragment,
    };
    
    Shader(ShaderType aType);
    virtual ~Shader();
    
    //Compiles the source code of the shader located in file aFileName.
    //Returns true if the shader compiled successfully, false otherwise.
    bool CompileSourceFile(const string& aFileName);
    
    //Returns true if the shader has been succcessfully compiled once
    //and can be integrated in a shader program.
    bool IsCompiled() const;
    
    //Returns the compilation log for this shader.
    string Log() const;
    
    //Returns the OpenGL shader Id for this shader.
    unsigned int ShaderId() const;
    
    //Returns the shader type of this given shader object.
    ShaderType Type() const;
    
protected:
    
    ShaderType      mType;
    unsigned int    mShaderId;    
};

inline bool Shader::IsCompiled() const
{
    int CompilationStatus = 0;
    glGetShaderiv(mShaderId,GL_COMPILE_STATUS,&CompilationStatus);

    return CompilationStatus == GL_TRUE;
}

inline unsigned int Shader::ShaderId() const
{
    return mShaderId;
}

inline Shader::ShaderType Shader::Type() const
{
    return mType;
}

#endif //SHADER_H_
