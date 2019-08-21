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
const aiScene* g_pScene = nullptr;
unsigned int* g_Texture = nullptr;
bool key;
std::vector<XMMATRIX> _MatList;
std::vector<ID3D11Buffer*> _Buflist;
void CModel::Init()
{
	m_Position = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	m_Rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Scale = XMFLOAT3( 1.0f, 1.0f, 1.0f );


	Load( "asset/miku_01.obj" );
		
}

void CModel::Init(const char* filename) {
	/*
	m_Position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	g_pScene = aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality);
	
	int index = g_pScene->mNumMaterials;	// マテリアル数
	g_Texture = new unsigned int[index];
	key = false;
	aiString path;				// テクスチャファイル名の取得

	for (int i = 0; i < g_pScene->mNumMaterials; i++) {

		if (g_pScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			std::string modelPath = "asset/MODEL/tank2/gatitan.obj";	// 例　モデルデータとテクスチャが同じフォルダにある
			modelPath.c_str();	// 文字列の先頭アドレス
			size_t pos = modelPath.find_last_of("\\/");
			std::string texturePath = modelPath.substr(0, pos + 1);
			texturePath += path.data;
			//g_Texture[i] = Texture::Load(texturePath.c_str());

		}
		else {
			g_Texture[i] = 0;
		}
	}
	
	*/
	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_pScene = aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality);
	if (g_pScene == nullptr) {
		MessageBox(NULL, "モデルファイルが読み込めません", "Assimp", MB_OK | MB_ICONHAND);
		exit(1);
	}
	int material = g_pScene->mNumMaterials;
	g_Texture = new unsigned int[material];
	for (int i = 0; i < material; i++) {
		aiString path;


		g_pScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		// マテリアルに画像がある
		std::string texPath = path.data;
		size_t pos = texPath.find_last_of("\\/");
		std::string headerPath = texPath.substr(0, pos + 1);
		headerPath += path.data;
		texPath.c_str();	// stringの先頭アドレスを取得できる
		texture[i] = LoadTexture(headerPath.c_str(), 2);
	}



	Load(filename);
	
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

	// マトリクス設定
	
	XMMATRIX world;
	world = XMMatrixIdentity();
	world *= XMMatrixScaling( m_Scale.x, m_Scale.y, m_Scale.z );
	world *= XMMatrixRotationRollPitchYaw( m_Rotation.x, m_Rotation.y, m_Rotation.z );
	world *= XMMatrixTranslation( m_Position.x, m_Position.y, m_Position.z );
	CRenderer::SetWorldMatrix( &world );
	
	// 頂点バッファ設定
	CRenderer::SetVertexBuffers( m_VertexBuffer );

	// インデックスバッファ設定
	CRenderer::SetIndexBuffer( m_IndexBuffer );

	for( unsigned short i = 0; i < m_SubsetNum; i++ )
	{
		// マテリアル設定
		CRenderer::SetMaterial( m_SubsetArray[i].Material.Material );

		// テクスチャ設定
		CRenderer::SetTexture( m_SubsetArray[i].Material.Texture );

		// ポリゴン描画
		CRenderer::DrawIndexed( m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0 );
	}

}

void CModel::Draw(XMFLOAT3 m_Position)
{
	// マトリクス設定
	XMMATRIX world;
	world = XMMatrixIdentity();
	//world *= XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	//world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	CRenderer::SetVertexBuffers(m_VertexBuffer);

	// インデックスバッファ設定
	CRenderer::SetIndexBuffer(m_IndexBuffer);

	for (unsigned short i = 0; i < m_SubsetNum; i++)
	{
		// マテリアル設定
		CRenderer::SetMaterial(m_SubsetArray[i].Material.Material);

		// テクスチャ設定
		CRenderer::SetTexture(m_SubsetArray[i].Material.Texture);

		// ポリゴン描画
		CRenderer::DrawIndexed(m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0);
	}
}

void CModel::Draw(XMMATRIX* transform, unsigned int e_FILETYPE) {
	switch (e_FILETYPE) {
	case e_FILEOBJ:
		// マトリクス設定
		
		CRenderer::SetWorldMatrix(transform);

		// 頂点バッファ設定
		CRenderer::SetVertexBuffers(m_VertexBuffer);

		// インデックスバッファ設定
		CRenderer::SetIndexBuffer(m_IndexBuffer);

		for (unsigned short i = 0; i < m_SubsetNum; i++)
		{
			// マテリアル設定
			CRenderer::SetMaterial(m_SubsetArray[i].Material.Material);

			// テクスチャ設定
			CRenderer::SetTexture(m_SubsetArray[i].Material.Texture);

			// ポリゴン描画
			CRenderer::DrawIndexed(m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0);
		}

		break;
	case e_FILEFBX:
		
		aiNode* pNode = g_pScene->mRootNode;
		

		_MatList.push_back(*transform);
		
		for (int i = 0; i < pNode->mNumChildren; i++) {
			DrawChild(pNode->mChildren[i]);
		}
		
		_MatList.pop_back();
		
		for (XMMATRIX matrix : _MatList) {
			// 行列が残っていたらassert
			assert(false);
		}
		
		break;
		
	}
	
}


void CModel::Load( const char *FileName )
{

	MODEL model;
	LoadObj( FileName, &model );



	// 頂点バッファ生成
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


	// インデックスバッファ生成
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

	// サブセット設定
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

void CModel::LoadFBX(const char * filename)
{
	aiNode* pNode = g_pScene->mRootNode;
	XMMATRIX world;
	world = XMMatrixIdentity();

	_MatList.push_back(world);

	for (int i = 0; i < pNode->mNumChildren; i++) {
		DrawChild(pNode->mChildren[i]);
	}

	_MatList.pop_back();
	for (XMMATRIX matrix : _MatList) {
		// 行列が残っていたらassert
		assert(false);
	}
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





//モデル読込////////////////////////////////////////////
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



	//要素数カウント
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

			//四角は三角に分割
			if( in == 4 )
				in = 6;

			indexNum += in;
		}
	}


	//メモリ確保
	positionArray = new XMFLOAT3[ positionNum ];
	normalArray = new XMFLOAT3[ normalNum ];
	texcoordArray = new XMFLOAT2[ texcoordNum ];


	Model->VertexArray = new VERTEX_3D[ vertexNum ];
	Model->VertexNum = vertexNum;

	Model->IndexArray = new unsigned short[ indexNum ];
	Model->IndexNum = indexNum;

	Model->SubsetArray = new SUBSET[ subsetNum ];
	Model->SubsetNum = subsetNum;




	//要素読込
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
			//マテリアルファイル
			fscanf( file, "%s", str );

			char path[256];
			strcpy( path, "asset/" );
			strcat( path, str );

			LoadMaterial( path, &materialArray, &materialNum );
		}
		else if( strcmp( str, "o" ) == 0 )
		{
			//オブジェクト名
			fscanf( file, "%s", str );
		}
		else if( strcmp( str, "v" ) == 0 )
		{
			//頂点座標
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
			//法線
			fscanf( file, "%f", &normal->x );
			fscanf( file, "%f", &normal->y );
			fscanf( file, "%f", &normal->z );
			normal++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			//テクスチャ座標
			fscanf( file, "%f", &texcoord->x );
			fscanf( file, "%f", &texcoord->y );
			//texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			//マテリアル
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
			//面
			in = 0;

			do
			{
				fscanf( file, "%s", str );

				s = strtok( str, "/" );	
				Model->VertexArray[vc].Position = positionArray[ atoi( s ) - 1 ];
				if( s[ strlen( s ) + 1 ] != '/' )
				{
					//テクスチャ座標が存在しない場合もある
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

			//四角は三角に分割
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




//マテリアル読み込み///////////////////////////////////////////////////////////////////
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

	//要素数カウント
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


	//メモリ確保
	materialArray = new MODEL_MATERIAL[ materialNum ];


	//要素読込
	int mc = -1;

	fseek( file, 0, SEEK_SET );

	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			//マテリアル名
			mc++;
			fscanf( file, "%s", materialArray[ mc ].Name );
			strcpy( materialArray[ mc ].TextureName, "" );
		}
		else if( strcmp( str, "Ka" ) == 0 )
		{
			//アンビエント
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.b );
			materialArray[ mc ].Material.Ambient.a = 1.0f;
		}
		else if( strcmp( str, "Kd" ) == 0 )
		{
			//ディフューズ
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.b );
			materialArray[ mc ].Material.Diffuse.a = 1.0f;
		}
		else if( strcmp( str, "Ks" ) == 0 )
		{
			//スペキュラ
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.b );
			materialArray[ mc ].Material.Specular.a = 1.0f;
		}
		else if( strcmp( str, "Ns" ) == 0 )
		{
			//スペキュラ強度
			fscanf( file, "%f", &materialArray[ mc ].Material.Shininess );
		}
		else if( strcmp( str, "d" ) == 0 )
		{
			//アルファ
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.a );
		}
		else if( strcmp( str, "map_Kd" ) == 0 )
		{
			//テクスチャ
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



void CModel::DrawChild(aiNode* pNode) {

	aiMatrix4x4 matrix = pNode->mTransformation;	// 行列（位置）を取ってくる
	XMMATRIX pushMat = XMMatrixSet(matrix.a1, matrix.a2, matrix.a3, matrix.a4,
		matrix.b1, matrix.b2, matrix.b3, matrix.b4,
		matrix.c1, matrix.c2, matrix.c3, matrix.c4,
		matrix.d1, matrix.d2, matrix.d3, matrix.d4);
	_MatList.push_back(pushMat);
	XMMATRIX world = XMMatrixIdentity();

	for (XMMATRIX matrix : _MatList) {		
		world *= matrix;
	}
	CRenderer::SetWorldMatrix(&world);

	if (strcmp(pNode->mName.data, "Body") == 0) {
		/*
		glPushMatrix();
		glRotatef(bodyFront, 0.0f, 1.0f, 0.0f);
		*/
	}



	for (int n = 0; n < pNode->mNumMeshes; n++) {
		const aiMesh* pMesh = g_pScene->mMeshes[pNode->mMeshes[n]];

		const aiMaterial* mat = g_pScene->mMaterials[pMesh->mMaterialIndex];

		MATERIAL material;
		aiColor4D color;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color);
		material.Diffuse.r = color.r;
		material.Diffuse.g = color.g;
		material.Diffuse.b = color.b;
		material.Diffuse.a = color.a;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &color);
		material.Ambient.r = color.r;
		material.Ambient.g = color.g;
		material.Ambient.b = color.b;
		material.Ambient.a = color.a;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &color);
		material.Specular.r = color.r;
		material.Specular.g = color.g;
		material.Specular.b = color.b;
		material.Specular.a = color.a;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &color);
		material.Emission.r = color.r;
		material.Emission.g = color.g;
		material.Emission.b = color.b;
		material.Emission.a = color.a;


		// マテリアル設定
		CRenderer::SetMaterial(material);

		// テクスチャ設定
		//CRenderer::SetTexture(m_SubsetArray[i].Material.Texture);

		for (int fn = 0; fn < pMesh->mNumFaces; fn++) {
			// メッシュの面の回数繰り返す
			const aiFace* pFace = &pMesh->mFaces[fn];
			for (int i = 0; i < pFace->mNumIndices; i++) {
				VERTEX_3D* bufVertex = new VERTEX_3D[pFace->mNumIndices];
				for (int n = 0; n < pFace->mNumIndices; n++) {
					bufVertex[n].Diffuse.x = material.Diffuse.r;
					bufVertex[n].Diffuse.y = material.Diffuse.g;
					bufVertex[n].Diffuse.z = material.Diffuse.b;
					bufVertex[n].Diffuse.w = material.Diffuse.a;
					bufVertex[n].Normal.x = pMesh->mNormals[pFace->mIndices[n]].x;
					bufVertex[n].Normal.y = pMesh->mNormals[pFace->mIndices[n]].y;
					bufVertex[n].Normal.z = pMesh->mNormals[pFace->mIndices[n]].z;
					bufVertex[n].Position.x = pMesh->mVertices[pFace->mIndices[n]].x;
					bufVertex[n].Position.y = pMesh->mVertices[pFace->mIndices[n]].y;
					bufVertex[n].Position.z = pMesh->mVertices[pFace->mIndices[n]].z;
					if (pMesh->HasTextureCoords(0)) {
						bufVertex[n].TexCoord.x = pMesh->mTextureCoords[0][pFace->mIndices[n]].x;
						bufVertex[n].TexCoord.y = pMesh->mTextureCoords[0][pFace->mIndices[n]].y;
					}
					else {
						bufVertex[n].TexCoord = XMFLOAT2(0.0f, 0.0f);
					}

				}
				CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
				CRenderer::GetDeviceContext()->Draw(pFace->mNumIndices, (UINT)bufVertex);
				delete bufVertex;
			}
		}

	}


	if (strcmp(pNode->mName.data, "Canon") == 0) {
		/*
		glPushMatrix();

		glRotatef(g_Angle["Canon"], 0.0f, 0.0f, 1.0f);
		*/
	}


	for (int i = 0; i < pNode->mNumChildren; i++) {
		DrawChild(pNode->mChildren[i]);
	}

	if (strcmp(pNode->mName.data, "Canon") == 0) {
		//glPopMatrix();
	}

	if (strcmp(pNode->mName.data, "Body") == 0) {
		//glPopMatrix();
	}


	_MatList.pop_back();
	//glPopMatrix();
}

