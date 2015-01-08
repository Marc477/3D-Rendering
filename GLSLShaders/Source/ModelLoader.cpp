#include "ModelLoader.h"

#include <cstring>
#include <iostream>
#include <cfloat>

#include "GLInclude.h"

ModelLoader::ModelLoader()
{}

unsigned int ModelLoader::Load(const string &aFileName,
                               bool aNormalizeMesh /*= false*/)
{
    ResetLoader();
    
    cout << "Loading mesh " << aFileName << " ... ";
    
    FILE* pFile = fopen(aFileName.c_str(),"rb");
    
    while(!feof(pFile))
    {
        DispatchRead(pFile);
    }
    
    if(mNormals.size() == 1)
    {
        GenerateNormals();
    }

    if(aNormalizeMesh)
    {
        NormalizeMesh();
    }

	
    unsigned int TmpRetVal = FillPrimitive();
    
    fclose(pFile);
    cout << "DONE." << endl;
    
    return TmpRetVal;
}

#define BUFFER_SIZE 512
void ModelLoader::DispatchRead(FILE* apFile)
{
    char tBuffer[BUFFER_SIZE];
    
    fscanf(apFile,"%s",tBuffer);
    
    if(!strcmp("v",tBuffer))
    {
        ReadVertexData(apFile);
    }
    else if(!strcmp("vt",tBuffer))
    {
        ReadTextureData(apFile);
    }
    else if(!strcmp("vn",tBuffer))
    {
        ReadNormalData(apFile);
    }
    else if(!strcmp("f",tBuffer))
    {
        ReadSurfaceData(apFile);
    }
    else
    {
        fgets(tBuffer,BUFFER_SIZE,apFile);
    }
}

void ModelLoader::ReadVertexData(FILE* apFile)
{
    Vector3 tmpVec;
    
    fscanf(apFile, "%f %f %f",&tmpVec.X(),&tmpVec.Y(),&tmpVec.Z());
    
    mVertices.push_back(tmpVec);
}

void ModelLoader::ReadNormalData(FILE* apFile)
{
    Vector3 tmpVec;
    
    fscanf(apFile, "%f %f %f",&tmpVec.X(),&tmpVec.Y(),&tmpVec.Z());
    
    mNormals.push_back(tmpVec);
}

void ModelLoader::ReadTextureData(FILE *apFile)
{
    Point2 tmpUV;
    float Unused;
    
    fscanf(apFile, "%f %f %f",&tmpUV.X(),&tmpUV.Y(),&Unused);
    
    mUVs.push_back(tmpUV);
}

void ModelLoader::ReadSurfaceData(FILE* apFile)
{
    vector<TmpTriplet> DataList;
    char tLineBuffer[512];
    char *TripletBuffer = NULL;
    TmpTriplet TmpData;
    
    fgets(tLineBuffer,512,apFile);
    
    TmpData.VxRef = 0;	
    TmpData.TexRef = 0;	
    TmpData.NormRef = 0;
    
    TripletBuffer = strtok(tLineBuffer," ");
    while(TripletBuffer)
    {
        int i = 0;
        int j = 0;
        
        int SlashIndexes[2];
        
        while(TripletBuffer[i] && j < 2)
        {
            if(TripletBuffer[i] == '/')
            {
                SlashIndexes[j++] = i;		
            }
            ++i;
        }
        
        switch(j)
        {
            case 2:
                TmpData.NormRef = atoi(&TripletBuffer[SlashIndexes[1]+1]);
            case 1:
                TmpData.TexRef = atoi(&TripletBuffer[SlashIndexes[0]+1]);
            case 0:
                TmpData.VxRef = atoi(&TripletBuffer[0]);
        }
        TripletBuffer = strtok(NULL," ");
        
        DataList.push_back(TmpData);
    }
    
    GenerateTriangles(DataList);
}

void ModelLoader::GenerateTriangles(std::vector<TmpTriplet> &aDataList)
{
    if(aDataList.size() < 3)
    {
        cout << "Not enough vertices, cannot generate a triangle." << endl;
        return;
        }
    else if(aDataList.size() == 3)
    {
        TmpTriangle Tri;
		
        Tri.Ref1 = aDataList[0].VxRef;
        Tri.Ref2 = aDataList[1].VxRef;
        Tri.Ref3 = aDataList[2].VxRef;
        
        if(aDataList[0].NormRef)
        {
            TmpTriangle NTri;
            
            NTri.Ref1 = aDataList[0].NormRef;
            NTri.Ref2 = aDataList[1].NormRef;
            NTri.Ref3 = aDataList[2].NormRef;
            
            mCombinedNormals.push_back(NTri);
        }
        
        TmpTriangle TTri;
        
        TTri.Ref1 = aDataList[0].TexRef;
        TTri.Ref2 = aDataList[1].TexRef;
        TTri.Ref3 = aDataList[2].TexRef;
        
        mCombinedUVs.push_back(TTri);
        
        mCombinedVertices.push_back(Tri);
    }
    else if(aDataList.size() == 4 )
    {
        TmpTriangle Tri1;
        TmpTriangle Tri2;
        
        Tri1.Ref1 = aDataList[0].VxRef;
        Tri1.Ref2 = aDataList[1].VxRef;
        Tri1.Ref3 = aDataList[2].VxRef;
        
        Tri2.Ref1 = aDataList[0].VxRef;
        Tri2.Ref2 = aDataList[2].VxRef;
        Tri2.Ref3 = aDataList[3].VxRef;
        
        mCombinedVertices.push_back(Tri1);
        mCombinedVertices.push_back(Tri2);
        
        if(aDataList[0].NormRef)
        {
            TmpTriangle NTri1;
            TmpTriangle NTri2;
            
            NTri1.Ref1 = aDataList[0].NormRef;
            NTri1.Ref2 = aDataList[1].NormRef;
            NTri1.Ref3 = aDataList[2].NormRef;
            
            NTri2.Ref1 = aDataList[0].NormRef;
            NTri2.Ref2 = aDataList[2].NormRef;
            NTri2.Ref3 = aDataList[3].NormRef;
            
            mCombinedNormals.push_back(NTri1);
            mCombinedNormals.push_back(NTri2);
        }
        
        TmpTriangle	TTri1;
        TmpTriangle TTri2;
        
        TTri1.Ref1 = aDataList[0].TexRef;
        TTri1.Ref2 = aDataList[1].TexRef;
        TTri1.Ref3 = aDataList[2].TexRef;
        
        TTri2.Ref1 = aDataList[0].TexRef;
        TTri2.Ref2 = aDataList[2].TexRef;
        TTri2.Ref3 = aDataList[3].TexRef;
        
        mCombinedUVs.push_back(TTri1);
        mCombinedUVs.push_back(TTri2);
    }
    else if(aDataList.size() > 4)
    {
        TmpTriangle Tri;
        TmpTriangle NTri;
        TmpTriangle TTri;
        
        Tri.Ref1 = aDataList[0].VxRef;
        NTri.Ref1 = aDataList[0].NormRef;
        TTri.Ref1 = aDataList[0].TexRef;
        
        for(unsigned int i = 1; i < aDataList.size()-1;++i)
        {
            Tri.Ref2 = aDataList[i].VxRef;
            Tri.Ref3 = aDataList[i+1].VxRef;
            
            if(aDataList[i].NormRef)
            {
                NTri.Ref2 = aDataList[i].NormRef;
                NTri.Ref3 = aDataList[i+1].NormRef;
            }
            
            TTri.Ref2 = aDataList[i].TexRef;
            TTri.Ref3 = aDataList[i+1].TexRef;
        }
        
        mCombinedVertices.push_back(Tri);
        if(aDataList[0].NormRef)
        {
            mCombinedNormals.push_back(NTri);
        }
		
        mCombinedUVs.push_back(TTri);
    }
}

void ModelLoader::GenerateNormals()
{
    mVertReferences.resize(mVertices.size());
    for(unsigned int i = 0; i < mCombinedVertices.size(); ++i)
    {
        //Assign every vertice to their respective triangle(s).
        mVertReferences[mCombinedVertices[i].Ref1].push_back(i);
        mVertReferences[mCombinedVertices[i].Ref2].push_back(i);
        mVertReferences[mCombinedVertices[i].Ref3].push_back(i);
    }
    
    vector<Vector3> FacesNormals(mCombinedVertices.size());
    
    for(unsigned int i = 0; i < mCombinedVertices.size(); ++i)
    {
        Vector3 VecL =  mVertices[mCombinedVertices[i].Ref1] - 
                        mVertices[mCombinedVertices[i].Ref2];
        Vector3 VecR =  mVertices[mCombinedVertices[i].Ref3] - 
                        mVertices[mCombinedVertices[i].Ref2];
        
        FacesNormals[i] = VecR.CrossProduct(VecL).NormalisedCopy();	
    }
    
    mNormals.resize(mVertices.size());
    mCombinedNormals.resize(mCombinedVertices.size());
    for(unsigned int i = 1; i < mVertices.size(); ++i)
    {
        Vector3 NormalSum(0,0,0);
        for(unsigned int j = 0; j < mVertReferences[i].size(); ++j)
        {
            NormalSum += FacesNormals[mVertReferences[i][j]];
            
            mCombinedNormals[mVertReferences[i][j]].Ref1 = 
                mCombinedVertices[mVertReferences[i][j]].Ref1;
            
            mCombinedNormals[mVertReferences[i][j]].Ref2 = 
                mCombinedVertices[mVertReferences[i][j]].Ref2;
            
            mCombinedNormals[mVertReferences[i][j]].Ref3 = 
                mCombinedVertices[mVertReferences[i][j]].Ref3;
        }
        
        NormalSum /= float(mVertReferences[i].size());
        mNormals[i] = NormalSum.NormalisedCopy();
    }
}

void ModelLoader::NormalizeMesh()
{
    float Min = FLT_MAX;
    float Max = FLT_MIN;

    for(unsigned int i = 0; i < mVertices.size(); ++i)
    {
        if(mVertices[i].X() < Min)
        {
            Min = mVertices[i].X();
        }
        else if(mVertices[i].X() > Max)
        {
            Max = mVertices[i].X();
        }

        if(mVertices[i].Y() < Min)
        {
            Min = mVertices[i].Y();
        }
        else if(mVertices[i].Y() > Max)
        {
            Max = mVertices[i].Y();
        }

        if(mVertices[i].Z() < Min)
        {
            Min = mVertices[i].Z();
        }
        else if(mVertices[i].Z() > Max)
        {
            Max = mVertices[i].Z();
        }
    }

    Vector3 Minima(Min,Min,Min);



    //Normalization
    for(unsigned int i = 0; i < mVertices.size(); ++i)
    {
        mVertices[i] = (mVertices[i] - Minima) / (Max-Min);
    }

    Vector3 MinVec(FLT_MAX,FLT_MAX,FLT_MAX);
    Vector3 MaxVec(FLT_MIN,FLT_MIN,FLT_MIN);

    //Find axis boundaries for centering
    for(unsigned int i = 0; i < mVertices.size(); ++i)
    {
        if(mVertices[i].X() < MinVec.X())
        {
            MinVec.X() = mVertices[i].X();
        }
        else if(mVertices[i].X() > MaxVec.X())
        {
            MaxVec.X() = mVertices[i].X();
        }

        if(mVertices[i].Y() < MinVec.Y())
        {
            MinVec.Y() = mVertices[i].Y();
        }
        else if(mVertices[i].Y() > MaxVec.Y())
        {
            MaxVec.Y() = mVertices[i].Y();
        }

        if(mVertices[i].Z() < MinVec.Z())
        {
            MinVec.Z() = mVertices[i].Z();
        }
        else if(mVertices[i].Z() > MaxVec.Z())
        {
            MaxVec.Z() = mVertices[i].Z();
        }
    }

    //Proceed to centering
    for(unsigned int i = 0; i < mVertices.size(); ++i)
    {
        mVertices[i]  -= (MinVec + MaxVec)/2.0f;
    }
}

unsigned int ModelLoader::FillPrimitive()
{
    //Create our mesh
    unsigned int ListId = glGenLists(1);
    
    glNewList(ListId,GL_COMPILE);
    
    glBegin(GL_TRIANGLES);
    for(unsigned int i = 0; i < mCombinedVertices.size(); ++i)
    {
        //VX1
        glNormal3fv(mNormals[mCombinedNormals[i].Ref1]);
        glTexCoord2fv(mUVs[mCombinedUVs[i].Ref1]);
        glVertex3fv(mVertices[mCombinedVertices[i].Ref1]);
        
        //VX2
        glNormal3fv(mNormals[mCombinedNormals[i].Ref2]);
        glTexCoord2fv(mUVs[mCombinedUVs[i].Ref2]);
        glVertex3fv(mVertices[mCombinedVertices[i].Ref2]);
        
        //VX3
        glNormal3fv(mNormals[mCombinedNormals[i].Ref3]);
        glTexCoord2fv(mUVs[mCombinedUVs[i].Ref3]);
        glVertex3fv(mVertices[mCombinedVertices[i].Ref3]);
        }
    glEnd();
    glEndList();
    
    return ListId;
}

void ModelLoader::ResetLoader()
{
    mVertices.clear();
    mVertices.push_back(Vector3(0,0,0));    //Fill the first vector since
    //obj file indexes start at 1.
    //(requires less cpu than subtracting
    // 1 every time we access a vertex.)
    
    mNormals.clear();
    mNormals.push_back(Vector3(0,0,0));     //Fill the first vector since
    //obj file normals start at 1.
    //(requires less cpu than subtracting
    // 1 every time we access a normal.)
    
    mUVs.clear();
    mUVs.push_back(Point2(0,0));
    
    mVertReferences.clear();
    mNormalsReference.clear();
    
    mCombinedNormals.clear();
    mCombinedVertices.clear();
    
    
    mCombinedUVs.clear();
    mUVsReference.clear();
}

ModelLoader::~ModelLoader()
{}
