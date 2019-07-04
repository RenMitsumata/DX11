#include "main.h"

#include "renderer.h"
#include "texture.h"
#include "GameObject.h"
#include <time.h>

#include "Field.h"

static XMMATRIX mtx;
static XMMATRIX offsetPos;
static XMFLOAT3** vertexPos;
static ID3D11Buffer* index;
static int g_Num;
#define SQUARE_NUMBER (30)
#define SQUARE_SIZE (3)

CField::CField()
{
}


CField::~CField()
{
}

void CField::Init()
{
	//　頂点バッファ
	const int n = SQUARE_NUMBER;
	static const int vertexNum = 2 * (n + 1) * n + 2 * (n - 1);
	g_Num = vertexNum;
	m_Position = XMFLOAT3{ 0.0f,0.0f,0.0f };
	VERTEX_3D vertex[vertexNum];
	int count = 0;
	
	srand(time(NULL));


	for (int i = 0; i < n; i++) {
		for (int j = 0; j < (2 * (n + 1)); j++,count++) {
			vertex[count].Position.x = m_Position.x - (n * 0.5f - (j % 2) - i) * SQUARE_SIZE;
			float r = (rand() % 10) * 0.1f;
			if ((i != 0) && (count / 2)) {
				vertex[count].Position.y = vertex[count - (2 * (n + 1)) - 1].Position.y;
			}
			else {
				vertex[count].Position.y = m_Position.y + r;
			}
			vertex[count].Position.z = m_Position.z + (n * 0.5f - (j / 2)) * SQUARE_SIZE;
			
			
			vertex[count].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);


			vertex[count].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[count].TexCoord = { (float)((j % 2) + i),float(j / 2)};
		}		
		if (i != (n - 1)) {
			vertex[count] = vertex[count - 1];
			count++;
			vertex[count] = vertex[count - 2 * (n + 1)];
			count++;
		}
	}


	// === 頂点の法線ベクトルを求める ===
	// まずはすべての面の法線ベクトルを求める
	const int faceNum = SQUARE_NUMBER * SQUARE_NUMBER * 2;
	XMVECTOR faceNormal[faceNum];
	XMVECTOR bufVector[2];
	int c = 0;
	for (int j = 0; j < SQUARE_NUMBER; j++) {
		for (int i = 0; i < 2 * SQUARE_NUMBER; i++) {
			bufVector[0] = (DirectX::XMLoadFloat3(&vertex[i + 1 + (j + 2) * 2].Position) - DirectX::XMLoadFloat3(&vertex[i + (j + 2) * 2].Position));
			bufVector[1] = (DirectX::XMLoadFloat3(&vertex[i + 2 + (j + 2) * 2].Position) - DirectX::XMLoadFloat3(&vertex[i + 1 + (j + 2) * 2].Position));
			bufVector[0] = XMVector3Normalize(bufVector[0]);
			bufVector[1] = XMVector3Normalize(bufVector[1]);
			faceNormal[c] = XMVector3Cross(bufVector[0], bufVector[1]);
			c++;
		}
	}

	// 次に、その点が接する全ての面の法線の平均を代入する
	XMVECTOR faceQuadNormal[SQUARE_NUMBER * SQUARE_NUMBER];
	for (int i = 0; i < SQUARE_NUMBER * SQUARE_NUMBER; i++) {
		XMVECTOR buf = faceNormal[i * 2] + faceNormal[i * 2 + 1];
		buf = XMVector3Normalize(buf);
		faceQuadNormal[i] = buf;
	}
	count = 0;
	// 1周目の偶数は左がない
	// 各周１、２個目は上がない->頂点の0、1番目
	// 各周最後１、２個目は下がない->頂点の2*NUM、2*NUM+1番目
	// 最後の周の奇数番目は右がない
	XMFLOAT3 initVec = XMFLOAT3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < (2 * (n + 1)); j++, count++) {
			XMVECTOR calcVec = DirectX::XMLoadFloat3(&initVec);
			if ((i != 0) && (j > 1)) {
				// 左上のポリゴンを足す
				if (count / 2) {
					calcVec += faceQuadNormal[i * SQUARE_NUMBER + (j / 2) - 1];
				}
				else {
					calcVec += faceQuadNormal[(i - 1) * SQUARE_NUMBER + (j / 2) - 1];
				}
			}
			if ((i != (n - 1)) && (j > 1)) {
				// 右上のポリゴンを足す
				if (count / 2) {
					calcVec += faceQuadNormal[(i + 1) * SQUARE_NUMBER + (j / 2) - 1];
				}
				else {
					calcVec += faceQuadNormal[i * SQUARE_NUMBER + (j / 2) - 1];
				}
			}
			if ((i != 0) && (j < (2 * (n + 1)) - 2)) {
				// 左下のポリゴンを足す
				if (count / 2) {
					calcVec += faceQuadNormal[i * SQUARE_NUMBER + (j / 2)];
				}
				else {
					calcVec += faceQuadNormal[(i - 1) * SQUARE_NUMBER + (j / 2)];
				}
			}
			if ((i != (n - 1)) && (j < (2 * (n + 1)) - 2)) {
				// 右下のポリゴンを足す
				if (count / 2) {
					calcVec += faceQuadNormal[(i + 1) * SQUARE_NUMBER + (j / 2)];
				}
				else {
					calcVec += faceQuadNormal[i * SQUARE_NUMBER + (j / 2)];
				}
			}
			calcVec = XMVector3Normalize(calcVec);
			DirectX::XMStoreFloat3(&vertex[count].Normal, calcVec);
		}
		if (i != (n - 1)) {
			vertex[count].Normal = vertex[count - 1].Normal;
			count++;
			vertex[count].Normal = vertex[count - 2 * (n + 1)].Normal;
			count++;
		}
	}
	
	


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * vertexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//　頂点バッファ
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	/*
	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, vertex, (sizeof(VERTEX_3D) * vertexNum));
	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	*/
	//　インデックスバッファ
	WORD index[vertexNum];
	for (int i = 0; i < vertexNum; i++) {
		index[i] = i;
	}
	D3D11_BUFFER_DESC bd_vb;
	ZeroMemory(&bd, sizeof(bd_vb));
	bd_vb.Usage = D3D11_USAGE_DEFAULT;
	bd_vb.ByteWidth = sizeof(WORD) * vertexNum;
	bd_vb.BindFlags = D3D11_BIND_INDEX_BUFFER;	//　インデックスバッファ
	bd_vb.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sd_vb;
	ZeroMemory(&sd, sizeof(sd_vb));
	sd.pSysMem = vertex;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	
	




	
	m_Texture = new CTexture;
	m_Texture->Load("asset/field004.tga");
	
	m_WallTexture = new CTexture;
	m_WallTexture->Load("asset/wall.tga");
	
	


}

void CField::Uninit()
{
	m_VertexBuffer->Release();
	m_WallTexture->Unload();
	m_Texture->Unload();
	delete m_Texture;
}

void CField::Update()
{
	
}

void CField::Draw()
{
	XMMATRIX worldMtx;
	worldMtx = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	worldMtx *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	worldMtx *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	CRenderer::SetWorldMatrix(&worldMtx);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;


	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	CRenderer::SetTexture(m_Texture);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw((g_Num), 0);


	worldMtx *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.0f, 0.0f);
	worldMtx *= XMMatrixTranslation(0.0f, 0.0f, (SQUARE_SIZE * SQUARE_NUMBER) / 2);
	CRenderer::SetWorldMatrix(&worldMtx);
	CRenderer::SetTexture(m_WallTexture);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw((g_Num), 0);
	for (int i = 0; i < 3; i++) {
		//worldMtx *= XMMatrixTranslation(0.0f, 0.0f, (SQUARE_SIZE * SQUARE_NUMBER) / 2);
		worldMtx *= XMMatrixRotationY(XMConvertToRadians(90.0f));		
		CRenderer::SetWorldMatrix(&worldMtx);
		CRenderer::SetTexture(m_WallTexture);
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		CRenderer::GetDeviceContext()->Draw((g_Num), 0);
	}
	
}

