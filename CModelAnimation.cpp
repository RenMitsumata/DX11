#define NOMINMAX
#include "main.h"
#include "renderer.h"
#include <vector>
#include <assimp\cimport.h>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\matrix4x4.h>
#pragma comment (lib,"assimp.lib")
#include "texture.h"
#include "model.h"



#include "CModelAnimation.h"





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
	_MatList.clear();
	
	CRenderer::SetWorldMatrix(&matrix);
	
	// ���_�o�b�t�@�ݒ�
	CRenderer::SetVertexBuffers(vertexBuffer);
	// �C���f�b�N�X�o�b�t�@�ݒ�
	CRenderer::SetIndexBuffer(indexBuffer);
	_MatList.push_back(matrix);
	

	aiNode* pCurrentNode = pScene->mRootNode; // ���̃V�[���́H

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

	

	// �܂��̓��[�J�����W�����߂āA�X�^�b�N�ɉ�������
	aiMatrix4x4 localMat = pNode->mTransformation;
	aiTransposeMatrix4(&localMat);
	XMMATRIX pushMatrix = XMMatrixSet(localMat.a1, localMat.a2, localMat.a3, localMat.a4,
		localMat.b1, localMat.b2, localMat.b3, localMat.b4,
		localMat.c1, localMat.c2, localMat.c3, localMat.c4,
		localMat.d1, localMat.d2, localMat.d3, localMat.d4
	);
	_MatList.push_back(pushMatrix);

	
	

	XMMATRIX worldMatrix = XMMatrixIdentity();
	for (XMMATRIX matrix : _MatList) {
		worldMatrix = matrix * worldMatrix;
	}
	CRenderer::SetWorldMatrix(&worldMatrix);

	
	// �܂��A������`�悷��
	for (int m = 0; m < pNode->mNumMeshes; m++) {
		// �}�e���A���ݒ�
		CRenderer::SetMaterial(meshes[pNode->mMeshes[m]].pMaterial);
		

		// �|���S���`��
		CRenderer::DrawIndexed(meshes[pNode->mMeshes[m]].indexCount, meshes[pNode->mMeshes[m]].startIndex, 0);
			   		 
	}
	// ���̌�A�q��������Ύq����`�悷��
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
	
	
	// �I�������A�s����|�b�v����
	_MatList.pop_back();
}

void CModelAnimation::Load(const char * filename)
{
	pScene = aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality);
	if (pScene == nullptr) {
		char filestring[256];
		lstrcpy(filestring, filename);
		char exp[32] = { "���ǂݍ��߂܂���" };
		lstrcat(filestring, exp);
		MessageBox(NULL, filestring, "Assimp", MB_OK | MB_ICONHAND);
	}
	
	// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�̒P�ꉻ�̂��߂̃J�E���^
	unsigned int indexCnt = 0;

	meshes = new MESH[pScene->mNumMeshes];

	std::vector<VERTEX_3D> _VertexList;
	std::vector<unsigned short> _IndexList;

	for (int i = 0; i < pScene->mNumMeshes; i++) {
		meshes[i].pMesh = pScene->mMeshes[i];
		
		// �}�e���A���̐ݒ�
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

		
		// �{���Ȃ炱����material��texture�֌W��`��
		//meshes[i].pTexture = new CTexture();
		//pScene->mMeshes[pNode->mMeshes[0]]->mVertices;
		VERTEX_3D vertex;
		for (int n = 0; n < meshes[i].pMesh->mNumVertices; n++) {
			vertex.Position.x = meshes[i].pMesh->mVertices[n].x;
			vertex.Position.y = meshes[i].pMesh->mVertices[n].y;
			vertex.Position.z = meshes[i].pMesh->mVertices[n].z;
			vertex.Normal.x = meshes[i].pMesh->mNormals[n].x;
			vertex.Normal.y = meshes[i].pMesh->mNormals[n].y;
			vertex.Normal.z = meshes[i].pMesh->mNormals[n].z;
			vertex.Diffuse.x = 3.0f;
			vertex.Diffuse.y = 3.0f;
			vertex.Diffuse.z = 3.0f;
			vertex.Diffuse.w = 1.0f;
			vertex.TexCoord.x = 0.0f;
			vertex.TexCoord.y = 0.0f;
			_VertexList.push_back(vertex);
		}
		
		// �C���f�b�N�X�o�b�t�@�����
		meshes[i].pFaces = new FACE[meshes[i].pMesh->mNumFaces];
		unsigned int indexNum = 0;
		for (int in = 0; in < meshes[i].pMesh->mNumFaces; in++) {
			meshes[i].pFaces[in].pFace = &meshes[i].pMesh->mFaces[in];
			indexNum += meshes[i].pFaces[in].pFace->mNumIndices;
		}
		meshes[i].indexCount = indexNum;

		// �C���f�b�N�X�̊J�n�ʒu�����߂�
		meshes[i].startIndex = indexCnt;


		unsigned short indexIns;
		int a = 0;
		for (int f = 0; f < meshes[i].pMesh->mNumFaces; f++) {
			for (int idx = 0; idx < meshes[i].pFaces[f].pFace->mNumIndices; idx++) {
				indexIns = meshes[i].pFaces[f].pFace->mIndices[idx];
				_IndexList.push_back(indexIns);
				indexCnt++;
				a++;
			}
		}


		
	}

	// ���_�o�b�t�@�̍쐬
	VERTEX_3D* vertexBuf = new VERTEX_3D[_VertexList.size()];
	unsigned int num = 0;
	for (VERTEX_3D oneVertex : _VertexList) {
		vertexBuf[num] = oneVertex;
		num++;
	}
	 
	// �C���f�b�N�X�o�b�t�@�̍쐻
	unsigned short* indexBuf = new unsigned short[_IndexList.size()];
	num = 0;
	for (unsigned short oneIndex : _IndexList) {
		indexBuf[num] = oneIndex;
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
	CRenderer::GetDevice()->CreateBuffer(&vertexBufferDesc, &vbData, &vertexBuffer);

	delete vertexBuf;




	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * _IndexList.size();
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA ibData;
	ibData.pSysMem = indexBuf;
	ibData.SysMemPitch = 0;
	ibData.SysMemSlicePitch = 0;
	CRenderer::GetDevice()->CreateBuffer(&indexBufferDesc, &ibData, &indexBuffer);

	delete indexBuf;
}

void CModelAnimation::UnLoad()
{
	delete[] meshes;
	aiReleaseImport(pScene);
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
