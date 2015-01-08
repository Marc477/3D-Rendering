/*
 *  ModelLoader.h
 *  TP2
 *
 *  Created by Olivier Vaillancourt on 10-10-26.
 *  Copyright 2010 Université de Sherbrooke. All rights reserved.
 *
 */

#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#include <cstdio>
#include <string>
#include <vector>

#include "../Model.h"
#include "Point2.h"
#include "Vector3.h"


using namespace std;

class ModelLoader
{
public:
    
    ModelLoader();
    virtual ~ModelLoader();
    
    void ResetLoader();
    
    Model Load(const string &aFileName, bool aNormalizeMesh = false);
    
protected:
    
    struct TmpTriplet
    {
        TmpTriplet(): VxRef(0), TexRef(0),NormRef(0){};
        
        int VxRef;
        int TexRef;
        int NormRef;
    };

    void DispatchRead(FILE* apFile);
    
    void ReadVertexData(FILE* apFile);
    void ReadNormalData(FILE* apFile);
    void ReadTextureData(FILE *apFile);
    void ReadSurfaceData(FILE* apFile);
    
    void GenerateTriangles(vector<TmpTriplet> &aDataList);
    void GenerateNormals();

    void NormalizeMesh();
    
    unsigned int FillPrimitive();
    
    vector<Vector3>                                     mVertices;
    vector<vector<int> >                                mVertReferences;
    
    vector<Vector3>                                     mNormals;
    vector<vector<int> >                                mNormalsReference;
    
    vector<Point2>                                      mUVs;
    vector<vector<int> >                                mUVsReference;
    
    vector<Model::Triangle>                                 mCombinedVertices;
    vector<Model::Triangle>                                 mCombinedNormals;
    vector<Model::Triangle>                                 mCombinedUVs;
};

#endif //MODELLOADER_H_
