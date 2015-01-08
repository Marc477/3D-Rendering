
/*
  Marc-Antoine Desbiens
  November 2012
*/

#include "Model.h"

#include "Utilities/GLInclude.h"
#include <list>

Model::Model()
{}

//////////////////////////////////////////////////////////////////////////

Model::Model(const vector<Vector3>& aVertices, 
             const vector<Triangle>& aTriangles, 
             unsigned int aGLId)
: mVertices(aVertices)
, mTriangles(aTriangles)
, mGLId(aGLId)
{}

//////////////////////////////////////////////////////////////////////////

void Model::DrawMesh()
{
    //Material setup
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mMat.Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mMat.Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,mMat.Specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,mMat.Shininess);

    //Mesh, to the batmobile!
    glCallList(mGLId);
}

//////////////////////////////////////////////////////////////////////////

void Model::Init()
{
	//Save triangles normals
	Vector3 v[3];
	for(int i=0; i< (int)Triangles().size(); i++)
	{
		v[0] = mVertices[mTriangles[i].PtId1];
		v[1] = mVertices[mTriangles[i].PtId2];
		v[2] = mVertices[mTriangles[i].PtId3];
		mTriangles[i].normal = (v[1]-v[0]).CrossProduct(v[2]-v[0]);
	}
	 
	///////////////Build Connectivity list ////////////////////
	std::list<Triangle*> listTri;
	std::list<Triangle*>::iterator iter;
	Segment segm;
	Triangle *tri1, *tri2;
	int poTri1[4], poTri2[4], k, sideIndex, sideIndex2;
	vector<int> nbConnect;
	vector<vector<int>> triConnex;
	bool flagSide1[3], flagSide2[3]; //Tell on which side is the connexion

	//Put all triangles in a list and init values
	nbConnect.resize(Triangles().size());
	triConnex.resize(Triangles().size());
	for(int i=0; i< (int)Triangles().size(); i++)
	{
		mTriangles[i].index = i;
		listTri.push_back(&mTriangles[i]);
		nbConnect[i] = 0;
		triConnex[i].resize(3);
		triConnex[i][0] = triConnex[i][1] = triConnex[i][2] = -1;
	}

	//Loop on all triangles
	for(int i=0; i< (int)Triangles().size(); i++)
	{
		tri1 = &mTriangles[i];
		//Place in array to use loop
		poTri1[0] = tri1->PtId1;
		poTri1[1] = tri1->PtId2;
		poTri1[2] = tri1->PtId3;
		poTri1[3] = tri1->PtId1; //usefull for [sideIndex+1]

		//Find connected triangles
		iter = listTri.begin();
		while(iter!=listTri.end())
		{
			tri2 = *iter;

			 //Not same triangle && not already visited
			if( tri1 != tri2 && triConnex[tri1->index][0] != tri2->index 
				&& triConnex[tri1->index][1] != tri2->index  && triConnex[tri1->index][2] != tri2->index  )
			{
				//Place in array to use loop
				poTri2[0] = tri2->PtId1;
				poTri2[1] = tri2->PtId2;
				poTri2[2] = tri2->PtId3;

				//Check if segment matches
				k=0; flagSide1[0]=flagSide1[1]=flagSide1[2]=0; 
				flagSide2[0]=flagSide2[1]=flagSide2[2]=0; 
				for(int s1=0; s1<3; s1++){
					for(int s2=0; s2<3; s2++){
						if(poTri1[s1] == poTri2[s2]){
							k++;
							flagSide1[s1] = 1;
							flagSide2[s2] = 1;
						}
					}
				}

				//If 2 points are the same there is a common segment
				if(k>=2)
				{
					//Check which side is connected to triangle
					if(flagSide1[0] == 1 && flagSide1[1] == 1)
						sideIndex = 0;
					if(flagSide1[1] == 1 && flagSide1[2] == 1)
						sideIndex = 1;
					if(flagSide1[2] == 1 && flagSide1[0] == 1)
						sideIndex = 2;
					if(flagSide2[0] == 1 && flagSide2[1] == 1)
						sideIndex2 = 0;
					if(flagSide2[1] == 1 && flagSide2[2] == 1)
						sideIndex2 = 1;
					if(flagSide2[2] == 1 && flagSide2[0] == 1)
						sideIndex2 = 2;

					//Link triangles and increment nbConnections
					triConnex[tri1->index][sideIndex] = tri2->index;
					triConnex[tri2->index][sideIndex2] = tri1->index;
					nbConnect[tri1->index] += 1;
					nbConnect[tri2->index] += 1;

					//Add Segment
					segm.PtId1 = poTri1[sideIndex];
					segm.PtId2 = poTri1[sideIndex+1];
					segm.TriId1 = tri1->index;
					segm.TriId2 = tri2->index;
					mSegments.push_back(segm);
				}
			}
			//Remove triangle from list if nb of connected triangles >=3 ¸or else increment iteration
			if( nbConnect[tri2->index] >= 3 )
				iter = listTri.erase(iter);
			else
				++iter;
		}

		//Push back segments if lower then 3  (ADD border segment that are only on tri1)
		segm.TriId1 = tri1->index;
		segm.TriId2 = -1; //No adjacent triangle
		for(int j=0; j<3; j++)
		{
			if(triConnex[tri1->index][j] == -1) //If still not found any triangle connected to that side
			{
				segm.PtId1 = poTri1[j];
				segm.PtId2 = poTri1[j+1];		
				mSegments.push_back(segm);
			}
		}
	}
	//////////End Build Connectivity list ////////////////////
}