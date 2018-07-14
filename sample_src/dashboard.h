//==========================================================
/// @file
/// @brief    ダッシュボード関連のクラス
/// @author   阿部拳之
///
/// @attention  ダッシュボード（棒グラフ、スイッチなど）を表すクラスです。
///				システムグラフィックスとして使用することができます。

//==========================================================
#ifndef _DASHBOARD_H            // このファイルが複数の箇所でインクルードされる場合に、 
#define _DASHBOARD_H            // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include <sstream>
#include "image.h"
#include "constants.h"
#include "textureManager.h"
#include "input.h"

//==========================================================

// ダッシュボードの定数
// ゲーム内でのステータス以外をここに記述
namespace dashboardNS
{
	const int	WIDTH = 48;				// 画像の幅（各フレーム）
	const int	HEGIHT = 48;			// 画像の高さ
    const int   TEXTURE_COLS = 4;       // テクスチャは4列
    const int   BAR_FRAME = 0;          // それぞれのテクスチャのフレーム番号
    const int   DIAL360_FRAME = 1;      // "
    const int   DIAL270_FRAME = 2;      // ...
    const int   DIAL180_FRAME = 3;
    const int   LIGHT_FRAME = 4;
    const int   POINTER_FRAME = 5;
    const int   SEGMENT_FRAME = 6;
    const int   DECIMAL_FRAME = 7;
    const int   SWITCH_OFF_FRAME = 8;
    const int   SWITCH_ON_FRAME = 9;
    const int   BUTTON_UP_FRAME = 10;
    const int   BUTTON_DOWN_FRAME = 11;
    const int   BAR_GRAPH_FRAME = 12;
    const int   SWITCH_WIDTH = 44;      // トグルスイッチの幅（ピクセル単位）
    const int   BAR_GRAPH_WIDTH = 8;    // 棒グラフの幅＋空きスペース
    enum DialType{DIAL360, DIAL270, DIAL180};
}

// セブンセグメントを表すクラスです。
// システムグラフィックスとして使用することができます。
class SevenSegment : public Image
{
private:
    Image   mDecimal;	// セブンセグメントの画像
    UINT    mDigits;	// 桁数
    double  mNumber;	// 表示する数値
public:
	// コンストラクタ
    SevenSegment();
	// セブンセグメント表示の初期化
	// 引数：*graphics	Graphicsオブジェクトへのポインタ
	// 引数：*textureM	TextureManagerオブジェクトへのポインタ
	// 引数：left、top	画面位置
	// 引数：scale		倍率（ズーム）
	// 引数：digits		桁数
	// 引数：color		数字の色
	// 戻り値：成功した場合はtrue、エラーの場合はfalseを戻す
    bool initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                    const float scale, UINT digits, const COLOR_ARGB color);
	// セブンセグメント表示に表示する数値を設定
	// 引数：value	表示する数値
    void set(const double value);
	// Update
	// 通常、フレームごとに1回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
    virtual void update(const float frameTime)    {}
	// セブンセグメントの数字「0」～「9」と「-」を表示
	// 引数：n	表示する数値
	// 引数：color	表示する色
    void drawDigit(const char n, const COLOR_ARGB color);
	// 小数点を描画
	// 引数：color	表示する色
    void drawDecimal(const COLOR_ARGB color);
	// セブンセグメント表示を描画
	// number変数には、表示する浮動小数点数値が格納されている
	// 引数：color	表示する色（デフォルトでは白）
	virtual void draw(const COLOR_ARGB color = graphicsNS::WHITE);
};

// バーを表すクラスです。
// システムグラフィックスとして使用することができます。
class Bar : public Image
{
public:
	// Barを初期化
	// 引数：*graphics	Graphicsオブジェクトへのポインタ
	// 引数：*textureM	TextureManagerオブジェクトへのポインタ
	// 引数：left、top	画面位置
	// 引数：scale		倍率（ズーム）
	// 引数：color		バーの色
	// 戻り値：成功した場合はtrue、エラーの場合はfalseを戻す
    bool initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                    const float scale, const COLOR_ARGB color);
	// バーのサイズを設定
	// 引数：percentOn	バーのサイズ
    void set(float percentOn);
	// Update
	// 通常、フレームごとに1回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
    virtual void update(const float frameTime)    {}
};

// 円グラフを表すクラスです。
// システムグラフィックスとして使用することができます。
class DialGauge : public Image
{
private:
    Image   mPointer;					// 画像
    dashboardNS::DialType mDialType;	// 円盤の種類
public:
	// DialGaugeを初期化
	// 引数：*graphics	Graphicsオブジェクトへのポインタ
	// 引数：*textureM	TextureManagerオブジェクトへのポインタ
	// 引数：left、top	画面位置
	// 引数：scale		倍率（ズーム）
	// 引数：type		円盤の種類
	// 引数：zeroAngle	円盤のゼロの位置
	// 引数：dialColor	円盤の色
	// 引数：pointColor	数字の色
	// 戻り値：成功した場合はtrue、エラーの場合はfalseを戻す
    bool initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                    const float scale, const dashboardNS::DialType type, const float zeroAngle,
                    const COLOR_ARGB dialColor, const COLOR_ARGB pointerColor);
	// DialGaugeの指針を設定
	// 引数：percentOn	指針の値
    void set(float percentOn);
	// Update
	// 通常、フレームごとに1回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
    virtual void update(const float frameTime)    {}
	// DialGaugeと指針を描画
	// 引数：color	表示する色（デフォルトでは白）
    virtual void draw(const COLOR_ARGB color = graphicsNS::WHITE); // フィルターとしてcolorを使用して描画
};

// ライトを表すクラスです。
// システムグラフィックスとして使用することができます。
class Light : public Image
{
private:
    float   mFlashTimer;	// ライト点滅用のタイマー
    float   mFlashDelay;	// ライトが点滅する間隔
    COLOR_ARGB  mOnColor;	// 点灯時のライトの色
    COLOR_ARGB  mOffColor;	// 消灯時のライトの色
public:
	// コンストラクタ
    Light();
	// Lightを初期化
	// 引数：*graphics	Graphicsオブジェクトへのポインタ
	// 引数：*textureM	TextureManagerオブジェクトへのポインタ
	// 引数：left、top	画面位置
	// 引数：scale		倍率（ズーム）
	// 引数flashRate	点灯/消灯/点滅の速さ
	//		   （<0の場合は点灯、=0の場合は消灯、>0の場合は1秒あたりの点滅の時間）
	// 引数：colorOn	点灯時のライトの色
	// 引数：colorOff	消灯時のライトの色
	// 戻り値：成功した場合はtrue、エラーの場合はfalseを戻す
    bool initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                const float scale, const float flashRate, const COLOR_ARGB colorOn, const COLOR_ARGB colorOff);
	// flashRateを設定：<0の場合は点灯、 =0の場合は消灯、>0の場合は点滅の周期
	// 引数：rate	flashRate
	void set(const float rate);

	// Update
	// ライトを更新。
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
    virtual void update(const float frameTime);
};

// トグルスイッチを表すクラスです。
// システムグラフィックスとして使用することができます。
class ToggleSwitch : public Image
{
private:
    Input   *mInput;			// 入力システムへのポインタ
    HWND    mHwnd;				// ウィンドウハンドル
    RECT    mSwitchRect;        // マウスクリックを受け付ける範囲
    bool    mSwitchOn;          // スイッチの状態
    bool    mMouseClick;        // マウスクリックの状態
    public:
    // Toggle switchコンストラクタ
    ToggleSwitch();
	// 切り替えスイッチ（ToggleSwitch）を初期化
	// 引数：*graphics	Graphicsオブジェクトへのポインタ
	// 引数：*textureM	TextureManagerオブジェクトへのポインタ
	// 引数：*in		Inputオブジェクトへのポインタ
	// 引数：hwnd		ウィンドウへのハンドル
	// 引数：left、top	画面位置
	// 引数：scale		倍率（ズーム）
	// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
    bool initialize(Graphics *graphics, TextureManager *textureM, Input *in, const HWND hwnd,
                    const int left, const int top, const float scale);
	// Update
	// スイッチ上でのマウスクリックをチェック
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
    virtual void update(const float frameTime);
    // スイッチの状態を取得
	// 戻り値：スイッチがオンかどうか
    bool getSwitchOn()   {return mSwitchOn;}
    // スイッチの状態を設定
	// 引数：on	スイッチがオンかどうか
    void setSwitch(const bool on) {mSwitchOn = on;}
};

// プッシュボタンを表すクラスです。
// システムグラフィックスとして使用することができます。
class PushButton : public Image
{
private:
	Input   *mInput;			// 入力システムへのポインタ
	HWND    mHwnd;				// ウィンドウハンドル
	RECT    mSwitchRect;        // マウスクリックを受け付ける範囲
	bool    mSwitchOn;          // スイッチの状態
	bool    mMouseClick;        // マウスクリックの状態
    bool    mMomentary;         // 一定時間ごとに切り替える場合はtrue、トグルの場合はfalse
public:
    // Pushbutton switchコンストラクタ
    PushButton();
	// プッシュボタン（PushButton）を初期化
	// 引数：*graphics	Graphicsオブジェクトへのポインタ
	// 引数：*textureM	TextureManagerオブジェクトへのポインタ
	// 引数：*in		Inputオブジェクトへのポインタ
	// 引数：hwnd		ウィンドウへのハンドル
	// 引数：left、top	画面位置
	// 引数：scale		倍率（ズーム）
	// 引数：type		trueの場合はモーメンタリー、falseの場合はオルタネート
	// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
    bool initialize(Graphics *graphics, TextureManager *textureM, Input *in, const HWND hwnd,
                    const int left, const int top, const float scale, const bool momentary);
	// Update
	// プッシュボタン上でのマウスクリックをチェック
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
    virtual void update(const float frameTime);
	// スイッチの状態を取得
	// 戻り値：スイッチがオンかどうか
    bool getSwitchOn()  {return mSwitchOn;}
	// スイッチの状態を設定
	// 引数：on	スイッチがオンかどうか
    void setSwitch(const bool on) {mSwitchOn = on;}
};


// 棒グラフを表すクラスです。
// システムグラフィックスとして使用することができます。
class BarGraph : public Image
{
private:
    int mMaxBars;			// バーの本数の最大数
    int mBarsOn;			// 現在のバーの本数
public:
    // BarGraphコンストラクタ
    BarGraph();
	// BarGraphを初期化
	// 引数：*graphics	Graphicsオブジェクトへのポインタ
	// 引数：*textureM	TextureManagerオブジェクトへのポインタ
	// 引数：left、top	画面位置
	// 引数：scale		倍率（ズーム）
	// 引数：bars		メーター内のバーの本数
	// 引数：color		バーの色
	// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
    bool initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                    const float scale, const UINT bars, const COLOR_ARGB color);
	// barsOnの値を、表示するバーの本数に設定
	// パラメータpは割合（0～100）
    void set(float percentOn);
	// Update
	// 通常、フレームごとに1回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
    virtual void update(const float frameTime)    {}
	// BarGraphを描画
	// barsOnは、表示するバーの本数を格納
	// 引数：color	表示する色（デフォルトでは白）
    virtual void draw(const COLOR_ARGB color = graphicsNS::WHITE); // フィルターとしてcolorを使用して描画
};

#endif

