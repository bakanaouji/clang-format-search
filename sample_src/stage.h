//==========================================================
/// @file
/// @brief    Stageクラス
/// @author   阿部拳之
///
/// @attention  ステージ情報を管理するクラスです。
///				勇者、敵、マップ情報、バリケード、城などステージに関する情報はここで管理します。

//==========================================================
#ifndef _STAGE_H	// このファイルが複数の箇所でインクルードされる場合に、
#define _STAGE_H	// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "enemy.h"
#include "midBoss.h"
#include "brave.h"
#include "castle.h"
#include "textureManager.h"

//==========================================================
// ステージ情報を管理するクラスです。
// 勇者、敵、マップ情報、バリケード、城などステージに関する情報はここで管理します。
class Stage
{
private:
	Map mMap;									// ステージのマップ情報
	Barricade mBarricades[mapNS::BARRICADE_NUM];// バリケードの配列
	Brave mBrave;								// プレイヤー
	Castle mCastle;								// 城
	Enemy **mEnemy;	    						// 敵へのポインタの配列
	float *mEnemyX;								// 敵の初期位置Xの配列
	float *mEnemyY;								// 敵の初期位置Yの配列
	float mRemainingTime;						// ゲーム内の残り時間
	int mEnemyNum;								// 敵の数
	int mStageNum;								// 選択しているステージの番号
	int mEnemyWaveNum;							// 現在の敵の波の番号（第一波、第二波、、、）、第一波をすべて倒したら第二波が、、と進む
	bool    mRoundOver;							// ラウンドが終了した場合、true
	bool mClearedStage;							// ステージをクリアした場合、true
	bool mInitializedEnemies;					// 敵を初期化できたらtrue
public:
	// コンストラクタ
	Stage();
	// デストラクタ
	~Stage();

	//==========================================================
	// getter
	//==========================================================

	// マップ情報を返す
	// 戻り値：マップ情報
	Map& getMap() { return mMap; }
	// 敵の数を返す
	// 戻り値：敵の数
	int getEnemyNum() { return mEnemyNum; }
	// ステージ番号を返す
	// 戻り値：ステージ番号
	int getStageNum() { return mStageNum; }
	// ゲーム内の残り時間を返す
	// 戻り値：ゲーム内の残り時間
	float getRemainingTime() { return mRemainingTime; }
	// 指定したインデックスの敵を返す
	// 戻り値：指定したインデックスの敵
	Enemy& getEnemy(int index) { return *mEnemy[index]; }
	// 勇者を返す
	// 戻り値：勇者
	Brave& getBrave() { return mBrave; }
	// 指定したインデックスのバリケードを返す
	// 戻り値：指定したインデックスのバリケード
	Barricade& getBarricade(int index) { return mBarricades[index]; }
	// 城を返す
	// 戻り値：城
	Castle& getCastle() { return mCastle; }
	// ゲームオーバーかどうかを返す
	// 戻り値：ゲームオーバーかどうか
	bool getRoundOver() { return mRoundOver; }
	// ステージをクリアしたかどうかを返す
	// 戻り値：ステージをクリアしたかどうか
	bool getClearedStage() { return mClearedStage; }
	// 敵を初期化できているかどうか
	// 戻り値：敵を初期化できているかどうか
	bool getInitializedEnemies() { return mInitializedEnemies; }


	//==========================================================
	// setter
	//==========================================================
	// ステージをクリアしたかどうかをセット
	// 引数：cs	ステージをクリアしたかどうか
	void setClearedStage(bool cs) { mClearedStage = cs; }
	// ステージ番号をセット
	// 引数：sn	ステージ番号
	void setStageNum(int sn) { mStageNum = sn; }
	// 敵を初期化できているかどうかをセット
	// 引数：initializedE	敵を初期化できているかどうか
	void setInitializedEnemies(bool initializedE) { mInitializedEnemies = initializedE; }
	// ゲームオーバーかどうかをセット
	// 引数：roundOverゲームオーバーかどうか
	void setRoundOver(bool roundOver) { mRoundOver = roundOver; }

	// ステージ情報を初期化
	void reset();
	// 現在のステージ番号でゲームスタート
	void roundStart();
	// 人工知能
	// 各敵の人工知能を計算
	// 引数：frameTime　1フレームで経過した時間
	void ai(float frameTime);
	// 衝突を処理
	// 処理する衝突は以下の通り：
	// 勇者の攻撃と各敵との衝突
	// 各敵の攻撃と勇者や城、バリケードとの衝突
	// 勇者の必殺技（炎）と各敵の衝突
	// 各敵と勇者の衝突
	// 勇者の攻撃とバリケードとの衝突
	// 各敵とほかのエンティティとの衝突状況によって、敵の状態を分岐
	// ①攻撃中はそのまま攻撃
	// ②勇者と衝突している場合は勇者に対しての攻撃状態に遷移
	// ③城と衝突している場合は城に対しての攻撃状態に遷移
	// ④最近接のバリケードに衝突している場合はバリケードに対しての攻撃状態に遷移
	// ⑤いずれかのマップタイルの中央まで移動した場合、
	//   最近接のバリケードに向かって移動状態に遷移（1/3くらいの確率で方向をランダムに決めるようにしている）
	void collisions();
	// stageを1ステップ分進める
	// 引数：frameTime　1フレームで経過した時間
	void runStage(float frameTime);
	// 敵の数をチェックし、マップ上に敵がいなければ新たに生成
	void checkCurrentEnemyNum();
	// 指定されたステージの敵データを読み込む
	// 引数：stageNum		ステージ番号
	// 引数：enemyWave		第何派目の敵の群か
	bool readEnemyFile(int stageNum, int enemyWave);
	// 指定されたステージ、派の敵データを読み込み敵を初期化する
	// 引数：stageNum		ステージ番号
	// 引数：enemyWave		第何派目の敵の群か
	void initializeEnemies(int stageNum, int enemyWave);
};
#endif