#pragma once



// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	MODEL_MATERIAL	Material;
};

// モデル構造体
struct MODEL
{
	VERTEX_3D		*VertexArray;
	unsigned short	VertexNum;
	unsigned short	*IndexArray;
	unsigned short	IndexNum;
	SUBSET			*SubsetArray;
	unsigned short	SubsetNum;
};

//　前方宣言
class aiNode;

class CModel
{
private:

	XMFLOAT3					m_Position;
	XMFLOAT3					m_Rotation;
	XMFLOAT3					m_Scale;

	ID3D11Buffer*	m_VertexBuffer = NULL;
	ID3D11Buffer*	m_IndexBuffer = NULL;

	DX11_SUBSET*	m_SubsetArray = NULL;
	unsigned short	m_SubsetNum;

	void LoadObj( const char *FileName, MODEL *Model );
	void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned short *MaterialNum );
	void DrawChild(aiNode* pNode);
public:
	void Init();
	void Init(const char * filename);
	void Init(const char * filename,XMFLOAT3 pos);
	void Uninit();
	void Update();
	void Update(XMFLOAT3 deltaPos);
	void Draw();
	void Draw(XMFLOAT3 m_Position);
	void Draw(XMMATRIX* transform, unsigned int e_FILETYPE);
	void Load( const char *FileName );
	void LoadFBX(const char* filename);
	void Unload();
	enum e_FILETYPE {
		e_FILEOBJ = 0,
		e_FILEFBX
	};
};