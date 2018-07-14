//==========================================================
/// @file
/// @brief    map.h�̎���
/// @author   �������V
///
/// @attention  �X�e�[�W���̃}�b�v����ێ�����N���X�ł��B

//==========================================================
#include "map.h"
//==========================================================

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
Map::Map() : Entity()
{
};

////==========================================================
// �}�b�v��`��
//==========================================================
void Map::draw()
{
	Image::draw();
}

//==========================================================
// �}�b�v�f�[�^�̍X�V�@�������͍X�V���������W�l�ƍX�V����l
//==========================================================
void Map::updateMapData(const float x, const float y, const int val)
{
	// ���W���C���f�b�N�X�ɕύX
	int px = (int)(x / 32);
	int py = (int)(y / 32);

	// ���W�l���X�V
	mMapData[px][py] = val;
}

//==========================================================
// �}�b�v�����蔻��̍X�V �������͍X�V���������W�l�ƍX�V����l
// val -> 0 : ����Ȃ��@val -> 1 : ���肠��/
//==========================================================
void Map::updateMapCol(const float x, const float y, const int val)
{
	if (x < 0 || x >= mapNS::MAP_HEIGHT * 32 || y < 0 || y >= mapNS::MAP_WIDTH * 32) { return; }

	// ���W�l���C���f�b�N�X�ɕύX
	int px = (int)(x / 32);
	int py = (int)(y / 32);

	// ���W�l���X�V
	mColData[px][py] = val;
}

//==========================================================
// �}�b�v��̃I�u�W�F�N�g�f�[�^�X�V�@�������͍X�V���������W�l�ƍX�V����l
//==========================================================
void Map::updateMapObj(const float x, const float y, const int val)
{
	// ���W�l���C���f�b�N�X�ɕύX
	int px = (int)(x / 32);
	int py = (int)(y / 32);

	// ���W�l���X�V
	mObjData[px][py] = val;
}

//==========================================================
// �}�b�v��̃I�u�W�F�N�g�f�[�^�X�V�@�������͍X�V���������W�l�ƍX�V����l
//==========================================================
void Map::updateMapObjInt(const int y, const int x, const int val)
{
	// ���W�l���X�V
	mObjData[y][x] = val;
}

//==========================================================
// �w�肳�ꂽ�X�e�[�W�̃}�b�v�f�[�^��ǂݍ���
//==========================================================
void Map::readMapFile(const int stageNum)
{
	string mapDataFilename = "stageData\\stage" + std::to_string(stageNum) + "\\mapdata.csv";
	string colDataFilename = "stageData\\stage" + std::to_string(stageNum) + "\\coldata.csv";
	string objDataFilename = "stageData\\stage" + std::to_string(stageNum) + "\\objdata.csv";

	ifstream* ifs = new ifstream(mapDataFilename);
	if (!ifs)
		exit(1);
	//csv�t�@�C����1�s���ǂݍ���
	string str;
	int y = 0, x = 0;
	while (getline(*ifs, str)) {
		string token;
		istringstream stream(str);
		x = 0;
		//1�s�̂����A������ƃR���}�𕪊�����
		while (getline(stream, token, ',')) {
			//���ׂĕ�����Ƃ��ēǂݍ��܂�邽��
			//���l�͕ϊ����K�v
			int temp = (int)stof(token); //stof(string str) : string��float�ɕϊ�
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
	//csv�t�@�C����1�s���ǂݍ���
	y = 0;
	x = 0;
	while (getline(*ifs, str)) {
		string token;
		istringstream stream(str);
		x = 0;
		//1�s�̂����A������ƃR���}�𕪊�����
		while (getline(stream, token, ',')) {
			//���ׂĕ�����Ƃ��ēǂݍ��܂�邽��
			//���l�͕ϊ����K�v
			int temp = (int)stof(token); //stof(string str) : string��float�ɕϊ�
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
	//csv�t�@�C����1�s���ǂݍ���
	y = 0;
	x = 0;
	while (getline(*ifs, str)) {
		string token;
		istringstream stream(str);
		x = 0;
		//1�s�̂����A������ƃR���}�𕪊�����
		while (getline(stream, token, ',')) {
			//���ׂĕ�����Ƃ��ēǂݍ��܂�邽��
			//���l�͕ϊ����K�v
			int temp = (int)stof(token); //stof(string str) : string��float�ɕϊ�
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