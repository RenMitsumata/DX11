#include "main.h"
#include "renderer.h"
#include <vector>
#include <string>
#include "Cource.h"
#pragma comment(lib,"d3d11.lib")
using namespace std;

#define FILENAME ("cource/test.txt")
#define STARTPOS (XMFLOAT3(0.0f,0.0f,0.0f))
#define COURCEWIDTH (10.0f)
#define DRAWLENGTH (500.0f)	//�@�`�拗���i�O�せ�����j

typedef struct CourceVtx_Tag {
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 texcoord;
}CourceVtx;


Cource::Cource()
{
}


Cource::~Cource()
{
}

//�@�R�[�X�f�[�^�̓ǂݍ��݋y�сA���̑������ݒ�
void Cource::Init(void) {
	//�@�R�[�X�t�@�C���̓ǂݍ���
	//�@�R�[�X�f�[�^��distance,yaw,pitch,roll,radius,insline
	FILE* fp;
	fp = fopen(FILENAME, "rt");
	char buffer[64];
	//�@�w�b�_������ǂݔ�΂�
	for (int i = 0; i < 4; i++) {
		fgets(buffer, 64, fp);
	}
	//	Cource Length:��ǂݔ�΂�
	fgets(buffer, 64, fp);

	//�@�R�[�X�̒����i�����j�������o��
	fscanf(fp, "%d", &courceLength);
	fscanf(fp, "\n\n", NULL);

	
	//�@�����o�����R�[�X�̒������ACourceData�𓮓I�m��
	courceData = new CourceData*[courceLength * 8];
	for (int i = 0; i < (courceLength * 8); i++) {
		courceData[i] = new CourceData;
	}

	//�@�R�[�X�f�[�^����A���S���W�y�юl���̒��_�̈ʒu���v�Z
	XMFLOAT3 courceFront = { 0.0f,0.0f,1.0f };
	XMFLOAT3 courceRight = { 1.0f,0.0f,0.0f };
	XMMATRIX rotateMtx;
	float deltaYaw = 0.0f;			//�@��Yaw�i�P�ʋ���������ǂꂭ�炢�X�����ԂȂ̂��i�J�[�u���Ȃǁj�j
	float deltaPitch = 0.0f;		//�@deltaYaw��Pitch��
	float deltaRoll = 0.0f;			//	deltaYaw��Roll��
	float radius = 0.0f;			//�@�x����]���́A��]���a�B
	float incline = 0.0f;			//�@�w����]���́A��]���a�B

	vector<int> distancelist;
	vector<float> curveStatus[3];

	int disBuf;
	int k = 0;
	while (fscanf(fp,"%d ",&disBuf) != EOF) {
		//�@�֐��H��ǂ݉���
		float curveData[3];
		
		// curveData�̒��g�F[0]Roll��]�ʁA[1]�Ȑ����a(Y��)�A[2]�Ȑ����a(Right��)
		fscanf(fp,"%[^:]:",buffer);
		string moji = buffer;
		if (moji == "Curve") {
			fscanf(fp, "%f,", &curveData[0]);
			fscanf(fp, "%f,", &curveData[1]);
			fscanf(fp, "%f,", &curveData[2]);
		}
		else if(moji == "CurveEnd"){
			curveData[0] = 0.0f;
			curveData[1] = 0.0f;
			curveData[2] = 0.0f;
		}



		fscanf(fp, "\n", NULL);
		curveStatus[0].push_back(curveData[0]);
		curveStatus[1].push_back(curveData[1]);
		curveStatus[2].push_back(curveData[2]);
		distancelist.push_back(disBuf);
		k++;
	}
	fclose(fp);


	//�@�R�[�X�f�[�^�̐���
	for (int i = 0; i < (courceLength * 8); i++) {
		courceData[i]->distance = i * 0.125f;
		if (i == 0) {
			courceData[i]->centerpos = STARTPOS;
		}
		else {
			int Num = -1;
			bool bChange = false;
			for (int j = 0; j < k; j++) {
				if (courceData[i]->distance == distancelist[j]) {
					Num = j;
					bChange = true;
					break;
				}
			}
			// deltaYaw,deltaPitch,deltaRoll�̍X�V
			if (bChange == true) {
				radius = curveStatus[1][Num];			//�@�x����]���́A��]���a�B
				incline = curveStatus[2][Num];		//�@�w����]���́A��]���a�B
				if (radius == 0.0f) {
					deltaYaw = 0.0f;
				}
				else {
					deltaYaw = 0.125f / radius;
				}
				if (incline == 0.0f) {
					deltaRoll = 0.0f;
				}
				else {
					deltaPitch = 0.125f / incline;
				}
				deltaRoll = 0;
			}


			// Yaw,Pitch,Roll�̍X�V
			yaw += deltaYaw;
			pitch += deltaPitch;
			roll += deltaRoll;

			rotateMtx = XMMatrixIdentity();
			rotateMtx *= XMMatrixRotationRollPitchYaw(deltaRoll, deltaPitch, deltaYaw);
			XMStoreFloat3(&courceFront , XMVector3TransformNormal(XMLoadFloat3(&courceFront), rotateMtx));
			XMStoreFloat3(&courceRight, XMVector3TransformNormal(XMLoadFloat3(&courceRight), rotateMtx));
			
			courceData[i]->centerpos = courceData[i - 1]->centerpos + courceFront * 0.125f;
			courceData[i]->vecFront = courceFront;
		}

		//	MF�̊e���_���W	�@�@ =�@�@�@���S�|�W�V����     +/-�@�@�@�@�@�R�[�X���@�@�@�@�@�@ +/-�@�@�@�@�O�㕝
		courceData[i]->vtxpos[0] = courceData[i]->centerpos - (courceRight / 2) * COURCEWIDTH + (courceFront / 2) * 0.125f;
		courceData[i]->vtxpos[1] = courceData[i]->centerpos + (courceRight / 2) * COURCEWIDTH + (courceFront / 2) * 0.125f;
		courceData[i]->vtxpos[2] = courceData[i]->centerpos - (courceRight / 2) * COURCEWIDTH - (courceFront / 2) * 0.125f;
		courceData[i]->vtxpos[3] = courceData[i]->centerpos + (courceRight / 2) * COURCEWIDTH - (courceFront / 2) * 0.125f;
	}
}

void Cource::Draw(int currentPos) {
	//�@���b�V���t�B�[���h�̕`��
	
	int maxnum = max(0, currentPos * 8 - DRAWLENGTH);
	int minnum = min(courceLength * 8, currentPos * 8 + DRAWLENGTH);
	for (int i = maxnum; i < minnum; i++) {
		CourceVtx Vtx[4] = {
			{ courceData[i]->vtxpos[0],XMFLOAT4(1.0f,1.0f,1.0f,0.75f),XMFLOAT2(0,0) },
			{ courceData[i]->vtxpos[1],XMFLOAT4(1.0f,1.0f,1.0f,0.75f),XMFLOAT2(1,0) },
			{ courceData[i]->vtxpos[2],XMFLOAT4(1.0f,1.0f,1.0f,0.75f),XMFLOAT2(0,1) },
			{ courceData[i]->vtxpos[3],XMFLOAT4(1.0f,1.0f,1.0f,0.75f),XMFLOAT2(1,1) },
		};
		XMMATRIX worldMtx;
		worldMtx = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		worldMtx *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		worldMtx *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		CRenderer::SetWorldMatrix(&worldMtx);

		
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		CRenderer::GetDeviceContext()->Draw(2, 0);

	}

}

void Cource::Uninit(void) {
	//�@�R�[�X�t�@�C���̍폜�i�����[�X�j
	for (int i = 0; i < courceLength * 8; i++) {
		delete courceData[i];
	}
	delete courceData;
}

XMFLOAT3 Cource::GetCource(float distance) {
	float bufDis = distance;
	bufDis *= 8;
	int bufInt = bufDis;
	if (bufInt - bufDis == 0) {
		return courceData[bufInt]->centerpos;
	}
	else {
		float a = distance * 8 - bufInt;
		float b = (bufInt + 1) - distance * 8;
		XMFLOAT3 currentPos;
		currentPos = ((a / (a + b)) * courceData[bufInt]->centerpos + (b / (a + b)) * courceData[bufInt + 1]->centerpos);
		return currentPos;
	}

}

XMFLOAT3 Cource::GetTild(float distance) {
	float bufDis = distance;
	bufDis *= 8;
	int bufInt = bufDis;
	if (bufInt - bufDis == 0) {
		return courceData[bufInt]->vecFront;
	}
	else {
		float a = distance - (bufInt / 8);
		float b = ((bufInt + 1) / 8) - distance;
		XMFLOAT3 currentFront;
		currentFront = ((a / (a + b)) * courceData[bufInt]->vecFront + (b / (a + b)) * courceData[bufInt + 1]->vecFront);
		XMStoreFloat3(&currentFront , XMVector3Normalize(XMLoadFloat3(&currentFront)));
		
		return currentFront;
	}
}