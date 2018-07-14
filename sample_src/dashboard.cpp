//==========================================================
/// @file
/// @brief    dashboard.hの実装
/// @author   阿部拳之
///
/// @attention  ダッシュボード（棒グラフ、スイッチなど）を表すクラスです。
///				システムグラフィックスとして使用することができます。

//==========================================================

#include "dashboard.h"

//=============================================================================
// Seven Segmentコンストラクタ
//=============================================================================
SevenSegment::SevenSegment()
{
    mDigits = 1;	// 桁数
    mNumber = 0;	// 表示する数値
}

//=============================================================================
// セブンセグメント表示の初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   left、top = 画面位置
//         scale = 倍率（ズーム）
//		   digits = 桁数
//		   color = 数字の色
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool SevenSegment::initialize(Graphics *graphics, TextureManager *textureM,
                   const int left, const int top, const float scale, UINT digs, const COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::SEGMENT_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mColorFilter = color;
        if(digs < 1)
            digs = 1;
        mDigits = digs;

        mDecimal.initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT,
                           dashboardNS::TEXTURE_COLS, textureM);
        mDecimal.setCurrentFrame(dashboardNS::DECIMAL_FRAME);
        mDecimal.setColorFilter(color);
    }
    catch(...)
    {
        return false;
    }
	// OKを戻す
    return true;
}

//=============================================================================
// セブンセグメント表示に表示する数値を設定
//=============================================================================
void SevenSegment::set(const double value)
{
    mNumber = value;
}

//=============================================================================
// セブンセグメントの数字「0」～「9」と「-」を表示
//         A
//       -----
//     F|     |B
//      |  G  |
//       -----
//     E|     |C
//      |  D  |
//       -----  
//=============================================================================
void SevenSegment::drawDigit(const char n, const COLOR_ARGB color)
{
    float lowerY = mSpriteData.y + mSpriteData.height * mSpriteData.scale * 0.75f;
    float saveY = mSpriteData.y;

	// セグメントA
    if(n=='0' || n=='2' || n=='3' || n=='5' || n=='6' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(0);
        Image::draw(color);
    }
	// セグメントB
    if(n=='0' || n=='1' || n=='2' || n=='3' || n=='4' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(90);
        Image::draw(color);
    }
	// セグメントG
    if(n=='-' || n=='2' || n=='3' || n=='4' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(180);
        Image::draw(color);
    }
	// セグメントFの場合
    if(n=='0' || n=='4' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(270);
        Image::draw(color);
    }

    mSpriteData.y = lowerY;  // 数字の下半分用にYを設定
	
	// セグメントEの場合
    if(n=='0' || n=='2' || n=='6' || n=='8')
    {
        setDegrees(270);
        Image::draw(color);
    }
	// セグメントDの場合
    if(n=='0' || n=='2' || n=='3' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(180);
        Image::draw(color);
    }
	// セグメントCの場合
    if(n=='0' || n=='1' || n=='3' || n=='4' || n=='5' || n=='6' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(90);
        Image::draw(color);
    }
    mSpriteData.y = saveY;
}

//=============================================================================
// 小数点を描画
//=============================================================================
void SevenSegment::drawDecimal(const COLOR_ARGB color)
{
    float saveX = mSpriteData.x;
    float saveY = mSpriteData.y;

    setDegrees(0);
    mSpriteData.x -= mSpriteData.width * mSpriteData.scale * 0.25f;
    mSpriteData.y += mSpriteData.height * mSpriteData.scale * 0.80f;

    mDecimal.draw(mSpriteData, color);

    mSpriteData.x = saveX;
    mSpriteData.y = saveY;
}

//=============================================================================
// セブンセグメント表示を描画
// number変数には、表示する浮動小数点数値が格納されている
//=============================================================================
void SevenSegment::draw(const COLOR_ARGB color)
{
    float saveX = mSpriteData.x;
    float saveY = mSpriteData.y;
    char ch;

    if(mDigits == 0)
        return;

	// 数値を文字列に変換
    std::stringstream strstm;
    strstm.precision(mDigits);
    strstm.flags(std::ios_base::fixed);
    strstm << mNumber;           // 数値を文字列に変換
    std::string str = strstm.str();

    UINT digitN = str.length(); // 文字列の桁数を取得
	// 文字列の桁数が、セブンセグメント表示の桁数よりも多い場合
    if (digitN > mDigits)
        digitN = mDigits;

	// 最も左に位置する桁のx位置
    mSpriteData.x += mSpriteData.width * mSpriteData.scale * 1.2f * (mDigits-digitN);

    UINT n=0;
    ch = str.at(n++);           // numberの最初の桁を取得
    while(digitN > 0)           // 表示する桁が残っている間は継続
    {
        if(ch == '.')           // 小数点の場合
            drawDecimal(color);
        else
        {
            drawDigit(ch, color);   // 数字を表示
			// 次の桁の画面上の位置
            mSpriteData.x += mSpriteData.width * mSpriteData.scale * 1.2f;
        }
        if(n < str.length())
            ch = str.at(n++);   // 次の桁を取得
        else
            ch = '0';           // 文字列が「.」で終わっている場合、数字0で埋める
        if(ch != '.')           // 小数点でない場合
            digitN--;           // 桁数を減分
    }
    mSpriteData.x = saveX;
}

//=============================================================================
// Barを初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   left、top = 画面位置
//         scale = 倍率（ズーム）
//		   color = バーの色
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool Bar::initialize(Graphics *graphics, TextureManager *textureM, const int left,
                     const int top, const float scale, COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BAR_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mColorFilter = color;
    }
    catch(...)
    {
        return false;
    }
    // OKを戻す
    return true;
}

//=============================================================================
// バーのサイズを設定
//=============================================================================
void Bar::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    mSpriteData.rect.right = mSpriteData.rect.left + (LONG)(mSpriteData.width*p/100);
}

//=============================================================================
// DialGaugeを初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   left、top = 画面位置
//         scale = 倍率（ズーム）
//		   type = 円盤の種類
//		   zeroAngle = 円盤のゼロの位置
//		   dialColor = 円盤の色
//		   pointColor = 数字の色
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool DialGauge::initialize(Graphics *graphics, TextureManager *textureM, const int left,
                    const int top, const float scale, const dashboardNS::DialType type,
                    const float zeroAngle, const COLOR_ARGB dialColor, const COLOR_ARGB pointerColor)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        mDialType = type;
        switch(type)
        {
        case dashboardNS::DIAL360:
            setCurrentFrame(dashboardNS::DIAL360_FRAME); break;
        case dashboardNS::DIAL270:
            setCurrentFrame(dashboardNS::DIAL270_FRAME); break;
        case dashboardNS::DIAL180:
            setCurrentFrame(dashboardNS::DIAL180_FRAME); break;
        }
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mColorFilter = dialColor;
        setDegrees(zeroAngle);

        mPointer.initialize(graphics, dashboardNS::WIDTH,
                           dashboardNS::HEGIHT, dashboardNS::TEXTURE_COLS,
                           textureM);
        mPointer.setCurrentFrame(dashboardNS::POINTER_FRAME);
        mPointer.setColorFilter(pointerColor);
    }
    catch(...)
    {
        return false;
    }
    // OKを戻す
    return true;
}

//=============================================================================
// DialGaugeの指針を設定
//=============================================================================
void DialGauge::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    switch(mDialType)
    {
    case dashboardNS::DIAL360:
        mPointer.setDegrees(p * 360/100.0f + getDegrees()); break;
    case dashboardNS::DIAL270:
        mPointer.setDegrees(p * 270/100.0f + getDegrees() - 135); break;
    case dashboardNS::DIAL180:
        mPointer.setDegrees(p * 180/100.0f + getDegrees() - 90); break;
    }
}

//=============================================================================
// DialGaugeと指針を描画
//=============================================================================
void DialGauge::draw(const COLOR_ARGB color)
{
    Image::draw(color);
    float dialAngle = mSpriteData.angle;
    mSpriteData.angle = mPointer.getRadians();
    mPointer.draw(mSpriteData, graphicsNS::FILTER);
    mSpriteData.angle = dialAngle;
}

//=============================================================================
// Lightコンストラクタ
//=============================================================================
Light::Light()
{
    mFlashDelay = 0;
    mFlashTimer = 0;
    mOnColor = graphicsNS::RED;
    mOffColor = SETCOLOR_ARGB(255,32,0,0);   // ダークレッド
}

//=============================================================================
// Lightを初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   left、top = 画面位置
//         scale = 倍率（ズーム）
//		   flashRate = 点灯/消灯/点滅の速さ
//		   （<0の場合は点灯、=0の場合は消灯、>0の場合は1秒あたりの点滅の時間）
//		   colorOn = 点灯時のライトの色
//		   colorOff = 消灯時のライトの色
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool Light::initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                const float scale, const float flashRate, const COLOR_ARGB colorOn, const COLOR_ARGB colorOff)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::LIGHT_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mColorFilter = colorOff;
        mOffColor = colorOff;
        mOnColor = colorOn;
        if(flashRate > 0)
            mFlashDelay = flashRate/2.0f;    // デューティー比は50%
        else
            mFlashDelay = flashRate;
    }
    catch(...)
    {
        return false;
    }
	// OKを戻す
    return true;
}

//=============================================================================
// flashRateを設定：<0の場合は点灯、 =0の場合は消灯、>0の場合は点滅の周期
//=============================================================================
void Light::set(const float flashRate)
{
    if(flashRate > 0)
        mFlashDelay = flashRate/2.0f;    // デューティー比は50%
    else
        mFlashDelay = flashRate;
}

//=============================================================================
// ライトを更新
//=============================================================================
void Light::update(const float frameTime)
{
    if(mFlashDelay > 0)  // >0の場合、ライトを点滅させる
    {
        mFlashTimer += frameTime;
        if(mFlashTimer >= mFlashDelay)
        {
            mFlashTimer -= mFlashDelay;
            if(mColorFilter == mOnColor)
                mColorFilter = mOffColor;
            else
                mColorFilter = mOnColor;
        }
    }
}


//=============================================================================
//  ToggleSwitchコンストラクタ
//=============================================================================
ToggleSwitch::ToggleSwitch()
{
    mSwitchOn = false;
    mMouseClick = true;
}

//=============================================================================
// 切り替えスイッチ（ToggleSwitch）を初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   *in = Inputオブジェクトへのポインタ
//         hwnd = ウィンドウへのハンドル
//		   left、top = 画面位置
//		   scale = 倍率（ズーム）
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool ToggleSwitch::initialize(Graphics *graphics, TextureManager *textureM, Input *in, const HWND h,
                   const int left, const int top, const float scale)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::SWITCH_OFF_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mHwnd = h;                       // ウィンドウへのハンドル
        mInput = in;                     // 入力オブジェクト
        mSwitchRect.left = left;
        mSwitchRect.top  = top;
        mSwitchRect.right = (long)(left + (dashboardNS::SWITCH_WIDTH * mSpriteData.scale));
        mSwitchRect.bottom = (long)(top + (mSpriteData.height * mSpriteData.scale));
    }
    catch(...)
    {
        return false;
    }
	// OKを戻す
    return true;
}

//=============================================================================
// スイッチ上でのマウスクリックをチェック
//=============================================================================
void ToggleSwitch::update(const float frameTime)
{
    if (!mInitialized || !mVisible)
        return;

    // ウィンドウがサイズ変更されている場合を考慮して、画面の比率を計算
    RECT clientRect;
    GetClientRect(mHwnd, &clientRect);
    float screenRatioX = (float)GAME_WIDTH / clientRect.right;
    float screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;

    if (mInput->getMouseLButton())       // マウスの左ボタンの場合
    {
		// マウスクリックがスイッチの内側で発生した場合
        if (mInput->getMouseX()*screenRatioX >= mSwitchRect.left &&
            mInput->getMouseX()*screenRatioX <= mSwitchRect.right &&
            mInput->getMouseY()*screenRatioY >= mSwitchRect.top &&
            mInput->getMouseY()*screenRatioY <= mSwitchRect.bottom)
        {
            if(mMouseClick)
            {
                mMouseClick = false;
                mSwitchOn = !mSwitchOn;       // スイッチを切り替える
                if(mSwitchOn)
                    setCurrentFrame(dashboardNS::SWITCH_ON_FRAME);
                else
                    setCurrentFrame(dashboardNS::SWITCH_OFF_FRAME);
            }
        }
    } else
        mMouseClick = true;
}

//=============================================================================
// PushButtonコンストラクタ
//=============================================================================
PushButton::PushButton()
{
    mSwitchOn = false;
    mMouseClick = true;
    mMomentary = true;
}

//=============================================================================
// プッシュボタン（PushButton）を初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   *in = Inputオブジェクトへのポインタ
//         hwnd = ウィンドウへのハンドル
//		   left、top = 画面位置
//		   scale = 倍率（ズーム）
//		   type = trueの場合はモーメンタリー、falseの場合はオルタネート
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool PushButton::initialize(Graphics *graphics, TextureManager *textureM, Input *in, const HWND h,
                    const int left, const int top, const float scale, const bool type)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mHwnd = h;                       // ウィンドウへのハンドル
        mInput = in;                     // 入力オブジェクト
        mSwitchRect.left = left;
        mSwitchRect.top  = top;
        mSwitchRect.right = (long)(left + mSpriteData.width * mSpriteData.scale);
        mSwitchRect.bottom = (long)(top + mSpriteData.height * mSpriteData.scale);
        mMomentary = type;
    }
    catch(...)
    {
        return false;
    }
	// OKを戻す
    return true;
}

//=============================================================================
// プッシュボタン上でのマウスクリックをチェック
//=============================================================================
void PushButton::update(const float frameTime)
{
    if (!mInitialized || !mVisible)
        return;

	// ウィンドウがサイズ変更されている場合を考慮して、画面の比率を計算
    RECT clientRect;
    GetClientRect(mHwnd, &clientRect);
    float screenRatioX = (float)GAME_WIDTH / clientRect.right;
    float screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;

    if (mInput->getMouseLButton())           // マウスの左ボタンの場合
    {
		// スイッチの内部でマウスがクリックされた場合
        if (mInput->getMouseX()*screenRatioX >= mSwitchRect.left &&
            mInput->getMouseX()*screenRatioX <= mSwitchRect.right &&
            mInput->getMouseY()*screenRatioY >= mSwitchRect.top &&
            mInput->getMouseY()*screenRatioY <= mSwitchRect.bottom)
        {
            if(mMouseClick)
            {
                mMouseClick = false;
                if(mMomentary)               // モーメンタリースイッチの場合
                    mSwitchOn = true;
                else
                    mSwitchOn = !mSwitchOn;   // スイッチを切り替える
                if(mSwitchOn)
                    setCurrentFrame(dashboardNS::BUTTON_DOWN_FRAME);
                else
                    setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
            }
        }
    } 
    else
    {
        mMouseClick = true;
        if(mMomentary)
        {
            mSwitchOn = false;
            setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
        }
    }
}

//=============================================================================
//  BarGraphコンストラクタ
//=============================================================================
BarGraph::BarGraph()
{
    mMaxBars = 1;
    mBarsOn = 0;
}

//=============================================================================
// BarGraphを初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   left、top = 画面位置
//		   scale = 倍率（ズーム）
//		   bars = メーター内のバーの本数
//		   color = バーの色
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool BarGraph::initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                    const float scale, const UINT bars, const COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BAR_GRAPH_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mColorFilter = color;
        if(mMaxBars > 0)
            mMaxBars = bars;
    }
    catch(...)
    {
        return false;
    }
	// OKを戻す
    return true;
}

//=============================================================================
// barsOnの値を、表示するバーの本数に設定
// パラメータpは割合（0～100）
//=============================================================================
void BarGraph::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    mBarsOn = (int)(p * 0.01f * mMaxBars + 0.5);
}

//=============================================================================
// BarGraphを描画
// barsOnは、表示するバーの本数を格納
//=============================================================================
void BarGraph::draw(const COLOR_ARGB color)
{
    float saveX = mSpriteData.x;

    for (int i=mBarsOn; i > 0; i--)
    {
		// 次のバーの画面上の位置
        mSpriteData.x += dashboardNS::BAR_GRAPH_WIDTH * mSpriteData.scale;
        Image::draw(color);
    }
    mSpriteData.x = saveX;
}

