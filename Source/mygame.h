/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
*/

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID {				// 定義各種音效的編號
	AUDIO_VampireKiller,	// 0
	AUDIO_Boss,				// 1
	AUDIO_StageClear,		// 2
	AUDIO_GameOver			// 3
};

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
//此為Number的Class
//主要用於傷害數字的顯示
/////////////////////////////////////////////////////////////////////////////
class CNumber 
{
public:
	CNumber();						// 建構式
	void LoadBitmap(int color=0);	// 載入0..9及負號之圖形(依據參數改變顏色)
	void SetNumber(int);			// 設定整數值
	void SetTopLeft(int,int);		// 將動畫的左上角座標移至 (x,y)
	void ShowBitmap();				// 將動畫貼到螢幕
private:
	int numdigit;					// 共顯示NUMDIGITS個位數
	CMovingBitmap digit[11];		// 儲存0..9及負號之圖形(bitmap)
	int x, y;						// 顯示的座標
	int n;							// 整數值
	bool isBmpLoaded;				// 是否已經載入圖形
};

/////////////////////////////////////////////////////////////////////////////
//此為MenuNumber的Class
//主要用於GameMenu內顯示數字
/////////////////////////////////////////////////////////////////////////////
class CMenuNumber
{
public:
	CMenuNumber();					// 建構式
	void LoadBitmap();				// 載入0..9及負號之圖形
	void SetNumber(int);			// 設定整數值
	void SetTopLeft(int,int);		// 將動畫的左上角座標移至 (x,y)
	void ShowBitmap();				// 將動畫貼到螢幕
private:
	int numdigit;					// 共顯示NUMDIGITS個位數
	CMovingBitmap digit[11];		// 儲存0..9及負號之圖形(bitmap)
	int x, y;						// 顯示的座標
	int n;							// 整數值
	bool isBmpLoaded;				// 是否已經載入圖形
};

/////////////////////////////////////////////////////////////////////////////
//此為FrameBody的Class
//在Frame裡定義一個矩形範圍
/////////////////////////////////////////////////////////////////////////////
class CFrameBody
{
public:
	CFrameBody();					// 建構式
	CFrameBody(int,int,int,int);	// 建構式:參數(x,y,width,height)
	int X();						// 回傳X值
	int Y();						// 回傳Y值
	int Width();					// 回傳Width值
	int Height();					// 回傳Height值
private:
	int		x;
	int		y;
	int		width;
	int		height;
};

/////////////////////////////////////////////////////////////////////////////
//此為FrameTrigger的Class
//在Frame裡定義一個矩形範圍並帶有GOTO值，可用於跳至其他畫格
/////////////////////////////////////////////////////////////////////////////
class CFrameTrigger : public CFrameBody
{
public:
	CFrameTrigger();					// 建構式
	CFrameTrigger(int,int,int,int,int);	// 建構式:參數(x,y,width,height,goto)
	int Goto();							// 回傳Height值
private:
	int		_goto;
};

/////////////////////////////////////////////////////////////////////////////
//此為FrameBallPoint的Class
//在Frame裡定義一個點並帶有ID值，可用於產生Ball物件
/////////////////////////////////////////////////////////////////////////////
class CFrameBallPoint
{
public:
	CFrameBallPoint();				// 建構式
	CFrameBallPoint(int,int,int);	// 建構式:參數(x,y,id)
	int X();						// 回傳X值
	int Y();						// 回傳Y值
	int ID();						// 回傳ID值
private:
	int		x;
	int		y;
	int		id;
};

/////////////////////////////////////////////////////////////////////////////
//此為Frame的Class
/////////////////////////////////////////////////////////////////////////////
class CFrame
{
public:
	CFrame();								// 建構式
	void SetFrame(int,int,int,int,int,int);	// 建構式:參數(kind,pic,wait,next,dvx,dvy)
	int Kind();								// 回傳Kind值
	int Pic();								// 回傳Pic值
	int Wait();								// 回傳Wait值
	int Next();								// 回傳Next值
	int Dvx();								// 回傳Dvx值
	int Dvy();								// 回傳Dvy值
	void AddBody(int,int,int,int);				// 在Frame裡加入一個Body
	list<CFrameBody>				bdy;		// list of CFrameBody
	list<CFrameBody>::iterator		bdy_iter;	// list iterator
	void AddAttack(int,int,int,int);			// 在Frame裡加入一個Attack
	list<CFrameBody>				atk;		// list of CFrameBody
	list<CFrameBody>::iterator		atk_iter;	// list iterator
	void AddTrigger(int,int,int,int,int);		// 在Frame裡加入一個Trigger
	list<CFrameTrigger>				trig;		// list of CFrameTrigger
	list<CFrameTrigger>::iterator	trig_iter;	// list iterator
	void AddBodyTrigger(int,int,int,int,int);	// 在Frame裡加入一個BodyTrigger
	list<CFrameTrigger>				bdt;		// list of CFrameTrigger
	list<CFrameTrigger>::iterator	bdt_iter;	// list iterator
	void AddBallPoint(int,int,int);				// 在Frame裡加入BallPoint
	list<CFrameBallPoint>			bpoint;		// list of CFrameBallPoint
	list<CFrameBallPoint>::iterator	bpoint_iter;// list iterator
	//
	// 此畫格中的Timer
	//
	bool	TimerExist;
	bool	TimerReset;
	int		TimerTriggedTime;
	int		TimerGoTo;
private:
	int		kind;
	int		pic;
	int		wait;
	int		next;
	int		dvx, dvy;
};

/////////////////////////////////////////////////////////////////////////////
//此為GameMap的Class
/////////////////////////////////////////////////////////////////////////////
class CHeroSoma;
class CMonster;
class CBlocker;
class CBall;
class CGameMap
{
public:
	CGameMap();								//建構式
	void Initialize();						//設定地圖為初始值
	void LoadBitmap();						//載入地圖
	void OnShow();							//顯示地圖
	void OnMove(CHeroSoma*,CBall*);			//處理動畫
	char* GetMapName();						//回傳地圖的名稱
	bool IsEmpty(int,int);					//回傳座標點是否可通過
	bool IsEmptySp(int,int);				//回傳座標點是否可通過
	int GetWidth();							//回傳地圖寬度
	int GetHeight();						//回傳地圖高度
	int ScreenX(int);						//回傳x在螢幕上之X座標
	int ScreenY(int);						//回傳y在螢幕上之Y座標
	void SetScreenX(int);					//移動螢幕在地圖上之X座標
	void SetScreenY(int);					//移動螢幕在地圖上之X座標
	void SetMap(char*);						//從字串取得外部地圖資料
	void AddBlocker();						//新增阻擋物至地圖
	~CGameMap();							//解構式
	//
	// 阻擋物
	//
	CBlocker* Blocker;
	int blocker_num;
protected:				
	char mapname[32];						//地圖名稱
	CMovingBitmap map_pic[64];				//小地圖
	int map_type_index[128][128];			//小地圖類型定義
	int map_pic_index[128][128];			//小地圖圖片定義
	int Width, Height;						//地圖的寬高
	int cell_w, cell_h;						//地圖的格數 cell_w * cell_h
	CMovingBitmap map_bg[64];				//地圖背景
	int bg_dx, bg_dy;						//地圖背景的左上角位移
	int bg_w, bg_h;							//每張地圖背景的寬高
	int bg_cell_w, bg_cell_h;				//地圖的格數 bg_cell_w * bg_cell_h
	CAnimation background_mid;				//地圖中景
	int bg_mid_x, bg_mid_y;					//地圖中景的座標
	CAnimation background_far;				//地圖遠景
	int sx, sy;								//(sx, sy)為螢幕(的左上角)在地圖上的點座標
	const int X,Y;							//大地圖的左上角座標
	const int MW,MH;						//每張小地圖的寬高
};

/////////////////////////////////////////////////////////////////////////////
//此為CBallBitmaps的Class
//處理Ball的圖片
/////////////////////////////////////////////////////////////////////////////
class CBallBitmap
{
public:
	CBallBitmap();									//建構式
	void LoadBitmap(int,char*,COLORREF=CLR_INVALID);//讀取圖片
	void ShowBitmap(bool,int,int,int);				//顯示圖片
	int GetPicWidth(bool,int);						//取得圖寬
	int GetPicHeight(bool,int);						//取得圖高
protected:
	CMovingBitmap	pic_right[32];					//面向右的圖片
	CMovingBitmap	pic_left[32];					//面向左的圖片
};

/////////////////////////////////////////////////////////////////////////////
//此為CBallData的Class
//處理Ball的畫格資料
/////////////////////////////////////////////////////////////////////////////
class CBallData
{
public:
	CBallData();						// 建構式
	void LoadData(char*,CBallBitmap*);	// 讀取球的資料
	CFrame* GetFrame(int);				// 取得特定的frame
	CBallBitmap* GetBitmaps();			// 取得圖片
	int GetShiftX();					// 取得擲出位移X
	int GetShiftY();					// 取得擲出位移Y
	int GetAttackPoint();				// 取得攻擊力
	int GetDeathFrame();				// 取得死亡畫格
protected:
	char			Name[16];			// 球的名稱
	int				ShiftX, ShiftY;		// 球被擲出時的位移
	int				AttackPoint;		// 球的攻擊力
	int				DeathFrame;			// 死亡的畫格
	CBallBitmap		*BallBitmaps;		// 球的圖片
	CFrame			frame[64];			// 畫格資料
};

/////////////////////////////////////////////////////////////////////////////
//此為Ball的Class
/////////////////////////////////////////////////////////////////////////////
class CBall
{
public:
	CBall();							// 建構式
	void OnMove(CGameMap*);				// 處理動作
	void OnShow(CGameMap*);				// 顯示於畫面上
	void SetXY(int,int);				// 設定球的XY座標
	void SetFrame(int);					// 設定球的初始畫格
	void BeCast(bool,CBallData*);		// 使球被射出
	void Shift();						// 使球做初始位移
	void DeleteBall();					// 使球消失
	void HurtHero(CHeroSoma*);			// 怪物傷害主角
	bool AliveCheck();					// 檢測球是否活動中
	int GetAtk();						// 取得球的攻擊力
	int GetX();							// 取得球的座標
	int GetY();							// 取得球的座標
	int GetPicWidth();					// 取得球的當前圖寬
	int GetPicHeight();					// 取得球的當前圖高
	bool GetFacing();					// 取得球的面向
	list<CFrameBody> GetFrameAtk();		// 取得球的攻擊區塊
protected:
	int				x, y;				// 球的座標
	int				vx,	vy;				// 球的速度
	CBallData		*BallData;			// 球的資料
	bool			Alive;				// 球是否生存
	bool			isFacingRight;		// 球的面向
	int				FrameNum;			// 當前畫格編號
	int				WaitTime;			// 畫格等待計數
	int				Timmer;				// Timmer計數
};

/////////////////////////////////////////////////////////////////////////////
//此為HeroSoma的Class
/////////////////////////////////////////////////////////////////////////////
class CHeroSoma
{
public:
	CHeroSoma();							// 建構式
	void Initialize();						// 設定角色為初始值
	void LoadBitmap();						// 載入圖形
	void OnMove(CGameMap*);					// 移動角色
	void OnShow(CGameMap*);					// 將角色圖形貼到畫面
	void WalkRight(CGameMap*);				// 處理向右行走
	void WalkLeft(CGameMap*);				// 處理向左行走
	void JumpUp(CGameMap*);					// 處理跳躍動作
	void DropDown(CGameMap*);				// 處理落下動作
	void Attack(CGameMap*);					// 處理攻擊動作
	void Injured(int);						// 使角色受到傷害
	void Restore();							// 使角色滿血量
	void CastBall(CBall*,CBallData*);		// 使角色發出球
	void OnKeyDown(UINT);					// 處理按鍵放開
	void OnKeyUp(UINT);						// 處理按鍵按下
	bool ImpactCheck(CGameMap*,int,int);	// 確認角色是否撞到地形
	bool AliveCheck();						// 確認角色是否生存
	bool AttackCheck();						// 確認角色是否正在攻擊
	bool CastCheck();						// 確認角色是否正在施法
	void SetX(int);							// 設定角色X座標
	void SetY(int);							// 設定角色Y座標
	int GetX();								// 取得角色X座標
	int GetY();								// 取得角色Y座標
	int GetScreenX(CGameMap*);				// 取得角色在螢幕上的X座標
	int GetScreenY(CGameMap*);				// 取得角色在螢幕上的Y座標
	//
	// 取得身體範圍
	//
	int GetX1();
	int GetX2();
	int GetY1();
	int GetY2();
	//
	int GetHp();							// 取得生命值
	int GetHpMax();							// 取得最大生命值
	int GetMp();							// 取得魔力值
	int GetMpMax();							// 取得最大魔力值
	int GetAtk();							// 取得攻擊力
	//
	// 取得攻擊範圍
	//
	int GetAttackX1();
	int GetAttackX2();
	int GetAttackY1();
	int GetAttackY2();
protected:
	//
	// 角色的各種動畫
	//
	CAnimation	anima_stand;			// 站立動畫
	CAnimation	anima_stand_left;		// 站立動畫(朝左)
	CAnimation	anima_walk_star;		// 開始行走動畫
	CAnimation	anima_walk_star_left;	// 開始行走動畫
	CAnimation	anima_walk_right;		// 向右行走動畫
	CAnimation	anima_walk_left;		// 向左行走動畫
	CAnimation	anima_walk_stop_right;	// 右行停止動畫
	CAnimation	anima_walk_stop_left;	// 左行停止動畫
	CAnimation	anima_jump_right;		// 面朝右跳躍動畫
	CAnimation	anima_jump_left;		// 面朝左跳躍動畫
	CAnimation	anima_drop_star_right;	// 落下開始動畫
	CAnimation	anima_drop_star_left;	// 落下開始動畫(朝左)
	CAnimation	anima_drop_right;		// 面朝右落下動畫
	CAnimation	anima_drop_left;		// 面朝左落下動畫
	CAnimation	anima_drop_end_right;	// 著地動畫
	CAnimation	anima_drop_end_left;	// 著地動畫(朝左)
	CAnimation	anima_injured_right;	// 受傷動畫
	CAnimation	anima_injured_left;		// 受傷動畫(朝左)
	CAnimation	anima_attack_right;		// 攻擊動畫
	CAnimation	anima_attack_left;		// 攻擊動畫(朝左)
	CAnimation	anima_cast_right;		// 施法動畫
	CAnimation	anima_cast_left;		// 施法動畫(朝左)
	//
	// 角色基本參數
	//
	int		x, y;					// 角色的座標
	int		AttackRange[4];			// 角色攻擊範圍
	int		HealthPoint;			// 角色的血量
	int		HealthPointMax;			// 角色最大血量
	int		ManaPoint;				// 角色的魔量
	int		ManaPointMax;			// 角色最大魔量
	int		AttackPoint;			// 角色的傷害
	int		VeloWalk;				// 行走速度
	int		VeloJump;				// 跳躍速度
	int		VeloUp[32];
	int		VeloDown[32];
	//
	// 受傷數字
	//
	CNumber injured_num;					// 受傷數字
	int		injured_num_time;				// 受傷數字剩餘的顯示時間
	int		injured_num_x, injured_num_y;	// 受傷數字的座標
	//
	// 其他數值
	//
	int		jump_i;
	int		invincible_time;
	//
	// 鍵盤輸入用布林值
	//
	bool	KeyLeftDown;
	bool	KeyRightDown;
	bool	KeyUpDown;
	bool	KeyDownDown;
	bool	KeySpaceDown;
	bool	KeyZDown;
	bool	KeyXDown;
	//
	// 角色狀態用布林值
	//
	bool	isFacingRight;
	bool	Jumping;
	bool	Droping;
	bool	DropEnd;
	bool	WalkStarEnd;
	bool	WalkEnd;
	bool	Attacking;
	bool	Casting;
	bool	Invincible;
};

/////////////////////////////////////////////////////////////////////////////
//此為LifeBar的Class
/////////////////////////////////////////////////////////////////////////////
class CLifeBar
{
public:
	CLifeBar();						// 建構式
	void Initialize();				// 設定為初始值
	void LoadBitmap();				// 載入圖形
	void OnMove(CHeroSoma *);		// 更新血條
	void OnShow();					// 更新到畫面
	void ChangeHp(int,int);			// 依據參數比改變血條長度
	void ChangeMp(int,int);			// 依據參數比改變魔條長度
protected:
	CMovingBitmap black_bar[2], yellow_bar, red_bar, blue_bar;
	int red_lenth;
	int yellow_lenth;
	int blue_lenth;
};

/////////////////////////////////////////////////////////////////////////////
//此為CMonsterBitmaps的Class
//處理Monster的圖片
/////////////////////////////////////////////////////////////////////////////
class CMonsterBitmap
{
public:
	CMonsterBitmap();								//建構式
	void LoadBitmap(int,char*,COLORREF=CLR_INVALID);//讀取圖片
	void ShowBitmap(bool,int,int,int);				//顯示圖片
	int GetPicWidth(bool,int);						//取得圖寬
	int GetPicHeight(bool,int);						//取得圖高
protected:
	CMovingBitmap	pic_right[64];					//面向右的圖片
	CMovingBitmap	pic_left[64];					//面向左的圖片
};

/////////////////////////////////////////////////////////////////////////////
//此為CMonsterData的Class
//處理Monster的畫格資料
/////////////////////////////////////////////////////////////////////////////
class CMonsterData
{
public:
	CMonsterData();							// 建構式
	void LoadData(char*,CMonsterBitmap*);	// 讀取怪物的資料
	CFrame* GetFrame(int);					// 取得特定的frame
	CMonsterBitmap* GetBitmaps();			// 取得圖片
	int GetHPMax();							// 取得最大血量
	int GetATK();							// 取得攻擊力
	int GetDEF();							// 取得防禦力
	int GetDeathFrame();					// 取得死亡畫格
protected:
	char			Name[16];			// 怪物的名稱
	int				HealthPointMax;		// 怪物的最大血量
	int				AttackPoint;		// 怪物的攻擊力
	int				DefencePoint;		// 怪物的防禦力
	int				DeathFrame;			// 死亡的畫格
	CMonsterBitmap	*MonsterBitmaps;	// 怪物的圖片
	CFrame			frame[64];			// 畫格資料
};

/////////////////////////////////////////////////////////////////////////////
//此為Monster的Class
/////////////////////////////////////////////////////////////////////////////
class CMonster
{
public:
	CMonster();							// 建構式
	void LoadBitmap();					// 載入圖形
	void Add(CMonsterData*);			// 加入怪物
	void OnMove(CGameMap*,CHeroSoma*);	// 移動怪物
	void OnShow(CGameMap*);				// 將怪物圖形貼到畫面
	void OnShowNumber(CGameMap*);		// 將數字貼到畫面
	void HurtHero(CHeroSoma*);			// 怪物傷害主角
	void Hurted(CHeroSoma*,CBall*);		// 怪物受到主角傷害
	void DropDown(CGameMap*);			// 怪物的落下
	void CastBallCheck(list<CBall>*,CBallData*);//使怪物射出球
	void SetXY(int,int);				// 設定怪物的XY座標
	void SetFrame(int);					// 設定怪物的初始畫格
	bool AliveCheck();					// 回傳怪物是否生存
protected:
	CMonsterData	*Data;				// 怪物的資料
	int				x, y;				// 怪物的座標
	int				HealthPoint;		// 怪物的當前血量
	bool			Alive;				// 怪物是否生存
	bool			isFacingRight;		// 怪物的面向
	int				DropTime;			// 怪物的落下時間
	int				BallInvTime;		// 對於球的無敵時間
	//
	// 畫格
	//
	int		FrameNum;					// 當前的畫格編號
	int		WaitTime;					// 等待記數
	int		Timmer;						// Timmer記數
	//
	// 受傷數字
	//
	CNumber		dam_num;				// 受傷時顯示的傷害數字
	int			dam_num_time;			// 傷害數字剩餘的顯示時間
	int			dam_num_x, dam_num_y;	// 傷害數字的座標
};

/////////////////////////////////////////////////////////////////////////////
//此為Blocker的Class
//地圖中阻擋玩家的額外物件
/////////////////////////////////////////////////////////////////////////////
class CBlocker
{
public:
	CBlocker();							// 建構式
	void LoadBlocker(char*);			// 載入阻擋物的資料
	void OnShow(CGameMap *);			// 將阻擋物圖形貼到畫面
	void SetTopLeft(int,int);			// 設定阻擋物的XY座標
	int GetX1();						// 取得最小X座標
	int GetX2();						// 取得最大X座標
	int GetY1();						// 取得最小Y座標
	int GetY2();						// 取得最大Y座標
private:
	CMovingBitmap	pic_blocker;		// 阻擋物的圖片
	int				x, y;				// 阻擋物的座標
	//
	// 畫格
	//
	int		FrameNum;
	int		WaitTime;
	int		Timmer;
	int		DeathFrame;
	CFrame  frame[64];
};

/////////////////////////////////////////////////////////////////////////////
//此為GameMenu的Class
/////////////////////////////////////////////////////////////////////////////
class CGameMenu
{
public:
	CGameMenu();				// 建構式
	void Initialize();			// 設定為預設值
	void LoadBitmap();			// 讀取圖片
	void OnShow();				// 顯示圖片
	int OnKeyDown(UINT);		// 處理按鍵
	void OpenState(CHeroSoma*);	// 打開State頁面
	void OpenHelp();			// 打開Help頁面
	void OpenEquip(int);		// 打開Equip頁面
protected:
	//
	// Menu的圖片
	//
	CMovingBitmap	menu_bg;
	CMovingBitmap	button;
	CMovingBitmap	button_light;
	CMovingBitmap	button_text[4];
	CMovingBitmap	text_bar;
	CMovingBitmap	state_bg;
	CMovingBitmap	help_bg;
	CMovingBitmap	equip_bg;
	CMovingBitmap	ball_button;
	CMovingBitmap	ball_button_light;
	CMovingBitmap	ball_button_icon[4];
	CMovingBitmap	equip_icon;
	CMenuNumber		HP,HPMAX,MP,MPMAX;
	//
	int				menu_page;		//目前的頁面
	int				btn_state;		//目前按鈕的編號
	int				btn_state_equip;//目前按鈕的編號在Equip頁面中
	int				equip_ball_type;//目前裝備的法術編號
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
	void OnInit();  								// 遊戲的初值及圖形設定
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
	void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
protected:
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	CMovingBitmap logo;								// Title的logo
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
public:
	CGameStateRun(CGame *g);
	~CGameStateRun();
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnInit();  								// 遊戲的初值及圖形設定
	void OnKeyDown(UINT, UINT, UINT);				// 處理按鍵的動作
	void OnKeyUp(UINT, UINT, UINT);					// 處理按鍵的動作
	void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
	void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	void LoadBallBitmap();							// 讀取球的圖片
	void LoadBallData();							// 讀取球的資料
	void LoadMonsterData();							// 讀取怪物的資料
	void AddMonster(char*);							// 放置怪物至地圖上
	void MapChange(int,int);						// 處理地圖移動
protected:
	void OnMove();									// 移動遊戲元素
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	//
	// Map
	//
	CGameMap		gamemap[16];				// 大地圖
	int				MapAmount;					// 地圖的數量
	int				MapLeft[16], MapRight[16];	// 緊鄰地圖的編號
	int				map_index;					// 當前地圖的編號
	//
	// Hero
	//
	CHeroSoma       soma;						// 主角:蒼真
	//
	// Ball
	//
	CBall					heroball;			// 英雄施法物
	int						heroball_type;		// 英雄施法物的類型
	CBallBitmap				BallBitmap[64];		// 各種球的圖片
	CBallData				BallData[64];		// 各種球的資料
	list<CBall>				MonsterBall;		// 怪物所射出的球
	//
	// Monster
	//
	CMonster* Monster;
	int *Mx, *My, *Mf;
	int monster_num;
	CMonsterBitmap			MonsterBitmap[64];	// 各種怪物的圖片
	CMonsterData			MonsterData[64];	// 各種怪物的資料
	//
	CGameMenu		game_menu;					// 遊戲中選單
	bool			menu_switch;				// 選單的開關
	//
	// 雜物
	//
	CMovingBitmap   frame_480_320;				// 遮罩邊框架
	CLifeBar		lifebar;					// 血條
	CAnimation		anime_bonfire;				// 篝火
	int				boss_door_y;				// 控制BossDoor的高度
	bool			BossDoorLock;				// 控制BossDoor的開關
	CMovingBitmap	stageclear;					// 過關圖
	bool			BossDead;					// Boss是否死亡
	bool			BossMusic;					// Boss關的音樂開關
	int				TimeCount;					// 過關至GameOver的時間
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
public:
	CGameStateOver(CGame *g);
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnInit();
protected:
	void OnMove();									// 移動遊戲元素
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	int				counter;						// 倒數之計數器
	CMovingBitmap	pic_gameover;					// GameOver圖
};

}