//==========================================================
/// @file
/// @brief    map.hの実装
/// @author   阿部拳之
///
/// @attention  ステージ内のマップ情報を保持するクラスです。

//==========================================================
#include "map.h"
//==========================================================

//==========================================================
// デフォルトコンストラクタ
//==========================================================
Map::Map() : Entity()
{
};

////==========================================================
// マップを描画
//==========================================================
void Map::draw()
{
	Image::draw();
}

//==========================================================
// マップデータの更新　引き数は更新したい座標値と更新する値
//==========================================================
void Map::updateMapData(const float x, const float y, const int val)
{
	// 座標をインデックスに変更
	int px = (int)(x / 32);
	int py = (int)(y / 32);

	// 座標値を更新
	mMapData[px][py] = val;
}

//==========================================================
// マップ当たり判定の更新 引き数は更新したい座標値と更新する値
// val -> 0 : 判定なし　val -> 1 : 判定あり/
//==========================================================
void Map::updateMapCol(const float x, const float y, const int val)
{
	if (x < 0 || x >= mapNS::MAP_HEIGHT * 32 || y < 0 || y >= mapNS::MAP_WIDTH * 32) { return; }

	// 座標値をインデックスに変更
	int px = (int)(x / 32);
	int py = (int)(y / 32);

	// 座標値を更新
	mColData[px][py] = val;
}

//==========================================================
// マップ上のオブジェクトデータ更新　引き数は更新したい座標値と更新する値
//==========================================================
void Map::updateMapObj(const float x, const float y, const int val)
{
	// 座標値をインデックスに変更
	int px = (int)(x / 32);
	int py = (int)(y / 32);

	// 座標値を更新
	mObjData[px][py] = val;
}

//==========================================================
// マップ上のオブジェクトデータ更新　引き数は更新したい座標値と更新する値
//==========================================================
void Map::updateMapObjInt(const int y, const int x, const int val)
{
	// 座標値を更新
	mObjData[y][x] = val;
}

//==========================================================
// 指定されたステージのマップデータを読み込む
//==========================================================
void Map::readMapFile(const int stageNum)
{
	string mapDataFilename = "stageData\\stage" + std::to_string(stageNum) + "\\mapdata.csv";
	string colDataFilename = "stageData\\stage" + std::to_string(stageNum) + "\\coldata.csv";
	string objDataFilename = "stageData\\stage" + std::to_string(stageNum) + "\\objdata.csv";

	ifstream* ifs = new ifstream(mapDataFilename);
	if (!ifs)
		exit(1);
	//csvファイルを1行ずつ読み込む
	string str;
	int y = 0, x = 0;
	while (getline(*ifs, str)) {
		string token;
		istringstream stream(str);
		x = 0;
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			int temp = (int)stof(token); //stof(string str) : stringをfloatに変換
			mMapData[y][x] = temp;
			x += 1;
			if (x == mapNS::MAP_WIDTH)
				break;
		}
		y += 1;
		if (y == mapNS::MAP_HEIGHT)
			break;
	}
	safeDelete(ifs);
	ifs = new ifstream(colDataFilename);
	if (!ifs)
		exit(1);
	//csvファイルを1行ずつ読み込む
	y = 0;
	x = 0;
	while (getline(*ifs, str)) {
		string token;
		istringstream stream(str);
		x = 0;
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			int temp = (int)stof(token); //stof(string str) : stringをfloatに変換
			mColData[y][x] = temp;
			mColDataInit[y][x] = temp;
			x += 1;
			if (x == mapNS::MAP_WIDTH)
				break;
		}
		y += 1;
		if (y == mapNS::MAP_HEIGHT)
			break;
	}

	safeDelete(ifs);
	ifs = new ifstream(objDataFilename);
	if (!ifs)
		exit(1);
	//csvファイルを1行ずつ読み込む
	y = 0;
	x = 0;
	while (getline(*ifs, str)) {
		string token;
		istringstream stream(str);
		x = 0;
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			int temp = (int)stof(token); //stof(string str) : stringをfloatに変換
			mObjData[y][x] = temp;
			x += 1;
			if (x == mapNS::MAP_WIDTH)
				break;
		}
		y += 1;
		if (y == mapNS::MAP_HEIGHT)
			break;
	}

	safeDelete(ifs);
}