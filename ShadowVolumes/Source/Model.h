
/*
  Marc-Antoine Desbiens
  November 2012
*/

#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include "Vector3.h"

using namespace std;

//Generic 3D Model class. Modify this class as you see fit
//to get your 3D volume shadows going.
//
class Model
{
public :

    //Small structure defining a triangle. The 3 ints
    //point to 3 locations in the Vertices() vector.
    //These 3 locations are 3 3D vectors corresponding
    //to the 3 vertices of the triangle.
    struct Triangle
    {
        int PtId1;
        int PtId2;
        int PtId3;
		Vector3 normal;
		int index;
    };

	struct Segment
	{
		int TriId1;
		int TriId2;
		int PtId1;
		int PtId2;
	};

    struct Material
    {
        float Ambient[4];
        float Diffuse[4];
        float Specular[4];
        float Shininess;
    };

    //Constructors, may be called multiple times. 
    //
    //It is suggested to refrain from
    //performing heavy initialization tasks in the
    //constructors, use the Init method instead, as
    //it is guaranteed to be called once.
    Model();
    Model(  const vector<Vector3>&  aVertices,
            const vector<Triangle>& aTriangles,
            unsigned int            aGLId);

    //Called once when the mesh is loaded the first
    //time. Generate useful info (such as mesh segment
    //and neighboring triangles data) here.
    void Init();

    //Sends the mesh to the OpenGL pipeline. The
    //Material is configured right before sending
    //the mesh.
    //
    //DO NOT MODIFY THIS METHOD
    void DrawMesh();

    //Sets the material, automatically called
    //when the mesh is loaded.
    void SetMaterial(const Material& aMat);

    //Returns the list of vertices that composes the mesh.
    const vector<Vector3>& Vertices()     const;

    //Returns the list of triangles that composes the mesh.
    //Every triangle is composed of 3 integers that point
    //to 3 Vertices from the vertices list (obtainable through
    //method Vertices()).
    const vector<Triangle>& Triangles()   const;

	const vector<Segment>& Segments() const;

protected:

    
    Material            mMat;
    unsigned int        mGLId;

public:   vector<Vector3>     mVertices;
    vector<Triangle>    mTriangles;
	vector<Segment>     mSegments;
};

//////////////////////////////////////////////////////////////////////////

inline void Model::SetMaterial(const Material& aMat)
{
    mMat = aMat;
}

//////////////////////////////////////////////////////////////////////////

inline const vector<Vector3>& Model::Vertices() const
{
    return mVertices;
}

//////////////////////////////////////////////////////////////////////////

inline const vector<Model::Triangle>& Model::Triangles() const
{
    return mTriangles;
}

inline const vector<Model::Segment>& Model::Segments() const
{
    return mSegments;
}

#endif //MODEL_H_
