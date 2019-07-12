

#ifndef COURCE_H
#define COURCE_H
//-----------------------------------

#define COURCE_LENGTH


//　CourceData：コース（メッシュフィールド）の座標および、プレイヤーの特定の時間における座標を表すもの。
typedef struct CourceData_Tag {
	float distance;			//　スタート地点（０）からの距離。メッシュフィールド用。
	XMFLOAT3 centerpos;	//　単位距離当たりのメッシュフィールドの中心座標
	XMFLOAT3 vtxpos[4];	//　コース（メッシュフィールド）用に計算された頂点座標を収納する。
	XMFLOAT3 vecFront;	//　コースの進行方向。計算に用いる
}CourceData;

class Cource
{
private:
	int courceLength;		//　コースの長さ
	CourceData** courceData;
	float yaw;				//　ニュートラル（playerのfrontがＺ＋方向を向いた状態）から、どれだけＹ軸回転をしているかを表す値。
	float pitch;			//　ニュートラルから、どれだけＸ軸回転をしているかを表す値。
	float roll;				//　ニュートラルから、どれだけＺ軸回転をしているかを表す値。
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

////////　オペレータのオーバーロード
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

