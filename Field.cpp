#include "main.h"
#include "texture.h"

#include "renderer.h"
#include "GameObject.h"
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
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < (2 * (n + 1)); j++,count++) {
			vertex[count].Position.x = m_Position.x - (n * 0.5f - (j % 2) - i) * SQUARE_SIZE;
			vertex[count].Position.y = m_Position.y;
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

