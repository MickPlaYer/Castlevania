/*
 * mygame.h: ���ɮ��x�C��������class��interface
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

enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
	AUDIO_VampireKiller,	// 0
	AUDIO_Boss,				// 1
	AUDIO_StageClear,		// 2
	AUDIO_GameOver			// 3
};

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
//����Number��Class
//�D�n�Ω�ˮ`�Ʀr�����
/////////////////////////////////////////////////////////////////////////////
class CNumber 
{
public:
	CNumber();						// �غc��
	void LoadBitmap(int color=0);	// ���J0..9�έt�����ϧ�(�̾ڰѼƧ����C��)
	void SetNumber(int);			// �]�w��ƭ�
	void SetTopLeft(int,int);		// �N�ʵe�����W���y�в��� (x,y)
	void ShowBitmap();				// �N�ʵe�K��ù�
private:
	int numdigit;					// �@���NUMDIGITS�Ӧ��
	CMovingBitmap digit[11];		// �x�s0..9�έt�����ϧ�(bitmap)
	int x, y;						// ��ܪ��y��
	int n;							// ��ƭ�
	bool isBmpLoaded;				// �O�_�w�g���J�ϧ�
};

/////////////////////////////////////////////////////////////////////////////
//����MenuNumber��Class
//�D�n�Ω�GameMenu����ܼƦr
/////////////////////////////////////////////////////////////////////////////
class CMenuNumber
{
public:
	CMenuNumber();					// �غc��
	void LoadBitmap();				// ���J0..9�έt�����ϧ�
	void SetNumber(int);			// �]�w��ƭ�
	void SetTopLeft(int,int);		// �N�ʵe�����W���y�в��� (x,y)
	void ShowBitmap();				// �N�ʵe�K��ù�
private:
	int numdigit;					// �@���NUMDIGITS�Ӧ��
	CMovingBitmap digit[11];		// �x�s0..9�έt�����ϧ�(bitmap)
	int x, y;						// ��ܪ��y��
	int n;							// ��ƭ�
	bool isBmpLoaded;				// �O�_�w�g���J�ϧ�
};

/////////////////////////////////////////////////////////////////////////////
//����FrameBody��Class
//�bFrame�̩w�q�@�ӯx�νd��
/////////////////////////////////////////////////////////////////////////////
class CFrameBody
{
public:
	CFrameBody();					// �غc��
	CFrameBody(int,int,int,int);	// �غc��:�Ѽ�(x,y,width,height)
	int X();						// �^��X��
	int Y();						// �^��Y��
	int Width();					// �^��Width��
	int Height();					// �^��Height��
private:
	int		x;
	int		y;
	int		width;
	int		height;
};

/////////////////////////////////////////////////////////////////////////////
//����FrameTrigger��Class
//�bFrame�̩w�q�@�ӯx�νd��ña��GOTO�ȡA�i�Ω���ܨ�L�e��
/////////////////////////////////////////////////////////////////////////////
class CFrameTrigger : public CFrameBody
{
public:
	CFrameTrigger();					// �غc��
	CFrameTrigger(int,int,int,int,int);	// �غc��:�Ѽ�(x,y,width,height,goto)
	int Goto();							// �^��Height��
private:
	int		_goto;
};

/////////////////////////////////////////////////////////////////////////////
//����FrameBallPoint��Class
//�bFrame�̩w�q�@���I�ña��ID�ȡA�i�Ω󲣥�Ball����
/////////////////////////////////////////////////////////////////////////////
class CFrameBallPoint
{
public:
	CFrameBallPoint();				// �غc��
	CFrameBallPoint(int,int,int);	// �غc��:�Ѽ�(x,y,id)
	int X();						// �^��X��
	int Y();						// �^��Y��
	int ID();						// �^��ID��
private:
	int		x;
	int		y;
	int		id;
};

/////////////////////////////////////////////////////////////////////////////
//����Frame��Class
/////////////////////////////////////////////////////////////////////////////
class CFrame
{
public:
	CFrame();								// �غc��
	void SetFrame(int,int,int,int,int,int);	// �غc��:�Ѽ�(kind,pic,wait,next,dvx,dvy)
	int Kind();								// �^��Kind��
	int Pic();								// �^��Pic��
	int Wait();								// �^��Wait��
	int Next();								// �^��Next��
	int Dvx();								// �^��Dvx��
	int Dvy();								// �^��Dvy��
	void AddBody(int,int,int,int);				// �bFrame�̥[�J�@��Body
	list<CFrameBody>				bdy;		// list of CFrameBody
	list<CFrameBody>::iterator		bdy_iter;	// list iterator
	void AddAttack(int,int,int,int);			// �bFrame�̥[�J�@��Attack
	list<CFrameBody>				atk;		// list of CFrameBody
	list<CFrameBody>::iterator		atk_iter;	// list iterator
	void AddTrigger(int,int,int,int,int);		// �bFrame�̥[�J�@��Trigger
	list<CFrameTrigger>				trig;		// list of CFrameTrigger
	list<CFrameTrigger>::iterator	trig_iter;	// list iterator
	void AddBodyTrigger(int,int,int,int,int);	// �bFrame�̥[�J�@��BodyTrigger
	list<CFrameTrigger>				bdt;		// list of CFrameTrigger
	list<CFrameTrigger>::iterator	bdt_iter;	// list iterator
	void AddBallPoint(int,int,int);				// �bFrame�̥[�JBallPoint
	list<CFrameBallPoint>			bpoint;		// list of CFrameBallPoint
	list<CFrameBallPoint>::iterator	bpoint_iter;// list iterator
	//
	// ���e�椤��Timer
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
//����GameMap��Class
/////////////////////////////////////////////////////////////////////////////
class CHeroSoma;
class CMonster;
class CBlocker;
class CBall;
class CGameMap
{
public:
	CGameMap();								//�غc��
	void Initialize();						//�]�w�a�Ϭ���l��
	void LoadBitmap();						//���J�a��
	void OnShow();							//��ܦa��
	void OnMove(CHeroSoma*,CBall*);			//�B�z�ʵe
	char* GetMapName();						//�^�Ǧa�Ϫ��W��
	bool IsEmpty(int,int);					//�^�Ǯy���I�O�_�i�q�L
	bool IsEmptySp(int,int);				//�^�Ǯy���I�O�_�i�q�L
	int GetWidth();							//�^�Ǧa�ϼe��
	int GetHeight();						//�^�Ǧa�ϰ���
	int ScreenX(int);						//�^��x�b�ù��W��X�y��
	int ScreenY(int);						//�^��y�b�ù��W��Y�y��
	void SetScreenX(int);					//���ʿù��b�a�ϤW��X�y��
	void SetScreenY(int);					//���ʿù��b�a�ϤW��X�y��
	void SetMap(char*);						//�q�r����o�~���a�ϸ��
	void AddBlocker();						//�s�W���ת��ܦa��
	~CGameMap();							//�Ѻc��
	//
	// ���ת�
	//
	CBlocker* Blocker;
	int blocker_num;
protected:				
	char mapname[32];						//�a�ϦW��
	CMovingBitmap map_pic[64];				//�p�a��
	int map_type_index[128][128];			//�p�a�������w�q
	int map_pic_index[128][128];			//�p�a�ϹϤ��w�q
	int Width, Height;						//�a�Ϫ��e��
	int cell_w, cell_h;						//�a�Ϫ���� cell_w * cell_h
	CMovingBitmap map_bg[64];				//�a�ϭI��
	int bg_dx, bg_dy;						//�a�ϭI�������W���첾
	int bg_w, bg_h;							//�C�i�a�ϭI�����e��
	int bg_cell_w, bg_cell_h;				//�a�Ϫ���� bg_cell_w * bg_cell_h
	CAnimation background_mid;				//�a�Ϥ���
	int bg_mid_x, bg_mid_y;					//�a�Ϥ������y��
	CAnimation background_far;				//�a�ϻ���
	int sx, sy;								//(sx, sy)���ù�(�����W��)�b�a�ϤW���I�y��
	const int X,Y;							//�j�a�Ϫ����W���y��
	const int MW,MH;						//�C�i�p�a�Ϫ��e��
};

/////////////////////////////////////////////////////////////////////////////
//����CBallBitmaps��Class
//�B�zBall���Ϥ�
/////////////////////////////////////////////////////////////////////////////
class CBallBitmap
{
public:
	CBallBitmap();									//�غc��
	void LoadBitmap(int,char*,COLORREF=CLR_INVALID);//Ū���Ϥ�
	void ShowBitmap(bool,int,int,int);				//��ܹϤ�
	int GetPicWidth(bool,int);						//���o�ϼe
	int GetPicHeight(bool,int);						//���o�ϰ�
protected:
	CMovingBitmap	pic_right[32];					//���V�k���Ϥ�
	CMovingBitmap	pic_left[32];					//���V�����Ϥ�
};

/////////////////////////////////////////////////////////////////////////////
//����CBallData��Class
//�B�zBall���e����
/////////////////////////////////////////////////////////////////////////////
class CBallData
{
public:
	CBallData();						// �غc��
	void LoadData(char*,CBallBitmap*);	// Ū���y�����
	CFrame* GetFrame(int);				// ���o�S�w��frame
	CBallBitmap* GetBitmaps();			// ���o�Ϥ�
	int GetShiftX();					// ���o�Y�X�첾X
	int GetShiftY();					// ���o�Y�X�첾Y
	int GetAttackPoint();				// ���o�����O
	int GetDeathFrame();				// ���o���`�e��
protected:
	char			Name[16];			// �y���W��
	int				ShiftX, ShiftY;		// �y�Q�Y�X�ɪ��첾
	int				AttackPoint;		// �y�������O
	int				DeathFrame;			// ���`���e��
	CBallBitmap		*BallBitmaps;		// �y���Ϥ�
	CFrame			frame[64];			// �e����
};

/////////////////////////////////////////////////////////////////////////////
//����Ball��Class
/////////////////////////////////////////////////////////////////////////////
class CBall
{
public:
	CBall();							// �غc��
	void OnMove(CGameMap*);				// �B�z�ʧ@
	void OnShow(CGameMap*);				// ��ܩ�e���W
	void SetXY(int,int);				// �]�w�y��XY�y��
	void SetFrame(int);					// �]�w�y����l�e��
	void BeCast(bool,CBallData*);		// �ϲy�Q�g�X
	void Shift();						// �ϲy����l�첾
	void DeleteBall();					// �ϲy����
	void HurtHero(CHeroSoma*);			// �Ǫ��ˮ`�D��
	bool AliveCheck();					// �˴��y�O�_���ʤ�
	int GetAtk();						// ���o�y�������O
	int GetX();							// ���o�y���y��
	int GetY();							// ���o�y���y��
	int GetPicWidth();					// ���o�y����e�ϼe
	int GetPicHeight();					// ���o�y����e�ϰ�
	bool GetFacing();					// ���o�y�����V
	list<CFrameBody> GetFrameAtk();		// ���o�y�������϶�
protected:
	int				x, y;				// �y���y��
	int				vx,	vy;				// �y���t��
	CBallData		*BallData;			// �y�����
	bool			Alive;				// �y�O�_�ͦs
	bool			isFacingRight;		// �y�����V
	int				FrameNum;			// ��e�e��s��
	int				WaitTime;			// �e�浥�ݭp��
	int				Timmer;				// Timmer�p��
};

/////////////////////////////////////////////////////////////////////////////
//����HeroSoma��Class
/////////////////////////////////////////////////////////////////////////////
class CHeroSoma
{
public:
	CHeroSoma();							// �غc��
	void Initialize();						// �]�w���⬰��l��
	void LoadBitmap();						// ���J�ϧ�
	void OnMove(CGameMap*);					// ���ʨ���
	void OnShow(CGameMap*);					// �N����ϧζK��e��
	void WalkRight(CGameMap*);				// �B�z�V�k�樫
	void WalkLeft(CGameMap*);				// �B�z�V���樫
	void JumpUp(CGameMap*);					// �B�z���D�ʧ@
	void DropDown(CGameMap*);				// �B�z���U�ʧ@
	void Attack(CGameMap*);					// �B�z�����ʧ@
	void Injured(int);						// �Ϩ������ˮ`
	void Restore();							// �Ϩ��⺡��q
	void CastBall(CBall*,CBallData*);		// �Ϩ���o�X�y
	void OnKeyDown(UINT);					// �B�z�����}
	void OnKeyUp(UINT);						// �B�z������U
	bool ImpactCheck(CGameMap*,int,int);	// �T�{����O�_����a��
	bool AliveCheck();						// �T�{����O�_�ͦs
	bool AttackCheck();						// �T�{����O�_���b����
	bool CastCheck();						// �T�{����O�_���b�I�k
	void SetX(int);							// �]�w����X�y��
	void SetY(int);							// �]�w����Y�y��
	int GetX();								// ���o����X�y��
	int GetY();								// ���o����Y�y��
	int GetScreenX(CGameMap*);				// ���o����b�ù��W��X�y��
	int GetScreenY(CGameMap*);				// ���o����b�ù��W��Y�y��
	//
	// ���o����d��
	//
	int GetX1();
	int GetX2();
	int GetY1();
	int GetY2();
	//
	int GetHp();							// ���o�ͩR��
	int GetHpMax();							// ���o�̤j�ͩR��
	int GetMp();							// ���o�]�O��
	int GetMpMax();							// ���o�̤j�]�O��
	int GetAtk();							// ���o�����O
	//
	// ���o�����d��
	//
	int GetAttackX1();
	int GetAttackX2();
	int GetAttackY1();
	int GetAttackY2();
protected:
	//
	// ���⪺�U�ذʵe
	//
	CAnimation	anima_stand;			// ���߰ʵe
	CAnimation	anima_stand_left;		// ���߰ʵe(�¥�)
	CAnimation	anima_walk_star;		// �}�l�樫�ʵe
	CAnimation	anima_walk_star_left;	// �}�l�樫�ʵe
	CAnimation	anima_walk_right;		// �V�k�樫�ʵe
	CAnimation	anima_walk_left;		// �V���樫�ʵe
	CAnimation	anima_walk_stop_right;	// �k�氱��ʵe
	CAnimation	anima_walk_stop_left;	// ���氱��ʵe
	CAnimation	anima_jump_right;		// ���¥k���D�ʵe
	CAnimation	anima_jump_left;		// ���¥����D�ʵe
	CAnimation	anima_drop_star_right;	// ���U�}�l�ʵe
	CAnimation	anima_drop_star_left;	// ���U�}�l�ʵe(�¥�)
	CAnimation	anima_drop_right;		// ���¥k���U�ʵe
	CAnimation	anima_drop_left;		// ���¥����U�ʵe
	CAnimation	anima_drop_end_right;	// �ۦa�ʵe
	CAnimation	anima_drop_end_left;	// �ۦa�ʵe(�¥�)
	CAnimation	anima_injured_right;	// ���˰ʵe
	CAnimation	anima_injured_left;		// ���˰ʵe(�¥�)
	CAnimation	anima_attack_right;		// �����ʵe
	CAnimation	anima_attack_left;		// �����ʵe(�¥�)
	CAnimation	anima_cast_right;		// �I�k�ʵe
	CAnimation	anima_cast_left;		// �I�k�ʵe(�¥�)
	//
	// ����򥻰Ѽ�
	//
	int		x, y;					// ���⪺�y��
	int		AttackRange[4];			// ��������d��
	int		HealthPoint;			// ���⪺��q
	int		HealthPointMax;			// ����̤j��q
	int		ManaPoint;				// ���⪺�]�q
	int		ManaPointMax;			// ����̤j�]�q
	int		AttackPoint;			// ���⪺�ˮ`
	int		VeloWalk;				// �樫�t��
	int		VeloJump;				// ���D�t��
	int		VeloUp[32];
	int		VeloDown[32];
	//
	// ���˼Ʀr
	//
	CNumber injured_num;					// ���˼Ʀr
	int		injured_num_time;				// ���˼Ʀr�Ѿl����ܮɶ�
	int		injured_num_x, injured_num_y;	// ���˼Ʀr���y��
	//
	// ��L�ƭ�
	//
	int		jump_i;
	int		invincible_time;
	//
	// ��L��J�Υ��L��
	//
	bool	KeyLeftDown;
	bool	KeyRightDown;
	bool	KeyUpDown;
	bool	KeyDownDown;
	bool	KeySpaceDown;
	bool	KeyZDown;
	bool	KeyXDown;
	//
	// ���⪬�A�Υ��L��
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
//����LifeBar��Class
/////////////////////////////////////////////////////////////////////////////
class CLifeBar
{
public:
	CLifeBar();						// �غc��
	void Initialize();				// �]�w����l��
	void LoadBitmap();				// ���J�ϧ�
	void OnMove(CHeroSoma *);		// ��s���
	void OnShow();					// ��s��e��
	void ChangeHp(int,int);			// �̾ڰѼƤ���ܦ������
	void ChangeMp(int,int);			// �̾ڰѼƤ�����]������
protected:
	CMovingBitmap black_bar[2], yellow_bar, red_bar, blue_bar;
	int red_lenth;
	int yellow_lenth;
	int blue_lenth;
};

/////////////////////////////////////////////////////////////////////////////
//����CMonsterBitmaps��Class
//�B�zMonster���Ϥ�
/////////////////////////////////////////////////////////////////////////////
class CMonsterBitmap
{
public:
	CMonsterBitmap();								//�غc��
	void LoadBitmap(int,char*,COLORREF=CLR_INVALID);//Ū���Ϥ�
	void ShowBitmap(bool,int,int,int);				//��ܹϤ�
	int GetPicWidth(bool,int);						//���o�ϼe
	int GetPicHeight(bool,int);						//���o�ϰ�
protected:
	CMovingBitmap	pic_right[64];					//���V�k���Ϥ�
	CMovingBitmap	pic_left[64];					//���V�����Ϥ�
};

/////////////////////////////////////////////////////////////////////////////
//����CMonsterData��Class
//�B�zMonster���e����
/////////////////////////////////////////////////////////////////////////////
class CMonsterData
{
public:
	CMonsterData();							// �غc��
	void LoadData(char*,CMonsterBitmap*);	// Ū���Ǫ������
	CFrame* GetFrame(int);					// ���o�S�w��frame
	CMonsterBitmap* GetBitmaps();			// ���o�Ϥ�
	int GetHPMax();							// ���o�̤j��q
	int GetATK();							// ���o�����O
	int GetDEF();							// ���o���m�O
	int GetDeathFrame();					// ���o���`�e��
protected:
	char			Name[16];			// �Ǫ����W��
	int				HealthPointMax;		// �Ǫ����̤j��q
	int				AttackPoint;		// �Ǫ��������O
	int				DefencePoint;		// �Ǫ������m�O
	int				DeathFrame;			// ���`���e��
	CMonsterBitmap	*MonsterBitmaps;	// �Ǫ����Ϥ�
	CFrame			frame[64];			// �e����
};

/////////////////////////////////////////////////////////////////////////////
//����Monster��Class
/////////////////////////////////////////////////////////////////////////////
class CMonster
{
public:
	CMonster();							// �غc��
	void LoadBitmap();					// ���J�ϧ�
	void Add(CMonsterData*);			// �[�J�Ǫ�
	void OnMove(CGameMap*,CHeroSoma*);	// ���ʩǪ�
	void OnShow(CGameMap*);				// �N�Ǫ��ϧζK��e��
	void OnShowNumber(CGameMap*);		// �N�Ʀr�K��e��
	void HurtHero(CHeroSoma*);			// �Ǫ��ˮ`�D��
	void Hurted(CHeroSoma*,CBall*);		// �Ǫ�����D���ˮ`
	void DropDown(CGameMap*);			// �Ǫ������U
	void CastBallCheck(list<CBall>*,CBallData*);//�ϩǪ��g�X�y
	void SetXY(int,int);				// �]�w�Ǫ���XY�y��
	void SetFrame(int);					// �]�w�Ǫ�����l�e��
	bool AliveCheck();					// �^�ǩǪ��O�_�ͦs
protected:
	CMonsterData	*Data;				// �Ǫ������
	int				x, y;				// �Ǫ����y��
	int				HealthPoint;		// �Ǫ�����e��q
	bool			Alive;				// �Ǫ��O�_�ͦs
	bool			isFacingRight;		// �Ǫ������V
	int				DropTime;			// �Ǫ������U�ɶ�
	int				BallInvTime;		// ���y���L�Įɶ�
	//
	// �e��
	//
	int		FrameNum;					// ��e���e��s��
	int		WaitTime;					// ���ݰO��
	int		Timmer;						// Timmer�O��
	//
	// ���˼Ʀr
	//
	CNumber		dam_num;				// ���ˮ���ܪ��ˮ`�Ʀr
	int			dam_num_time;			// �ˮ`�Ʀr�Ѿl����ܮɶ�
	int			dam_num_x, dam_num_y;	// �ˮ`�Ʀr���y��
};

/////////////////////////////////////////////////////////////////////////////
//����Blocker��Class
//�a�Ϥ����ת��a���B�~����
/////////////////////////////////////////////////////////////////////////////
class CBlocker
{
public:
	CBlocker();							// �غc��
	void LoadBlocker(char*);			// ���J���ת������
	void OnShow(CGameMap *);			// �N���ת��ϧζK��e��
	void SetTopLeft(int,int);			// �]�w���ת���XY�y��
	int GetX1();						// ���o�̤pX�y��
	int GetX2();						// ���o�̤jX�y��
	int GetY1();						// ���o�̤pY�y��
	int GetY2();						// ���o�̤jY�y��
private:
	CMovingBitmap	pic_blocker;		// ���ת����Ϥ�
	int				x, y;				// ���ת����y��
	//
	// �e��
	//
	int		FrameNum;
	int		WaitTime;
	int		Timmer;
	int		DeathFrame;
	CFrame  frame[64];
};

/////////////////////////////////////////////////////////////////////////////
//����GameMenu��Class
/////////////////////////////////////////////////////////////////////////////
class CGameMenu
{
public:
	CGameMenu();				// �غc��
	void Initialize();			// �]�w���w�]��
	void LoadBitmap();			// Ū���Ϥ�
	void OnShow();				// ��ܹϤ�
	int OnKeyDown(UINT);		// �B�z����
	void OpenState(CHeroSoma*);	// ���}State����
	void OpenHelp();			// ���}Help����
	void OpenEquip(int);		// ���}Equip����
protected:
	//
	// Menu���Ϥ�
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
	int				menu_page;		//�ثe������
	int				btn_state;		//�ثe���s���s��
	int				btn_state_equip;//�ثe���s���s���bEquip������
	int				equip_ball_type;//�ثe�˳ƪ��k�N�s��
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
	void OnInit();  								// �C������Ȥιϧγ]�w
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
	void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
protected:
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:
	CMovingBitmap logo;								// Title��logo
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
public:
	CGameStateRun(CGame *g);
	~CGameStateRun();
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnInit();  								// �C������Ȥιϧγ]�w
	void OnKeyDown(UINT, UINT, UINT);				// �B�z���䪺�ʧ@
	void OnKeyUp(UINT, UINT, UINT);					// �B�z���䪺�ʧ@
	void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
	void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@ 
	void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
	void LoadBallBitmap();							// Ū���y���Ϥ�
	void LoadBallData();							// Ū���y�����
	void LoadMonsterData();							// Ū���Ǫ������
	void AddMonster(char*);							// ��m�Ǫ��ܦa�ϤW
	void MapChange(int,int);						// �B�z�a�ϲ���
protected:
	void OnMove();									// ���ʹC������
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:
	//
	// Map
	//
	CGameMap		gamemap[16];				// �j�a��
	int				MapAmount;					// �a�Ϫ��ƶq
	int				MapLeft[16], MapRight[16];	// ��F�a�Ϫ��s��
	int				map_index;					// ��e�a�Ϫ��s��
	//
	// Hero
	//
	CHeroSoma       soma;						// �D��:�a�u
	//
	// Ball
	//
	CBall					heroball;			// �^���I�k��
	int						heroball_type;		// �^���I�k��������
	CBallBitmap				BallBitmap[64];		// �U�زy���Ϥ�
	CBallData				BallData[64];		// �U�زy�����
	list<CBall>				MonsterBall;		// �Ǫ��Үg�X���y
	//
	// Monster
	//
	CMonster* Monster;
	int *Mx, *My, *Mf;
	int monster_num;
	CMonsterBitmap			MonsterBitmap[64];	// �U�ةǪ����Ϥ�
	CMonsterData			MonsterData[64];	// �U�ةǪ������
	//
	CGameMenu		game_menu;					// �C�������
	bool			menu_switch;				// ��檺�}��
	//
	// ����
	//
	CMovingBitmap   frame_480_320;				// �B�n��ج[
	CLifeBar		lifebar;					// ���
	CAnimation		anime_bonfire;				// 꺤�
	int				boss_door_y;				// ����BossDoor������
	bool			BossDoorLock;				// ����BossDoor���}��
	CMovingBitmap	stageclear;					// �L����
	bool			BossDead;					// Boss�O�_���`
	bool			BossMusic;					// Boss�������ֶ}��
	int				TimeCount;					// �L����GameOver���ɶ�
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
public:
	CGameStateOver(CGame *g);
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnInit();
protected:
	void OnMove();									// ���ʹC������
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:
	int				counter;						// �˼Ƥ��p�ƾ�
	CMovingBitmap	pic_gameover;					// GameOver��
};

}