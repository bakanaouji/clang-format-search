//==========================================================
/// @file
/// @brief    Enemyクラス
/// @author   阿部拳之
///
/// @attention  雑魚敵を表すクラスです。
///				バリケードを破壊しながら城に向かって攻めてくるので、
///				プレイヤーは勇者を操作して倒します。

//==========================================================
#ifndef _ENEMY_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _ENEMY_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "character.h"
#include "constants.h"
#include "map.h"
#include "barricade.h"
#include "enemyAttackCollision.h"

//==========================================================

// Enemyクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace enemyNS
{
	const int WIDTH = 32;							// 画像の幅（各フレーム）
	const int HEIGHT = 32;							// 画像の高さ
	const int TEXTURE_COLS = 12;					// テクスチャは12列
	const int MOVE_UP_START_FRAME = 54;				// 上方向移動のアニメーションはフレーム54から開始
	const int MOVE_UP_END_FRAME = 56;				// 上方向移動のアニメーションフレームは54、55、56
	const int MOVE_RIGHT_START_FRAME = 66;			// 右方向移動のアニメーションはフレーム66から開始
	const int MOVE_RIGHT_END_FRAME = 68;			// 右方向移動のアニメーションフレームは66、67、68
	const int MOVE_DOWN_START_FRAME = 78;			// 下方向移動のアニメーションはフレーム78から開始
	const int MOVE_DOWN_END_FRAME = 80;				// 下方向移動のアニメーションフレームは78、79、80
	const int MOVE_LEFT_START_FRAME = 90;			// 左方向移動のアニメーションはフレーム90から開始
	const int MOVE_LEFT_END_FRAME = 92;				// 左方向移動のアニメーションフレームは90、91、92
	const float MOVE_ANIMATION_DELAY = 0.1f;		// 移動アニメーションのフレーム間の時間
	const int ATTACK_UP_START_FRAME = 151;			// 上方向攻撃のアニメーションはフレーム151から開始
	const int ATTACK_UP_END_FRAME = 151 + 36;		// 上方向攻撃にアニメーションフレームは151、163、175、184
	const int ATTACK_RIGHT_START_FRAME = 148;		// 右方向攻撃のアニメーションはフレーム148から開始
	const int ATTACK_RIGHT_END_FRAME = 148 + 36;	// 右方向攻撃のアニメーションフレームは148、161、173、182
	const int ATTACK_DOWN_START_FRAME = 103;		// 下方向攻撃のアニメーションはフレーム103から開始
	const int ATTACK_DOWN_END_FRAME = 103 + 36;		// 下方向攻撃のアニメーションフレームは103、115、127、139
	const int ATTACK_LEFT_START_FRAME = 100;		// 左方向攻撃のアニメーションはフレーム100から開始
	const int ATTACK_LEFT_END_FRAME = 100 + 36;		// 左方向攻撃のアニメーションフレームは100、112、124、136
	const float ATTACK_TIME = 0.8f;					// プレイヤーに隣接してから攻撃に移るまでの溜め時間
	const float CONTINUE_ATTACK_TIME = 0.8f;		// 連続で攻撃する場合の溜め時間
	const float DAMAGE_TIME = 1.0f;					// ダメージを受けている場合、DAMAGE_TIMEごとに画像が点滅
	enum TYPE { NORMAL, RED, BLUE };                // 敵の種類
}

// 雑魚敵を表すクラスです。
// バリケードを破壊しながら城に向かって攻めてくるので、
// プレイヤーは勇者を操作して倒します。
class Enemy : public Character
{
protected:
	Barricade* mBarricades;					// バリケードの配列へのポインタ
	EnemyCollision mAttackCollision;		// 攻撃判定用のコリジョン
	characterNS::DIRECTION mOldDirection;	// 攻撃直前に向いていた方向を格納
	characterNS::DIRECTION mGoalDirection;	// 次に進むべき方向
	bool mIsAttacked;						// 攻撃した直後かどうか
	bool mCanMakeDecesionMove;				// 移動に関しての意思決定を行えるかどうか
	int mNearBarricadeIndex;				// 最近接のバリケードのインデックス
	float mAttackTimer;						// 攻撃アニメーション用のタイマー
	float mContinueAttackTime;				// 連続で攻撃する場合の溜め時間
	float mAttackTime;						// プレイヤーに隣接してから攻撃に移るまでの溜め時間
	imageNS::MODE mAttackAnimationMode;		// 攻撃時のアニメーションの遷移モード（水平か垂直方向）
	enemyNS::TYPE mType;                    // 敵の種類
public:
	// コンストラクタ
	Enemy();

	//==========================================================
	// 継承されたメンバー関数
	//==========================================================
	
	// Update
	// 攻撃完了直後の状態遷移や移動などの行動を行う。
	// 全体的な状態遷移については、衝突判定によって行うためstage.collisions()に記載
	// 通常、フレームごとに1回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
	virtual void update(const float frameTime);
	// 人工知能。NPCの行動を決定するのに使用
	// 引数：frameTime　1フレームで経過した時間
	// 引数：ent		他のエンティティ
	virtual void ai(const float frameTime, const Entity &ent) {};
	// ダメージ処理
	// WEAPONの種類によって受けるダメージが分岐
	// 引数：weapon	WEAPONの種類
	virtual void damage(const WEAPON weapon);
	// パラメータリセット
	// roundStart()内で呼び出される
	virtual void reset();

	//==========================================================
	// 新しく追加するメンバー関数
	//==========================================================

	//==========================================================
	// getter
	//==========================================================
	
	// 次に進むべき方向を返す関数
	// 戻り値：次に進むべき方向
	characterNS::DIRECTION getGoalDirection() { return mGoalDirection; }
	// 移動に関しての意思決定を行えるかどうか
	// 戻り値：移動に関しての意思決定を行えるかどうか
	bool canMakeDecesionMoving() { return mCanMakeDecesionMove; }
	// 最近接のバリケードのインデックスを返す
	// 戻り値：最近接のバリケードのインデックス
	int getNearBarricadeIndex() { return mNearBarricadeIndex; }
	// 攻撃判定用のコリジョンを返す
	// 戻り値：攻撃の当たり判定用のエンティティ
	EnemyCollision& getAttackCollision() { return mAttackCollision; }
	// 敵の種類を取得する関数
	// 戻り値：敵の種類
	enemyNS::TYPE getEnemyType() { return mType; }
	

	//==========================================================
	// setter
	//==========================================================
	
	// バリケードの配列へのポインタをセットする関数
	// 引数：*barricades	バリケードの配列へのポインタ
	void setBarricadesPointer(Barricade *barricades) { mBarricades = barricades; }
	// 次に進むべき方向をセットする関数
	// 引数：goalDirection	次に進むべき方向
	void setGoalDirection(const characterNS::DIRECTION goalDirection) { mGoalDirection = goalDirection; }
	// 敵の種類をセットする関数
	// 引数：type	敵の種類
	void setEnemyType(const enemyNS::TYPE type){mType = type;}

	// 向きをgoalDirectionへとアップデートする関数
	// 引数：strF	goalDirectionの向きのアニメーションの最初のフレーム
	// 引数：endF	goalDirectionの向きのアニメーションの最後のフレーム
	void changeDirection(const int strF, const int endF);
	// 一番近くのバリケードのインデックスを返す関数
	int searchNearBarricadeIndex();
	// 雑魚敵から城までの直線上にバリケードが存在したらtrue、存在しない場合はfalseを返す。
	bool checkBarricadeOnLine();
	// 攻撃モードにチェンジするときに呼び出す関数
	// 引数：collisionVector	衝突しているエンティティへのベクトル
	virtual void changeAttack(VECTOR2 &collisionVector);
	// 攻撃モードにチェンジするときに呼び出す関数
	// 引数：dir	衝突しているエンティティがいる方向
	virtual void changeAttack(const characterNS::DIRECTION dir);
	// タイルの中央にキャラクターがいるかどうか
	virtual bool isCenterOfTile();
	// 指定した方向に進めるかどうか
	// 引数：dir	進めるか確認したい方向
	bool canMoveTo(const characterNS::DIRECTION dir);
};
#endif
