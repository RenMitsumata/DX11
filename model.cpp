#define NOMINMAX
#include "main.h"
#include "renderer.h"

#include "texture.h"


#include <Windows.h>
#include <vector>
#include <assimp\cimport.h>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\matrix4x4.h>
#include "model.h"
#pragma comment (lib,"assimp.lib")
// ����������Ƃ��́A����z�񉻂���
const aiScene* g_pScene = nullptr;
unsigned int* g_Texture = nullptr;
bool key;
static int Cnt = 0;
std::vector<XMMATRIX> _MatList;
std::vector<ID3D11Buffer*> _Buflist;
ID3D11Buffer*  i_VertexBuffer;
ID3D11Buffer*  i_IndexBuffer;

//void MakeBuffer(aiNode* pNode);

struct FACE {
	aiFace* pFace;
};

struct MESH {
	aiMesh*			pMesh;
	MATERIAL		pMaterial;
	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;
	unsigned int	indexCount;
	FACE*			pFaces;
};

MESH* meshes;

void CModel::Init()
{
	m_Position = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	m_Rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Scale = XMFLOAT3( 1.0f, 1.0f, 1.0f );


	Load( "asset/miku_01.obj" );
		
}
/*
void MakeBuffer(aiNode* pNode) {
	for (int i = 0; i < pNode->mNumMeshes; i++) {
		for (int j = 0; j < g_pScene->mMeshes[pNode->mMeshes[i]]->mNumFaces; j++) {
			
		}
	}


}
*/

// int�̊Ǘ��ԍ���return����
void CModel::Init(const char* filename) {
	
	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_pScene = aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality);
	if (g_pScene == nullptr) {
		MessageBox(NULL, "���f���t�@�C�����ǂݍ��߂܂���", "Assimp", MB_OK | MB_ICONHAND);
		exit(1);
	}

	//aiNode* node = g_pScene->mRootNode;

	//MakeBuffer(node);






	meshes = new MESH[g_pScene->mNumMeshes];

	std::vector<VERTEX_3D> _VertexList;

	for (int i = 0; i < g_pScene->mNumMeshes; i++) {
		meshes[i].pMesh = g_pScene->mMeshes[i];
		_VertexList.clear();
		// �}�e���A���̐ݒ�
		const aiMaterial* mat = g_pScene->mMaterials[meshes[i].pMesh->mMaterialIndex];

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



		VERTEX_3D* vertex = new VERTEX_3D[meshes[i].pMesh->mNumVertices];
		for (int n = 0; n < meshes[i].pMesh->mNumVertices; n++) {
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


		// ���_�o�b�t�@�̍쐬
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

		

		// �C���f�b�N�X�o�b�t�@�����
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
		
	}
	
	
	//int material = g_pScene->mNumMaterials;
	//g_Texture = new unsigned int[material];
	/*
	for (int i = 0; i < material; i++) {
		aiString path;


		g_pScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		// �}�e���A���ɉ摜������
		std::string texPath = path.data;
		size_t pos = texPath.find_last_of("\\/");
		std::string headerPath = texPath.substr(0, pos + 1);
		headerPath += path.data;
		texPath.c_str();	// string�̐擪�A�h���X���擾�ł���
		texture[i] = LoadTexture(headerPath.c_str(), 2);
	}
	*/


	//Load(filename);
	
}

void CModel::Init(const char * filename, XMFLOAT3 pos)
{
	m_Position = pos;
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);


	Load(filename);
}


void CModel::Uninit()
{
	delete[] meshes;
	Unload();
}


void CModel::Update()
{
	
}

void CModel::Update(XMFLOAT3 deltaPos)
{
	m_Position.x += deltaPos.x;
	m_Position.y += deltaPos.y;
	m_Position.z += deltaPos.z;
}

void CModel::Draw()
{

	// �}�g���N�X�ݒ�
	
	XMMATRIX world;
	world = XMMatrixIdentity();
	world *= XMMatrixScaling( m_Scale.x, m_Scale.y, m_Scale.z );
	world *= XMMatrixRotationRollPitchYaw( m_Rotation.x, m_Rotation.y, m_Rotation.z );
	world *= XMMatrixTranslation( m_Position.x, m_Position.y, m_Position.z );
	CRenderer::SetWorldMatrix( &world );
	
	// ���_�o�b�t�@�ݒ�
	CRenderer::SetVertexBuffers( m_VertexBuffer );

	// �C���f�b�N�X�o�b�t�@�ݒ�
	CRenderer::SetIndexBuffer( m_IndexBuffer );

	for( unsigned short i = 0; i < m_SubsetNum; i++ )
	{
		// �}�e���A���ݒ�
		CRenderer::SetMaterial( m_SubsetArray[i].Material.Material );

		// �e�N�X�`���ݒ�
		CRenderer::SetTexture( m_SubsetArray[i].Material.Texture );

		// �|���S���`��
		CRenderer::DrawIndexed( m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0 );
	}

}

// ��]�A�X�P�[���������Ȃ�A��Q������transform�s��ɂ��āArootMatrix�Ƃ��ēo�^����B
void CModel::Draw(unsigned int mgtNum, XMFLOAT3 rootPos) {
	// �܂��Arootnode��transform�s����v�b�V������(openGL��)

	XMMATRIX rootMatrix = XMMatrixIdentity();	
	rootMatrix *= XMMatrixRotationY(AI_MATH_PI);
	rootMatrix *= XMMatrixScaling(0.01f, 0.01f, 0.01f);
	rootMatrix *= XMMatrixTranslation(rootPos.x, rootPos.y, rootPos.z);
	
	CRenderer::SetWorldMatrix(&rootMatrix);
	_MatList.push_back(rootMatrix);
	CRenderer::SetDepthEnable(true);
	
	aiNode* pCurrentNode = g_pScene->mRootNode; // ���̃V�[���́H
	
	DrawChild(pCurrentNode,0.0f);
	_MatList.pop_back();
	Cnt = 0;
	CRenderer::SetDepthEnable(false);
}

// ��]�A�X�P�[���������Ȃ�A��Q������transform�s��ɂ��āArootMatrix�Ƃ��ēo�^����B
void CModel::Draw(unsigned int mgtNum, XMFLOAT3 rootPos, XMFLOAT3 yawpitchroll) {
	// �܂��Arootnode��transform�s����v�b�V������(openGL��)

	XMMATRIX rootMatrix = XMMatrixIdentity();
	rootMatrix *= XMMatrixRotationRollPitchYaw(yawpitchroll.x, yawpitchroll.y + AI_MATH_PI, yawpitchroll.z);
	rootMatrix *= XMMatrixScaling(0.01f, 0.01f, 0.01f);
	rootMatrix *= XMMatrixTranslation(rootPos.x, rootPos.y, rootPos.z);

	CRenderer::SetWorldMatrix(&rootMatrix);
	_MatList.push_back(rootMatrix);
	CRenderer::SetDepthEnable(true);

	aiNode* pCurrentNode = g_pScene->mRootNode; // ���̃V�[���́H

	DrawChild(pCurrentNode,0.0f);
	_MatList.pop_back();
	Cnt = 0;
	CRenderer::SetDepthEnable(false);
}

void CModel::Draw(unsigned int mgtNum, XMFLOAT3 rootPos, XMFLOAT3 yawpitchroll, float canonAngle) {
	// �܂��Arootnode��transform�s����v�b�V������(openGL��)

	XMMATRIX rootMatrix = XMMatrixIdentity();
	rootMatrix *= XMMatrixRotationRollPitchYaw(yawpitchroll.x, yawpitchroll.y, yawpitchroll.z);
	rootMatrix *= XMMatrixScaling(0.01f, 0.01f, 0.01f);
	rootMatrix *= XMMatrixTranslation(rootPos.x, rootPos.y, rootPos.z);

	CRenderer::SetWorldMatrix(&rootMatrix);
	_MatList.push_back(rootMatrix);
	CRenderer::SetDepthEnable(true);

	aiNode* pCurrentNode = g_pScene->mRootNode; // ���̃V�[���́H

	DrawChild(pCurrentNode,canonAngle);
	_MatList.pop_back();
	Cnt = 0;
	
}

void CModel::DrawChild(aiNode* pCurrentNode, float canonAngle) {

	if (strcmp(pCurrentNode->mName.data, "Body") == 0) {
		XMMATRIX bodyRotate;
		bodyRotate = XMMatrixIdentity();
		bodyRotate *= XMMatrixRotationY(AI_MATH_PI);
		_MatList.push_back(bodyRotate);

	}

	

	

	// �܂��̓��[�J�����W�����߂āA�X�^�b�N�ɉ�������
	aiMatrix4x4 localMat = pCurrentNode->mTransformation;
	aiTransposeMatrix4(&localMat);
	XMMATRIX pushMatrix = XMMatrixSet(localMat.a1, localMat.a2, localMat.a3, localMat.a4,
		localMat.b1, localMat.b2, localMat.b3, localMat.b4,
		localMat.c1, localMat.c2, localMat.c3, localMat.c4,
		localMat.d1, localMat.d2, localMat.d3, localMat.d4
		);
	_MatList.push_back(pushMatrix);


	if (strcmp(pCurrentNode->mName.data, "Canon") == 0) {

		XMMATRIX bodyRotate;
		bodyRotate = XMMatrixIdentity();
		bodyRotate *= XMMatrixRotationZ(canonAngle);
		_MatList.push_back(bodyRotate);


	}

	

	XMMATRIX worldMatrix = XMMatrixIdentity();
	for (XMMATRIX matrix : _MatList) {
		worldMatrix = matrix * worldMatrix;
	}
	CRenderer::SetWorldMatrix(&worldMatrix);

	// �܂��A������`�悷��
	for (int m = 0; m < pCurrentNode->mNumMeshes; m++) {
		// �}�e���A���ݒ�
		CRenderer::SetMaterial(meshes[pCurrentNode->mMeshes[m]].pMaterial);		

		// ���_�o�b�t�@�ݒ�
		CRenderer::SetVertexBuffers(meshes[pCurrentNode->mMeshes[m]].vertexBuffer);
		// �C���f�b�N�X�o�b�t�@�ݒ�
		CRenderer::SetIndexBuffer(meshes[pCurrentNode->mMeshes[m]].indexBuffer);

		
		// �|���S���`��
		CRenderer::DrawIndexed(meshes[pCurrentNode->mMeshes[m]].indexCount,0,0);

			
		

	}

	// ���̌�A�q��������Ύq����`�悷��
	for (int i = 0; i < pCurrentNode->mNumChildren; i++) {
		DrawChild(pCurrentNode->mChildren[i],canonAngle);
	}

	if (strcmp(pCurrentNode->mName.data, "Canon") == 0) {

		_MatList.pop_back();

	}

	if (strcmp(pCurrentNode->mName.data, "Body") == 0) {
		
		_MatList.pop_back();

	}

	// �I�������A�s����|�b�v����
	_MatList.pop_back();
}















































void CModel::Draw(XMFLOAT3 m_Position)
{
	// �}�g���N�X�ݒ�
	XMMATRIX world;
	world = XMMatrixIdentity();
	//world *= XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	//world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);

	// ���_�o�b�t�@�ݒ�
	CRenderer::SetVertexBuffers(m_VertexBuffer);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	CRenderer::SetIndexBuffer(m_IndexBuffer);

	for (unsigned short i = 0; i < m_SubsetNum; i++)
	{
		// �}�e���A���ݒ�
		CRenderer::SetMaterial(m_SubsetArray[i].Material.Material);

		// �e�N�X�`���ݒ�
		CRenderer::SetTexture(m_SubsetArray[i].Material.Texture);

		// �|���S���`��
		CRenderer::DrawIndexed(m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0);
	}
}




void CModel::Load( const char *FileName )
{

	MODEL model;
	LoadObj( FileName, &model );



	// ���_�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( VERTEX_3D ) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory( &sd, sizeof(sd) );
		sd.pSysMem = model.VertexArray;

		CRenderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );
	}


	// �C���f�b�N�X�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( unsigned short ) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory( &sd, sizeof(sd) );
		sd.pSysMem = model.IndexArray;

		CRenderer::GetDevice()->CreateBuffer( &bd, &sd, &m_IndexBuffer );
	}

	// �T�u�Z�b�g�ݒ�
	{
		m_SubsetArray = new DX11_SUBSET[ model.SubsetNum ];
		m_SubsetNum = model.SubsetNum;

		for( unsigned short i = 0; i < model.SubsetNum; i++ )
		{
			m_SubsetArray[i].StartIndex = model.SubsetArray[i].StartIndex;
			m_SubsetArray[i].IndexNum = model.SubsetArray[i].IndexNum;

			m_SubsetArray[i].Material.Material = model.SubsetArray[i].Material.Material;
			m_SubsetArray[i].Material.Texture = new CTexture();
			if (model.SubsetArray[i].Material.TextureName[0] != '\0') {
				m_SubsetArray[i].Material.Texture->Load(model.SubsetArray[i].Material.TextureName);
			}
			
		}
	}

	delete[] model.VertexArray;
	delete[] model.IndexArray;
	delete[] model.SubsetArray;

}



void CModel::Unload()
{
	if (m_VertexBuffer) {
		m_VertexBuffer->Release();
	}
	if (m_IndexBuffer) {
		m_IndexBuffer->Release();
	}
	


	delete[] m_SubsetArray;

}





//���f���Ǎ�////////////////////////////////////////////
void CModel::LoadObj( const char *FileName, MODEL *Model )
{

	XMFLOAT3	*positionArray;
	XMFLOAT3	*normalArray;
	XMFLOAT2	*texcoordArray;

	unsigned short	positionNum = 0;
	unsigned short	normalNum = 0;
	unsigned short	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned short	indexNum = 0;
	unsigned short	in = 0;
	unsigned short	subsetNum = 0;

	MODEL_MATERIAL	*materialArray = nullptr;
	unsigned short	materialNum = 0;

	char str[256];
	char *s;
	char c;


	FILE *file;
	file = fopen( FileName, "rt" );
	assert(file);



	//�v�f���J�E���g
	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "v" ) == 0 )
		{
			positionNum++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			normalNum++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			texcoordNum++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			subsetNum++;
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			in = 0;

			do
			{
				fscanf( file, "%s", str );
				vertexNum++;
				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c!= '\r' );

			//�l�p�͎O�p�ɕ���
			if( in == 4 )
				in = 6;

			indexNum += in;
		}
	}


	//�������m��
	positionArray = new XMFLOAT3[ positionNum ];
	normalArray = new XMFLOAT3[ normalNum ];
	texcoordArray = new XMFLOAT2[ texcoordNum ];


	Model->VertexArray = new VERTEX_3D[ vertexNum ];
	Model->VertexNum = vertexNum;

	Model->IndexArray = new unsigned short[ indexNum ];
	Model->IndexNum = indexNum;

	Model->SubsetArray = new SUBSET[ subsetNum ];
	Model->SubsetNum = subsetNum;




	//�v�f�Ǎ�
	XMFLOAT3 *position = positionArray;
	XMFLOAT3 *normal = normalArray;
	XMFLOAT2 *texcoord = texcoordArray;

	unsigned short vc = 0;
	unsigned short ic = 0;
	unsigned short sc = 0;


	fseek( file, 0, SEEK_SET );

	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "mtllib" ) == 0 )
		{
			//�}�e���A���t�@�C��
			fscanf( file, "%s", str );

			char path[256];
			strcpy( path, "asset/" );
			strcat( path, str );

			LoadMaterial( path, &materialArray, &materialNum );
		}
		else if( strcmp( str, "o" ) == 0 )
		{
			//�I�u�W�F�N�g��
			fscanf( file, "%s", str );
		}
		else if( strcmp( str, "v" ) == 0 )
		{
			//���_���W
			fscanf( file, "%f", &position->x );
			fscanf( file, "%f", &position->y );
			fscanf( file, "%f", &position->z );
			//position->x *= SCALE_MODEL;
			//position->y *= SCALE_MODEL;
			//position->z *= SCALE_MODEL;
			position++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			//�@��
			fscanf( file, "%f", &normal->x );
			fscanf( file, "%f", &normal->y );
			fscanf( file, "%f", &normal->z );
			normal++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			//�e�N�X�`�����W
			fscanf( file, "%f", &texcoord->x );
			fscanf( file, "%f", &texcoord->y );
			//texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			//�}�e���A��
			fscanf( file, "%s", str );

			if( sc != 0 )
				Model->SubsetArray[ sc - 1 ].IndexNum = ic - Model->SubsetArray[ sc - 1 ].StartIndex;

			Model->SubsetArray[ sc ].StartIndex = ic;


			for( unsigned short i = 0; i < materialNum; i++ )
			{
				if( strcmp( str, materialArray[i].Name ) == 0 )
				{
					Model->SubsetArray[ sc ].Material.Material = materialArray[i].Material;
					strcpy( Model->SubsetArray[ sc ].Material.TextureName, materialArray[i].TextureName );
					strcpy( Model->SubsetArray[ sc ].Material.Name, materialArray[i].Name );

					break;
				}
			}

			sc++;
			
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			//��
			in = 0;

			do
			{
				fscanf( file, "%s", str );

				s = strtok( str, "/" );	
				Model->VertexArray[vc].Position = positionArray[ atoi( s ) - 1 ];
				if( s[ strlen( s ) + 1 ] != '/' )
				{
					//�e�N�X�`�����W�����݂��Ȃ��ꍇ������
					s = strtok( NULL, "/" );
					Model->VertexArray[vc].TexCoord = texcoordArray[ atoi( s ) - 1 ];
				}
				s = strtok( NULL, "/" );	
				Model->VertexArray[vc].Normal = normalArray[ atoi( s ) - 1 ];

				Model->VertexArray[vc].Diffuse = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );

				Model->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c != '\r' );

			//�l�p�͎O�p�ɕ���
			if( in == 4 )
			{
				Model->IndexArray[ic] = vc - 4;
				ic++;
				Model->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}


	if( sc != 0 )
		Model->SubsetArray[ sc - 1 ].IndexNum = ic - Model->SubsetArray[ sc - 1 ].StartIndex;

	fclose(file);



	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}




//�}�e���A���ǂݍ���///////////////////////////////////////////////////////////////////
void CModel::LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned short *MaterialNum )
{
	char str[256];

	FILE *file;
	file = fopen( FileName, "rt" );
	if( file == NULL )
	{
		assert(false);
		return;
	}

	MODEL_MATERIAL *materialArray;
	unsigned short materialNum = 0;

	//�v�f���J�E���g
	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			materialNum++;
		}
	}


	//�������m��
	materialArray = new MODEL_MATERIAL[ materialNum ];


	//�v�f�Ǎ�
	int mc = -1;

	fseek( file, 0, SEEK_SET );

	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			//�}�e���A����
			mc++;
			fscanf( file, "%s", materialArray[ mc ].Name );
			strcpy( materialArray[ mc ].TextureName, "" );
		}
		else if( strcmp( str, "Ka" ) == 0 )
		{
			//�A���r�G���g
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.b );
			materialArray[ mc ].Material.Ambient.a = 1.0f;
		}
		else if( strcmp( str, "Kd" ) == 0 )
		{
			//�f�B�t���[�Y
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.b );
			materialArray[ mc ].Material.Diffuse.a = 1.0f;
		}
		else if( strcmp( str, "Ks" ) == 0 )
		{
			//�X�y�L����
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.b );
			materialArray[ mc ].Material.Specular.a = 1.0f;
		}
		else if( strcmp( str, "Ns" ) == 0 )
		{
			//�X�y�L�������x
			fscanf( file, "%f", &materialArray[ mc ].Material.Shininess );
		}
		else if( strcmp( str, "d" ) == 0 )
		{
			//�A���t�@
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.a );
		}
		else if( strcmp( str, "map_Kd" ) == 0 )
		{
			//�e�N�X�`��
			fscanf( file, "%s", str );

			char path[256];
			strcpy( path, "asset/" );
			strcat( path, str );

			strcat( materialArray[ mc ].TextureName, path );
		}
	}

	fclose(file);
	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}


