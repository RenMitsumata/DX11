

#ifndef COURCE_H
#define COURCE_H
//-----------------------------------

#define COURCE_LENGTH


//�@CourceData�F�R�[�X�i���b�V���t�B�[���h�j�̍��W����сA�v���C���[�̓���̎��Ԃɂ�������W��\�����́B
typedef struct CourceData_Tag {
	float distance;			//�@�X�^�[�g�n�_�i�O�j����̋����B���b�V���t�B�[���h�p�B
	XMFLOAT3 centerpos;	//�@�P�ʋ���������̃��b�V���t�B�[���h�̒��S���W
	XMFLOAT3 vtxpos[4];	//�@�R�[�X�i���b�V���t�B�[���h�j�p�Ɍv�Z���ꂽ���_���W�����[����B
	XMFLOAT3 vecFront;	//�@�R�[�X�̐i�s�����B�v�Z�ɗp����
}CourceData;

class Cource
{
private:
	int courceLength;		//�@�R�[�X�̒���
	CourceData** courceData;
	float yaw;				//�@�j���[�g�����iplayer��front���y�{��������������ԁj����A�ǂꂾ���x����]�����Ă��邩��\���l�B
	float pitch;			//�@�j���[�g��������A�ǂꂾ���w����]�����Ă��邩��\���l�B
	float roll;				//�@�j���[�g��������A�ǂꂾ���y����]�����Ă��邩��\���l�B
public:
	Cource();
	virtual ~Cource();
	void Init(void);
	void Draw(int currentPos);
	void Uninit(void);
	XMFLOAT3 GetCource(float distance);
	XMFLOAT3 GetTild(float distance);
};
//-----------------------------------
#endif	// COURCE_H

////////�@�I�y���[�^�̃I�[�o�[���[�h
XMFLOAT3 operator + (XMFLOAT3 a, XMFLOAT3 b) {
	XMFLOAT3 ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return ret;
}

XMFLOAT3 operator - (XMFLOAT3 a, XMFLOAT3 b) {
	XMFLOAT3 ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return ret;
}

XMFLOAT3 operator * (XMFLOAT3 a, float b) {
	XMFLOAT3 ret;
	ret.x = a.x * b;
	ret.y = a.y * b;
	ret.z = a.z * b;
	return ret;
}

XMFLOAT3 operator * (float a, XMFLOAT3 b) {
	XMFLOAT3 ret;
	ret.x = a * b.x;
	ret.y = a * b.y;
	ret.z = a * b.z;
	return ret;
}

XMFLOAT3 operator / (XMFLOAT3 a, float b) {
	XMFLOAT3 ret;
	ret.x = a.x / b;
	ret.y = a.y / b;
	ret.z = a.z / b;
	return ret;
}

