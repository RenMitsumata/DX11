#pragma once

class aiScene;

class CModelAnimation
{
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	unsigned int indexNum;
	const aiScene* pScene;
	MESH* meshes;
	//void DrawChild(aiNode* pNode);
public:
	void Draw();
	void Draw(XMMATRIX matrix, float canonAngle, float canonUpAngle);
	void DrawChild(aiNode * pNode, float canonAngle, float canonUpAngle);
	//void Draw(XMMATRIX matrix);
	void Load(const char* filename);
	void UnLoad();
	CModelAnimation();
	virtual ~CModelAnimation();
};

