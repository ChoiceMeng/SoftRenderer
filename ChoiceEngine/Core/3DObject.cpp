#include "3DObject.h"
using namespace CE::Core;
using namespace MAXOBJ;

C3DObject::C3DObject(MAXOBJ::t3DModel* model)
{
	MAXOBJ::t3DObject* obj = model->pObject[0];
	m_nVetexCount = obj->numOfVerts;
	m_nFaceCount = obj->numOfFaces;

	m_pVertexList = new class CVertex[m_nVetexCount];
	for (int i = 0; i < m_nVetexCount; i++)
	{
		m_pVertexList[i].mVertex.x = obj->pVerts[i].x;
		m_pVertexList[i].mVertex.y = obj->pVerts[i].y;
		m_pVertexList[i].mVertex.z = obj->pVerts[i].z;
		m_pVertexList[i].mVertex.w = 1.0f;

		m_pVertexList[i].mNormal.x = obj->pNormals[i].x;
		m_pVertexList[i].mNormal.y = obj->pNormals[i].y;
		m_pVertexList[i].mNormal.z = obj->pNormals[i].z;
		m_pVertexList[i].mNormal.w = 0.0f;

		m_pVertexList[i].mUv.x = obj->pTexVerts[i].x;
		m_pVertexList[i].mUv.y = obj->pTexVerts[i].y;
	}

	m_pFace = new struct CFace[m_nFaceCount];
	for(int i = 0; i < m_nFaceCount; i++)
	{
		m_pFace[i].mVertIndex[0] = obj->pFaces[i].vertIndex[0];
		m_pFace[i].mVertIndex[1] = obj->pFaces[i].vertIndex[1];
		m_pFace[i].mVertIndex[2] = obj->pFaces[i].vertIndex[2];
	}
}

C3DObject::~C3DObject(void)
{
	delete[] m_pVertexList;
	delete[] m_pFace;
}
