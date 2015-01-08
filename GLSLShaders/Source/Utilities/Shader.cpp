#include "Shader.h"

#include <fstream>

Shader::Shader(Shader::ShaderType aType)
: mType(aType)
, mShaderId(0)
{}

bool Shader::CompileSourceFile(const string& aFileName)
{
    //Delete previous shader if the class already contained one.
    if(mShaderId != 0)
    {
        glDeleteShader(mShaderId);
    }

    //Open and read the shader file.
    fstream ShaderFile;
    ShaderFile.open(aFileName.c_str());

    string ShaderCode(  (std::istreambuf_iterator<char>(ShaderFile)), 
                         std::istreambuf_iterator<char>()
                        );

    ShaderFile.close();

    //Create the shader object
    if(mType == Shader_Vertex)
    {
        mShaderId = glCreateShader(GL_VERTEX_SHADER);              
    }
    else if(mType == Shader_Fragment)
    {
        mShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    }
    
    //Associate the shader's source code to the shader object.
    const char* ShaderChars = ShaderCode.c_str();
    glShaderSource(mShaderId,1,&ShaderChars,NULL);
    
    //Compile the shader
    glCompileShader(mShaderId);
      
    mShaderId = mShaderId;

    return IsCompiled();
}

string Shader::Log() const
{
    int LogLength = 0;
    glGetShaderiv(mShaderId,GL_INFO_LOG_LENGTH,&LogLength);
    
    char *Log = new char[LogLength];
    
    int ReturnedLength = 0;
    glGetShaderInfoLog(mShaderId,LogLength,&ReturnedLength,Log);
    
    string RetStr(Log);
    delete[] Log;
    
    return RetStr;
}

Shader::~Shader()
{
    if(mShaderId != 0)
    {
        glDeleteShader(mShaderId);        
    }
}
