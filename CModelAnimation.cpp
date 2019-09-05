#define NOMINMAX
#include "main.h"
#include "renderer.h"
#include <vector>
#include <assimp\cimport.h>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\matrix4x4.h>
#pragma comment (lib,"assimp.lib")
#include "model.h"



#include "CModelAnimation.h"



static std::vector<XMMATRIX> _MatList;


void CModelAnimation::Draw()
{
	aiNode* pRootNode = pScene->mRootNode;
	XMMATRIX matrix = XMMatrixIdentity();

	matrix *= XMMatrixScaling(0.01f, 0.01f, 0.01f);
	
	_MatList.push_back(matrix);
	DrawChild(pRootNode,0,0);
	_MatList.pop_back();
}

void CModelAnimation::Draw(XMMATRIX matrix, float canonAngle, float canonUpAngle)
{
	
	
	
	_MatList.push_back(matrix);
	CRenderer::SetDepthEnable(true);

	aiNode* pCurrentNode = pScene->mRootNode; // 今のシーンは？

	DrawChild(pCurrentNode,canonAngle,canonUpAngle);
	_MatList.pop_back();

	
}

void CModelAnimation::DrawChild(aiNode * pNode, float canonAngle, float canonUpAngle)
{
	


	if (strcmp(pNode->mName.data, "Body") == 0) {
		XMMATRIX bodyRotate;
		bodyRotate = XMMatrixIdentity();
		bodyRotate *= XMMatrixRotationY(AI_MATH_PI);
		_MatList.push_back(bodyRotate);

	}

	if (strcmp(pNode->mName.data, "RightShoulder") == 0) {
		XMMATRIX bodyRotate;
		bodyRotate = XMMatrixIdentity();
		bodyRotate *= XMMatrixRotationZ(canonAngle);
		_MatList.push_back(bodyRotate);

	}

	if (strcmp(pNode->mName.data, "LeftShoulder") == 0) {
		XMMATRIX bodyRotate;
		bodyRotate = XMMatrixIdentity();
		bodyRotate *= XMMatrixRotationZ(canonUpAngle);
		_MatList.push_back(bodyRotate);

	}
	

	// まずはローカル座標を求めて、スタックに押し込む
	aiMatrix4x4 localMat = pNode->mTransformation;
	aiTransposeMatrix4(&localMat);
	XMMATRIX pushMatrix = XMMatrixSet(localMat.a1, localMat.a2, localMat.a3, localMat.a4,
		localMat.b1, localMat.b2, localMat.b3, localMat.b4,
		localMat.c1, localMat.c2, localMat.c3, localMat.c4,
		localMat.d1, localMat.d2, localMat.d3, localMat.d4
	);
	_MatList.push_back(pushMatrix);

	if (strcmp(pNode->mName.data, "Canon") == 0) {

		XMMATRIX bodyRotate;
		bodyRotate = XMMatrixIdentity();
		bodyRotate *= XMMatrixRotationZ(canonAngle);
		_MatList.push_back(bodyRotate);


	}

	if (strcmp(pNode->mName.data, "CanonBody") == 0) {

		XMMATRIX bodyRotate;
		bodyRotate = XMMatrixIdentity();
		bodyRotate *= XMMatrixRotationX(canonUpAngle);
		_MatList.push_back(bodyRotate);


	}





	

	XMMATRIX worldMatrix = XMMatrixIdentity();
	for (XMMATRIX matrix : _MatList) {
		worldMatrix = matrix * worldMatrix;
	}
	CRenderer::SetWorldMatrix(&worldMatrix);

	
	// まず、自分を描画する
	for (int m = 0; m < pNode->mNumMeshes; m++) {
		// マテリアル設定
		CRenderer::SetMaterial(meshes[pNode->mMeshes[m]].pMaterial);

		// 頂点バッファ設定
		CRenderer::SetVertexBuffers(meshes[pNode->mMeshes[m]].vertexBuffer);
		// インデックスバッファ設定
		CRenderer::SetIndexBuffer(meshes[pNode->mMeshes[m]].indexBuffer);
		
		

		// ポリゴン描画
		CRenderer::DrawIndexed(meshes[pNode->mMeshes[m]].indexCount, 0, 0);




	}

	// その後、子供がいれば子供を描画する
	for (int i = 0; i < pNode->mNumChildren; i++) {
		DrawChild(pNode->mChildren[i],canonAngle, canonUpAngle);
	}


	if (strcmp(pNode->mName.data, "RightShoulder") == 0) {
		_MatList.pop_back();

	}

	if (strcmp(pNode->mName.data, "LeftShoulder") == 0) {
		_MatList.pop_back();

	}






	if (strcmp(pNode->mName.data, "CanonBody") == 0) {

		_MatList.pop_back();

	}

	if (strcmp(pNode->mName.data, "Canon") == 0) {

		_MatList.pop_back();

	}

	if (strcmp(pNode->mName.data, "Body") == 0) {

		_MatList.pop_back();

	}

	// 終わったら、行列をポップする
	_MatList.pop_back();
}

void CModelAnimation::Load(const char * filename)
{
	pScene = aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality);
	if (pScene == nullptr) {
		char filestring[256];
		lstrcpy(filestring, filename);
		char exp[32] = { "が読み込めません" };
		lstrcat(filestring, exp);
		MessageBox(NULL, filestring, "Assimp", MB_OK | MB_ICONHAND);
	}

	meshes = new MESH[pScene->mNumMeshes];

	std::vector<VERTEX_3D> _VertexList;

	for (int i = 0; i < pScene->mNumMeshes; i++) {
		meshes[i].pMesh = pScene->mMeshes[i];
		_VertexList.clear();
		// マテリアルの設定
		const aiMaterial* mat = pScene->mMaterials[meshes[i].pMesh->mMaterialIndex];
		
		aiColor4D bufColor;

		aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &bufColor);
		meshes[i].pMaterial.Diffuse.r = bufColor.r;
		meshes[i].pMaterial.Diffuse.g = bufColor.g;
		meshes[i].pMaterial.Diffuse.b = bufColor.b;
		meshes[i].pMaterial.Diffuse.a = bufColor.a;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &bufColor);
		meshes[i].pMaterial.Ambient.r = bufColor.r;
		meshes[i].pMaterial.Ambient.g = bufColor.g;
		meshes[i].pMaterial.Ambient.b = bufColor.b;
		meshes[i].pMaterial.Ambient.a = bufColor.a;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &bufColor);
		meshes[i].pMaterial.Specular.r = bufColor.r;
		meshes[i].pMaterial.Specular.g = bufColor.g;
		meshes[i].pMaterial.Specular.b = bufColor.b;
		meshes[i].pMaterial.Specular.a = bufColor.a;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &bufColor);
		meshes[i].pMaterial.Emission.r = bufColor.r;
		meshes[i].pMaterial.Emission.g = bufColor.g;
		meshes[i].pMaterial.Emission.b = bufColor.b;
		meshes[i].pMaterial.Emission.a = bufColor.a;



		// 本来ならここにmaterialのtexture関係を描く

		//pScene->mMeshes[pNode->mMeshes[0]]->mVertices;

		VERTEX_3D* vertex = new VERTEX_3D[meshes[i].pMesh->mNumVertices];
		for (int n = 0; n < meshes[i].pMesh->mNumVertices; n++) {
			vertex[n].Position.x = pScene->mMeshes[i]->mVertices[n].x;


			vertex[n].Position.x = meshes[i].pMesh->mVertices[n].x;
			vertex[n].Position.y = meshes[i].pMesh->mVertices[n].y;
			vertex[n].Position.z = meshes[i].pMesh->mVertices[n].z;
			vertex[n].Normal.x = meshes[i].pMesh->mNormals[n].x;
			vertex[n].Normal.y = meshes[i].pMesh->mNormals[n].y;
			vertex[n].Normal.z = meshes[i].pMesh->mNormals[n].z;
			vertex[n].Diffuse.x = 3.0f;
			vertex[n].Diffuse.y = 3.0f;
			vertex[n].Diffuse.z = 3.0f;
			vertex[n].Diffuse.w = 1.0f;
			vertex[n].TexCoord.x = 0.0f;
			vertex[n].TexCoord.y = 0.0f;
			_VertexList.push_back(vertex[n]);
		}


		// 頂点バッファの作成
		VERTEX_3D* vertexBuf = new VERTEX_3D[_VertexList.size()];
		unsigned int num = 0;
		for (VERTEX_3D oneVertex : _VertexList) {
			vertexBuf[num] = oneVertex;
			num++;
		}

		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.ByteWidth = sizeof(VERTEX_3D) * _VertexList.size();
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vbData;
		vbData.pSysMem = vertexBuf;
		vbData.SysMemPitch = 0;
		vbData.SysMemSlicePitch = 0;
		CRenderer::GetDevice()->CreateBuffer(&vertexBufferDesc, &vbData, &meshes[i].vertexBuffer);

		delete vertexBuf;



		// インデックスバッファを作る
		meshes[i].pFaces = new FACE[meshes[i].pMesh->mNumFaces];
		unsigned short indexNum = 0;
		for (int in = 0; in < meshes[i].pMesh->mNumFaces; in++) {
			meshes[i].pFaces[in].pFace = &meshes[i].pMesh->mFaces[in];
			indexNum += meshes[i].pFaces[in].pFace->mNumIndices;
		}
		meshes[i].indexCount = indexNum;

		unsigned short* indexIns = new unsigned short[indexNum];
		int a = 0;
		for (int f = 0; f < meshes[i].pMesh->mNumFaces; f++) {
			for (int idx = 0; idx < meshes[i].pFaces[f].pFace->mNumIndices; idx++) {
				indexIns[a] = meshes[i].pFaces[f].pFace->mIndices[idx];
				a++;
			}
		}

		D3D11_BUFFER_DESC indexBuffer;
		indexBuffer.ByteWidth = sizeof(unsigned short) * indexNum;
		indexBuffer.Usage = D3D11_USAGE_DEFAULT;
		indexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBuffer.CPUAccessFlags = 0;
		indexBuffer.MiscFlags = 0;
		indexBuffer.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA ibData;
		ibData.pSysMem = indexIns;
		ibData.SysMemPitch = 0;
		ibData.SysMemSlicePitch = 0;
		CRenderer::GetDevice()->CreateBuffer(&indexBuffer, &ibData, &meshes[i].indexBuffer);



		delete indexIns;
		delete vertex;
		//aiReleaseImport(pScene);
	}
}

void CModelAnimation::UnLoad()
{
	delete[] meshes;
	if (vertexBuffer) {
		vertexBuffer->Release();
	}
	if (indexBuffer) {
		indexBuffer->Release();
	}
}

CModelAnimation::CModelAnimation()
{
}


CModelAnimation::~CModelAnimation()
{
}
