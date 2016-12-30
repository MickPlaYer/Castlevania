/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
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
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <fstream>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CNumber: Number class
/////////////////////////////////////////////////////////////////////////////
CNumber::CNumber()
{
	isBmpLoaded = false;
	numdigit = 5;
}

void CNumber::LoadBitmap(int color)
{
	if (!isBmpLoaded) 
	{
		//
		// 根據color的不同讀入不同顏色的圖片
		//
		switch (color)
		{
		case 0:
			digit[0].LoadBitmap(".\\bitmaps\\number\\dr0.bmp",RGB(0,255,0));
			digit[1].LoadBitmap(".\\bitmaps\\number\\dr1.bmp",RGB(0,255,0));
			digit[2].LoadBitmap(".\\bitmaps\\number\\dr2.bmp",RGB(0,255,0));
			digit[3].LoadBitmap(".\\bitmaps\\number\\dr3.bmp",RGB(0,255,0));
			digit[4].LoadBitmap(".\\bitmaps\\number\\dr4.bmp",RGB(0,255,0));
			digit[5].LoadBitmap(".\\bitmaps\\number\\dr5.bmp",RGB(0,255,0));
			digit[6].LoadBitmap(".\\bitmaps\\number\\dr6.bmp",RGB(0,255,0));
			digit[7].LoadBitmap(".\\bitmaps\\number\\dr7.bmp",RGB(0,255,0));
			digit[8].LoadBitmap(".\\bitmaps\\number\\dr8.bmp",RGB(0,255,0));
			digit[9].LoadBitmap(".\\bitmaps\\number\\dr9.bmp",RGB(0,255,0));
			digit[10].LoadBitmap(".\\bitmaps\\number\\drm.bmp",RGB(0,255,0));
			break;
		case 1:
			digit[0].LoadBitmap(".\\bitmaps\\number\\r0.bmp",RGB(0,255,0));
			digit[1].LoadBitmap(".\\bitmaps\\number\\r1.bmp",RGB(0,255,0));
			digit[2].LoadBitmap(".\\bitmaps\\number\\r2.bmp",RGB(0,255,0));
			digit[3].LoadBitmap(".\\bitmaps\\number\\r3.bmp",RGB(0,255,0));
			digit[4].LoadBitmap(".\\bitmaps\\number\\r4.bmp",RGB(0,255,0));
			digit[5].LoadBitmap(".\\bitmaps\\number\\r5.bmp",RGB(0,255,0));
			digit[6].LoadBitmap(".\\bitmaps\\number\\r6.bmp",RGB(0,255,0));
			digit[7].LoadBitmap(".\\bitmaps\\number\\r7.bmp",RGB(0,255,0));
			digit[8].LoadBitmap(".\\bitmaps\\number\\r8.bmp",RGB(0,255,0));
			digit[9].LoadBitmap(".\\bitmaps\\number\\r9.bmp",RGB(0,255,0));
			digit[10].LoadBitmap(".\\bitmaps\\number\\rm.bmp",RGB(0,255,0));
			break;
		}
		isBmpLoaded = true;
	}
}

void CNumber::SetNumber(int num)
{
	n = num;
	//
	// 同時計算num的位數
	//
	int i;
	if (num<0)
		num = -num;
	for (i=0; num>=1;i++)
	{
		num/=10;
	}
	numdigit = i;
}

void CNumber::SetTopLeft(int nx, int ny)		// 將動畫的左上角座標移至 (x,y)
{
	x = nx; y = ny;
}

void CNumber::ShowBitmap()
{
	GAME_ASSERT(isBmpLoaded, "CNumber: 請先執行LoadBitmap，然後才能ShowBitmap");
	int nx;		// 待顯示位數的 x 座標
	int MSB;	// 最左邊(含符號)的位數的數值
	if (n >= 0) {
		MSB = n;
		nx = x+digit[0].Width()*(numdigit-1);
	} else {
		MSB = -n;
		nx = x+digit[0].Width()*numdigit;
	}
	for (int i=0; i < numdigit; i++) {
		int d = MSB % 10;
		MSB /= 10;
		digit[d].SetTopLeft(nx, y);
		digit[d].ShowBitmap();
		nx -= digit[d].Width()-2;
	}
	if (n < 0) { // 如果小於0，則顯示負號
		digit[10].SetTopLeft(nx, y);
		digit[10].ShowBitmap();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMenuNumber: MenuNumber class
/////////////////////////////////////////////////////////////////////////////
CMenuNumber::CMenuNumber()
{
	isBmpLoaded = false;
}

void CMenuNumber::LoadBitmap()
{
	digit[0].LoadBitmap(".\\bitmaps\\menu\\num_0.bmp",RGB(0,255,0));
	digit[1].LoadBitmap(".\\bitmaps\\menu\\num_1.bmp",RGB(0,255,0));
	digit[2].LoadBitmap(".\\bitmaps\\menu\\num_2.bmp",RGB(0,255,0));
	digit[3].LoadBitmap(".\\bitmaps\\menu\\num_3.bmp",RGB(0,255,0));
	digit[4].LoadBitmap(".\\bitmaps\\menu\\num_4.bmp",RGB(0,255,0));
	digit[5].LoadBitmap(".\\bitmaps\\menu\\num_5.bmp",RGB(0,255,0));
	digit[6].LoadBitmap(".\\bitmaps\\menu\\num_6.bmp",RGB(0,255,0));
	digit[7].LoadBitmap(".\\bitmaps\\menu\\num_7.bmp",RGB(0,255,0));
	digit[8].LoadBitmap(".\\bitmaps\\menu\\num_8.bmp",RGB(0,255,0));
	digit[9].LoadBitmap(".\\bitmaps\\menu\\num_9.bmp",RGB(0,255,0));
	digit[10].LoadBitmap(".\\bitmaps\\menu\\num_m.bmp",RGB(0,255,0));
	isBmpLoaded = true;
}

void CMenuNumber::SetNumber(int num)
{
	n = num;
	//
	// 同時計算num的位數
	//
	int i;
	if (num<0)
		num = -num;
	for (i=0; num>=1;i++)
	{
		num/=10;
	}
	numdigit = i;
}

void CMenuNumber::SetTopLeft(int nx, int ny)		// 將動畫的左上角座標移至 (x,y)
{
	x = nx; y = ny;
}

void CMenuNumber::ShowBitmap()
{
	GAME_ASSERT(isBmpLoaded, "CNumber: 請先執行LoadBitmap，然後才能ShowBitmap");
	int nx;		// 待顯示位數的 x 座標
	int MSB;	// 最左邊(含符號)的位數的數值
	if (n >= 0) {
		MSB = n;
		nx = x+digit[0].Width()*(numdigit-1);
	} else {
		MSB = -n;
		nx = x+digit[0].Width()*numdigit;
	}
	for (int i=0; i < numdigit; i++) {
		int d = MSB % 10;
		MSB /= 10;
		digit[d].SetTopLeft(nx, y);
		digit[d].ShowBitmap();
		nx -= digit[d].Width();
	}
	if (n < 0) { // 如果小於0，則顯示負號
		digit[10].SetTopLeft(nx, y);
		digit[10].ShowBitmap();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGameMap: GameMap class
/////////////////////////////////////////////////////////////////////////////
CGameMap::CGameMap():X(80),Y(80),MW(32),MH(32)
{
	//
	// 初始化地圖陣列
	//
	for(int i=0;i<128;i++)
	{
		for(int j=0;j<128;j++)
		{
			map_pic_index[i][j] = 0;
			map_type_index[i][j] = 0;
		}
	}
	Blocker = NULL;
	blocker_num = 0;
	Initialize();
}

void CGameMap::SetMap(char* MapName)
{
	strcpy(mapname,MapName);

	//
	// 從外部讀入地圖的資料
	//
	
	char MapPath[64] = ".\\Data\\Maps\\";

	strcat(MapPath,mapname);
	strcat(MapPath,".map");

	ifstream input(MapPath,ios::in);

	char str[64];

	input >> str;//<Map>
	input >> str >> cell_w >> cell_h;
	Width=cell_w*32;
	Height=cell_h*32;
	input >> str;
    for(int i=0;i<cell_h;i++)
        for(int j=0;j<cell_w;j++)
            input >> map_pic_index[i][j];
	input >> str;
    for(int i=0;i<cell_h;i++)
        for(int j=0;j<cell_w;j++)
            input >> map_type_index[i][j];
	input >> str;//#MapEnd

	input >> str;//<Background>
	input >> str >> bg_dx >> bg_dy;
	input >> str >> bg_w >> bg_h;
	input >> str >> bg_cell_w >> bg_cell_h;
	input >> str;//#BackgroundEnd
    input.close();

	LoadBitmap();
}

void CGameMap::Initialize()
{
	sx = sy = -80;
}

void CGameMap::LoadBitmap()
{
	char MapPath[64] = ".\\Data\\Maps\\";

	strcat(MapPath,mapname);
	strcat(MapPath,".bpc");
	//
	// 從外部讀入地圖的圖檔
	//
	ifstream input(MapPath,ios::in);

	int R,G,B,DelayCount;
	char tag[64], str[64];

	input >> tag;// 地圖讀入
	input >> tag >> R >> G >> B;
	input >> tag >> str;
    for(int i=1;i<64&&str[0]!='#';i++)
	{
		map_pic[i].LoadBitmap(str,RGB(R,G,B));
		input >> str;
	}

	input >> tag;// 背景讀入
	input >> tag >> str;
	for(int i=0;i<64&&str[0]!='#';i++)
	{
		map_bg[i].LoadBitmap(str);
		input >> str;
	}

	input >> tag;// 中景讀入
	input >> tag >> DelayCount;
	input >> tag >> str;
	background_mid.SetDelayCount(DelayCount);
    while (str[0]!='#')
	{
		background_mid.AddBitmap(str,RGB(R,G,B));
		input >> str;
	}
	//
	input >> tag;// 遠景讀入
	input >> tag >> DelayCount;
	input >> tag >> str;
	background_far.SetDelayCount(DelayCount);
    while (str[0]!='#')
	{
		background_far.AddBitmap(str,RGB(R,G,B));
		input >> str;
	}

	input.close();
}

void CGameMap::OnShow()
{
	// 遠景顯示
	background_far.SetTopLeft(80,80);
	background_far.OnShow();
	// 中景顯示
	background_mid.SetTopLeft(bg_mid_x,bg_mid_y);
	background_mid.OnShow();
	// 背景顯示
	int k = 0;

	for (int i=0;i<bg_cell_h;i++)
	{
		for (int j=0;j<bg_cell_w;j++)
		{
			int x = bg_dx+(bg_w*j)-sx;
			int y = bg_dy+(bg_h*i)-sy;

			map_bg[k].SetTopLeft(x,y);
			map_bg[k].ShowBitmap();
			k++;
		}
	}
	// 地圖顯示
	for (int i=0;i<cell_w;i++)
	{
		for (int j=0;j<cell_h;j++)
		{
			int x = (MW*i)-sx;
			int y = (MH*j)-sy;
			int type = map_pic_index[j][i];

			if (type!=0)
			{
				map_pic[type].SetTopLeft(x,y);
				map_pic[type].ShowBitmap();
			}
		}
	}
	//
	// 阻擋物顯示
	//
	for (int i=0; i<blocker_num; i++)
	{
		Blocker[i].OnShow(this);
	}
}

void CGameMap::OnMove(CHeroSoma *soma,CBall *ball)
{
	//
	// 移動中景
	//
	background_mid.OnMove();
	if (Width == 480)
	{
		bg_mid_x = 80;
	}
	else
	{
		bg_mid_x = -int((double(sx+80)*double(background_mid.Width()-480)/double(Width-480)))+80;
	}
	if (Height == 320)
	{
		bg_mid_y = 80;
	}
	else
	{
		bg_mid_y = -int((double(sy+80)*double(background_mid.Height()-320)/double(Height-320)))+80;
	}
	//
	// 移動遠景
	//
	background_far.OnMove();
}

char* CGameMap::GetMapName()
{
	return mapname;
}

bool CGameMap::IsEmpty(int x,int y)
{
	int mx = x%32;
	int my = y%32;
	int gx = x/32;
	int gy = y/32;
	bool empty = false;

	switch(map_type_index[gy][gx])
	{
		case 0:
			empty = true;
			break;
		case 1:
			empty = false;
			break;
		//
		// 2~5皆為斜面
		//
		case 2:
			if (my-mx<15)
				empty = true;
			break;
		case 3:
			if (mx-my>17)
				empty = true;
			break;
		case 4:
			if (mx+my<46)
				empty = true;
			break;
		case 5:
			if (mx+my<14)
				empty = true;
			break;
		case 6:
			empty = true;
			break;
	}
	//
	// 判斷Blocker
	//
	for (int i=0; i<blocker_num; i++)
	{
		int Bx1 = Blocker[i].GetX1();
		int Bx2 = Blocker[i].GetX2();
		int By1 = Blocker[i].GetY1();
		int By2 = Blocker[i].GetY2();
		if ((Bx1-x)*(Bx2-x)<=0&&(By1-y)*(By2-y)<=0)
		{
			empty = false;
		}
	}
	//
	// 地圖之外的區域
	//
	if (x<0||x>=Width||y<0||y>Height)
	{
		empty = true;
	}

	return empty;
}

bool CGameMap::IsEmptySp(int x,int y)
{
	int my = y%32;
	int gx = x/32;
	int gy = y/32;
	bool empty = IsEmpty(x,y);

	switch(map_type_index[gy][gx])
	{
		case 6:
			if (my==1||my==0)
				empty = false;
			break;
	}
	return empty;
}

int CGameMap::GetWidth()
{
	return Width;
}

int CGameMap::GetHeight()
{
	return Height;
}

int CGameMap::ScreenX(int x)
{
	return x-sx;
}

int CGameMap::ScreenY(int y)
{
	return y-sy;
}

void CGameMap::SetScreenX(int x)
{
	sx=x;
}

void CGameMap::SetScreenY(int y)
{
	sy=y;
}

void CGameMap::AddBlocker()
{
	delete [] Blocker;
	char str[64], tag[64];
	char MapPath[64] = ".\\Data\\Maps\\";
	int x, y, counter = 0;
	//
	// 從外部資料讀入地圖的阻擋物
	//
	strcat(MapPath,mapname);
	strcat(MapPath,".itm");
	ifstream input(MapPath,ios::in);
	//
	// 取得阻擋物數量
	//
	input >> tag >> blocker_num;
	//
	// 依照怪物數量新增CBlocker及其參數
	//
	Blocker = new CBlocker[blocker_num];
	//
	// 設定阻擋物
	//
	while (!input.eof())
	{
		input >> tag >> str;
		if (!strcmp(tag,"<blocker>"))
		{
			input >> tag >> str;
			Blocker[counter].LoadBlocker(str);//設定阻擋物
			input >> tag >> x >> tag >> y >> tag;
			Blocker[counter].SetTopLeft(x,y);//設定阻擋物初始座標
			counter++;
		}
	}
	input.close();
}

CGameMap::~CGameMap()
{
	delete [] Blocker;
}

/////////////////////////////////////////////////////////////////////////////
// CBlocker: Blocker class
/////////////////////////////////////////////////////////////////////////////
CBlocker::CBlocker()
{
	x = y = 0;
}

void CBlocker::LoadBlocker(char* path)
{
	pic_blocker.LoadBitmap(path);
}

void CBlocker::OnShow(CGameMap* InMap)
{
	pic_blocker.SetTopLeft(InMap->ScreenX(x),InMap->ScreenY(y));
	pic_blocker.ShowBitmap();
}

void CBlocker::SetTopLeft(int x,int y)
{
	this->x = x;
	this->y = y;
}

int CBlocker::GetX1()
{
	return x;
}

int CBlocker::GetX2()
{
	return x+pic_blocker.Width();
}

int CBlocker::GetY1()
{
	return y;
}

int CBlocker::GetY2()
{
	return y+pic_blocker.Height();
}

/////////////////////////////////////////////////////////////////////////////
// CBallBitmaps: BallBitmap class
/////////////////////////////////////////////////////////////////////////////
CBallBitmap::CBallBitmap()
{

}

void CBallBitmap::LoadBitmap(int id,char *path,COLORREF color)
{
	//同時讀入雙面向的圖
	pic_right[id].LoadBitmap(path,color);
	path[strlen(path)-4] = '\0';
	strcat(path,"_rev.bmp");
	pic_left[id].LoadBitmap(path,color);
}

void CBallBitmap::ShowBitmap(bool facing,int id,int x,int y)
{
	if (facing)
	{
		pic_right[id].SetTopLeft(x,y);
		pic_right[id].ShowBitmap();
	}
	else
	{
		pic_left[id].SetTopLeft(x,y);
		pic_left[id].ShowBitmap();
	}
}

int CBallBitmap::GetPicWidth(bool facing,int id)
{
	return facing*pic_left[id].Width() + !facing*pic_right[id].Width();
}

int CBallBitmap::GetPicHeight(bool facing,int id)
{
	return facing*pic_left[id].Height() + !facing*pic_right[id].Height();
}

/////////////////////////////////////////////////////////////////////////////
// CBallData: BallData class
/////////////////////////////////////////////////////////////////////////////
CBallData::CBallData()
{
	ShiftX = 0;
	ShiftY = 0;
	AttackPoint = 0;
	DeathFrame = 0;
	BallBitmaps = NULL;
}

void CBallData::LoadData(char *ball_path,CBallBitmap *bitmap_list)
{
	char tag[64], str[64];
	int frame_num = -1;
	int bitmapnum = -1;
	//
	// 從外部資料讀取
	//
	ifstream input(ball_path,ios::in);
	//
	// 讀入屬性
	//
	input
	>> tag//<attribute>
	>> tag >> ShiftX
	>> tag >> ShiftY
	>> tag >> AttackPoint
	>> tag >> DeathFrame
	>> tag >> bitmapnum
	>> tag;//<attribute_end>
	BallBitmaps = &bitmap_list[bitmapnum];
	//
	// 讀入畫格
	//
	while (!input.eof())
	{
		input >> str;
		if (!strcmp(str,"<frame>"))
		{
			//
			// 畫格基本屬性
			//
			int kind, pic, wait, next, dvx, dvy;
			input >> frame_num >> tag;
			input 
			>> tag >> kind
			>> tag >> pic
			>> tag >> wait
			>> tag >> next
			>> tag >> dvx
			>> tag >> dvy
			>> tag;
			frame[frame_num].SetFrame(kind,pic,wait,next,dvx,dvy);
			while (strcmp(tag,"<frame_end>"))
			{
				//
				// Body矩形
				//
				if (!strcmp(tag,"bdy:"))
				{
					int bdx, bdy, bdw, bdh;
					input 
					>> tag >> bdx
					>> tag >> bdy
					>> tag >> bdw
					>> tag >> bdh
					>> tag >> tag;
					frame[frame_num].AddBody(bdx,bdy,bdw,bdh);
				}
				//
				// Attack矩形
				//
				if (!strcmp(tag,"atk:"))
				{
					int atkx, atky, atkw, atkh;
					input 
					>> tag >> atkx
					>> tag >> atky
					>> tag >> atkw
					>> tag >> atkh
					>> tag >> tag;
					frame[frame_num].AddAttack(atkx,atky,atkw,atkh);
				}
				//
				// Trigger矩形
				//
				if (!strcmp(tag,"trig:"))
				{
					int trigx, trigy, trigw, trigh, trig_goto;
					input
					>> tag >> trigx
					>> tag >> trigy
					>> tag >> trigw
					>> tag >> trigh
					>> tag >> trig_goto
					>> tag >> tag;
					frame[frame_num].AddTrigger(trigx,trigy,trigw,trigh,trig_goto);
				}
				//
				// Timer計時器
				//
				if (!strcmp(tag,"timer:"))
				{
					frame[frame_num].TimerExist = true;
					input
					>> tag >> frame[frame_num].TimerReset
					>> tag >> frame[frame_num].TimerTriggedTime
					>> tag >> frame[frame_num].TimerGoTo
					>> tag >> tag;
				}
			}
		}
	}
	input.close();
}

CFrame* CBallData::GetFrame(int frame_num)
{
	return &frame[frame_num];
}

CBallBitmap* CBallData::GetBitmaps()
{
	return BallBitmaps;
}

int CBallData::GetShiftX()
{
	return ShiftX;
}

int CBallData::GetShiftY()
{
	return ShiftY;
}

int CBallData::GetAttackPoint()
{
	return AttackPoint;
}

int CBallData::GetDeathFrame()
{
	return DeathFrame;
}

/////////////////////////////////////////////////////////////////////////////
// CBall: Ball class
/////////////////////////////////////////////////////////////////////////////
CBall::CBall()
{
	Alive = false;
}

void CBall::OnMove(CGameMap *InMap)
{
	if (!Alive)
		return;
	CFrame *frame = BallData->GetFrame(FrameNum);
	//
	// Kind檢測
	//
	switch (frame->Kind())
	{
	//
	// 使怪物死亡
	//
	case -1:
		Alive = false;
		break;
	}
	//
	// 依照dvx,dvy移動Ball
	//
	switch ((frame->Kind()))
	{
	case 2:
		vx += frame->Dvx();
		vy += frame->Dvy();
		break;
	default:
		vx = frame->Dvx();
		vy = frame->Dvy();
	}
	if (vx)
	{
		int temp_x = abs(vx);
		int step_x = 2*(vx/temp_x);
		if (!isFacingRight)
			step_x=step_x*-1;
		for (int i=0; i<temp_x;i++)
		{
			if (frame->Kind() == 1||frame->Kind() == 2)
				x=x+step_x;
			else
				if (InMap->IsEmpty(x-GetPicWidth()/2+2+step_x,y)&&InMap->IsEmpty(x+GetPicWidth()/2+step_x,y))
					x=x+step_x;
				else
				{
					SetFrame(BallData->GetDeathFrame());
					return;
				}
		}
	}
	if (vy)
	{
		int temp_y = abs(vy);
		int step_y = -2*(vy/temp_y);
		for (int i=0; i<temp_y;i++)
		{
			if (frame->Kind() == 1||frame->Kind() == 2)
				y=y+step_y;
			else
				if (InMap->IsEmpty(x-GetPicWidth()/2+2,y+step_y)&&InMap->IsEmpty(x+GetPicWidth()/2,y+step_y))
					y=y+step_y;
				else
				{
					SetFrame(BallData->GetDeathFrame());
					return;
				}
		}
	}
	//
	// Timer檢測
	//
	if (frame->TimerExist)
	{
		//
		// 重設Timer
		//
		if (frame->TimerReset)
		{
			Timmer = 0;
		}
		//
		//  如果到達Timer的觸發時間，跳至TimerGoTo畫格
		//
		else
		{
			if (Timmer<frame->TimerTriggedTime)
			{
				Timmer++;
			}
			else
			{
				SetFrame(frame->TimerGoTo);
				return;
			}
		}
	}
	//
	// 如果到達畫格時間，前往下一個畫格
	//
	if (WaitTime<frame->Wait())
	{
		WaitTime++;
	}
	else
	{
		SetFrame(frame->Next());
	}
	//
	// 超出地圖範圍:自滅
	//
	if ((InMap->ScreenX(x)+512)*(InMap->ScreenX(x)-640-512)>0||InMap->ScreenY(y)*(InMap->ScreenY(y)-480)>0)
		Alive = false;
}

void CBall::OnShow(CGameMap *InMap)
{
	if (!Alive)
		return;
	CFrame *frame = BallData->GetFrame(FrameNum);
	CBallBitmap *bitmap = BallData->GetBitmaps();

	int X = x-bitmap->GetPicWidth(isFacingRight,frame->Pic())/2+1;
	int Y = y-bitmap->GetPicHeight(isFacingRight,frame->Pic())+1;
	bitmap->ShowBitmap(isFacingRight,frame->Pic(),InMap->ScreenX(X),InMap->ScreenY(Y));
}

void CBall::SetXY(int x,int y)
{
	this->x = x;
	this->y = y;
}

void CBall::SetFrame(int frame)
{
	FrameNum = frame;
	WaitTime = 0;
}

void CBall::BeCast(bool facing,CBallData *balldata)
{
	SetFrame(0);
	isFacingRight = facing;
	Alive = true;
	vx = 0;
	vy = 0;
	Timmer = 0;
	WaitTime = 0;
	BallData = balldata;
}

void CBall::Shift()
{
	if (isFacingRight)
		x += BallData->GetShiftX();
	else
		x -= BallData->GetShiftX();
	y -= BallData->GetShiftY();
}

void CBall::DeleteBall()
{
	Alive = false;
}

void CBall::HurtHero(CHeroSoma* soma)
{
	if (Alive)
	{
		CFrame *frame = BallData->GetFrame(FrameNum);
		CBallBitmap *bitmap = BallData->GetBitmaps();
		int PW = bitmap->GetPicWidth(isFacingRight,frame->Pic());
		int PH = bitmap->GetPicHeight(isFacingRight,frame->Pic());
		bool beHit = false;
		int Sx1 = soma->GetX1()
			, Sx2 = soma->GetX2()
			, Sy1 = soma->GetY1()
			, Sy2 = soma->GetY2();
		//
		// 檢測自身所有的Atk區塊
		//
		for (frame->atk_iter=frame->atk.begin(); frame->atk_iter!=frame->atk.end(); frame->atk_iter++)
		{
			int Ax1, Ax2;
			int Ay1 = y-PH+1+frame->atk_iter->Y();
			int Ay2 = Ay1+frame->atk_iter->Height();
			if (isFacingRight)
			{
				Ax1 = x-PW/2+1+frame->atk_iter->X();
				Ax2 = Ax1+frame->atk_iter->Width();
			}
			else
			{
				Ax2 = x+PW/2+1-frame->atk_iter->X();
				Ax1 = Ax2-frame->atk_iter->Width();
			}
			beHit = beHit||(Ax2 >= Sx1 && Ax1 <= Sx2 && Ay2 >= Sy1 && Ay1 <= Sy2);
		}
		//
		// 檢測自身所有的Bdy區塊
		//
		for (frame->bdy_iter=frame->bdy.begin(); frame->bdy_iter!=frame->bdy.end(); frame->bdy_iter++)
		{
			int Bx1, Bx2;
			int By1 = y-PH+1+frame->bdy_iter->Y();
			int By2 = By1+frame->bdy_iter->Height();
			if (isFacingRight)
			{
				Bx1 = x-PW/2+1+frame->bdy_iter->X();
				Bx2 = Bx1+frame->bdy_iter->Width();
			}
			else
			{
				Bx2 = x+PW/2+1-frame->bdy_iter->X();
				Bx1 = Bx2-frame->bdy_iter->Width();
			}
			beHit = beHit||(Bx2 >= Sx1 && Bx1 <= Sx2 && By2 >= Sy1 && By1 <= Sy2);
		}
		if (beHit)
		{
			soma->Injured(GetAtk());
		}
	}
}

bool CBall::AliveCheck()
{
	return Alive;
}

int CBall::GetAtk()
{
	CFrame *frame = BallData->GetFrame(FrameNum);
	//
	// Kind為0時，同時自滅
	//
	if (frame->Kind()==0)
		SetFrame(BallData->GetDeathFrame());
	return BallData->GetAttackPoint();
}

int CBall::GetX()
{
	return x;
}

int CBall::GetY()
{
	return y;
}

int CBall::GetPicWidth()
{
	CFrame *frame = BallData->GetFrame(FrameNum);
	CBallBitmap *bitmap = BallData->GetBitmaps();

	return bitmap->GetPicWidth(isFacingRight,frame->Pic());
}

int CBall::GetPicHeight()
{
	CFrame *frame = BallData->GetFrame(FrameNum);
	CBallBitmap *bitmap = BallData->GetBitmaps();

	return bitmap->GetPicHeight(isFacingRight,frame->Pic());
}

bool CBall::GetFacing()
{
	return isFacingRight;
}

list<CFrameBody> CBall::GetFrameAtk()
{
	CFrame *frame = BallData->GetFrame(FrameNum);

	return frame->atk;
}

/////////////////////////////////////////////////////////////////////////////
// CHeroSoma: HeroSoma class
/////////////////////////////////////////////////////////////////////////////
CHeroSoma::CHeroSoma()
{
	Initialize();
}

void CHeroSoma::Initialize()
{
	x = 167;
	y = 287;
	HealthPoint = 100;
	HealthPointMax = 100;
	ManaPoint = 1000;
	ManaPointMax = 1000;
	AttackPoint = 20;
	invincible_time = 0;
	VeloWalk = 0;
	VeloJump = -99;
	injured_num_time = 0;
	isFacingRight = true;
	KeyDownDown = false;
	KeyLeftDown = false;
	KeyRightDown = false;
	KeyUpDown = false;
	KeyZDown= false;
	Jumping = false;
	Droping = false;
	DropEnd = false;
	WalkStarEnd = false;
	WalkEnd = false;
	Attacking = false;
	Casting = false;
	Invincible = false;
	int velo_init[] = {10,8,7,6,6,4,4,3,2,1,0,0,0,-99};
	for (int i=0;i<29;i++)
	{
		VeloUp[i] = velo_init[i];
	}
	int velo_init2[] = {0,0,1,1,2,3,3,4,4,5,6,6,8,8,8,8,8,8,8,-99};
	for (int i=0;i<29;i++)
	{
		VeloDown[i] = velo_init2[i];
	}
	jump_i= 0;
}

void CHeroSoma::LoadBitmap()
{
	anima_stand.SetDelayCount(6);
	anima_stand.AddBitmap(".\\bitmaps\\soma\\stand_0.bmp",RGB(0,88,24));
	anima_stand.AddBitmap(".\\bitmaps\\soma\\stand_1.bmp",RGB(0,88,24));
	anima_stand.AddBitmap(".\\bitmaps\\soma\\stand_2.bmp",RGB(0,88,24));
	anima_stand.AddBitmap(".\\bitmaps\\soma\\stand_1.bmp",RGB(0,88,24));
	anima_stand.AddBitmap(".\\bitmaps\\soma\\stand_0.bmp",RGB(0,88,24));

	anima_stand_left.SetDelayCount(6);
	anima_stand_left.AddBitmap(".\\bitmaps\\soma\\stand_0_reverse.bmp",RGB(0,88,24));
	anima_stand_left.AddBitmap(".\\bitmaps\\soma\\stand_1_reverse.bmp",RGB(0,88,24));
	anima_stand_left.AddBitmap(".\\bitmaps\\soma\\stand_2_reverse.bmp",RGB(0,88,24));
	anima_stand_left.AddBitmap(".\\bitmaps\\soma\\stand_1_reverse.bmp",RGB(0,88,24));
	anima_stand_left.AddBitmap(".\\bitmaps\\soma\\stand_0_reverse.bmp",RGB(0,88,24));

	anima_walk_star.SetDelayCount(2);
	anima_walk_star.AddBitmap(".\\bitmaps\\soma\\walk_star_0.bmp",RGB(0,88,24));
	anima_walk_star.AddBitmap(".\\bitmaps\\soma\\walk_star_1.bmp",RGB(0,88,24));
	anima_walk_star.AddBitmap(".\\bitmaps\\soma\\walk_star_2.bmp",RGB(0,88,24));
	anima_walk_star.AddBitmap(".\\bitmaps\\soma\\walk_star_2.bmp",RGB(0,88,24));

	anima_walk_star_left.SetDelayCount(2);
	anima_walk_star_left.AddBitmap(".\\bitmaps\\soma\\walk_star_0_reverse.bmp",RGB(0,88,24));
	anima_walk_star_left.AddBitmap(".\\bitmaps\\soma\\walk_star_1_reverse.bmp",RGB(0,88,24));
	anima_walk_star_left.AddBitmap(".\\bitmaps\\soma\\walk_star_2_reverse.bmp",RGB(0,88,24));
	anima_walk_star_left.AddBitmap(".\\bitmaps\\soma\\walk_star_2_reverse.bmp",RGB(0,88,24));

	anima_walk_right.SetDelayCount(2);
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_00.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_01.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_02.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_03.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_04.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_05.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_06.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_07.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_08.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_09.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_10.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_11.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_12.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_13.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_14.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_15.bmp",RGB(0,88,24));
	anima_walk_right.AddBitmap(".\\bitmaps\\soma\\walk_16.bmp",RGB(0,88,24));

	anima_walk_left.SetDelayCount(2);
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_00_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_01_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_02_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_03_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_04_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_05_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_06_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_07_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_08_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_09_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_10_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_11_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_12_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_13_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_14_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_15_reverse.bmp",RGB(0,88,24));
	anima_walk_left.AddBitmap(".\\bitmaps\\soma\\walk_16_reverse.bmp",RGB(0,88,24));

	anima_walk_stop_right.SetDelayCount(3);
	anima_walk_stop_right.AddBitmap(".\\bitmaps\\soma\\walk_stop_0.bmp",RGB(0,88,24));
	anima_walk_stop_right.AddBitmap(".\\bitmaps\\soma\\walk_stop_1.bmp",RGB(0,88,24));
	anima_walk_stop_right.AddBitmap(".\\bitmaps\\soma\\walk_stop_2.bmp",RGB(0,88,24));
	anima_walk_stop_right.AddBitmap(".\\bitmaps\\soma\\walk_stop_3.bmp",RGB(0,88,24));
	anima_walk_stop_right.AddBitmap(".\\bitmaps\\soma\\walk_stop_4.bmp",RGB(0,88,24));
	anima_walk_stop_right.AddBitmap(".\\bitmaps\\soma\\walk_stop_0.bmp",RGB(0,88,24));
	anima_walk_stop_right.AddBitmap(".\\bitmaps\\soma\\walk_stop_0.bmp",RGB(0,88,24));

	anima_walk_stop_left.SetDelayCount(3);
	anima_walk_stop_left.AddBitmap(".\\bitmaps\\soma\\walk_stop_0_reverse.bmp",RGB(0,88,24));
	anima_walk_stop_left.AddBitmap(".\\bitmaps\\soma\\walk_stop_1_reverse.bmp",RGB(0,88,24));
	anima_walk_stop_left.AddBitmap(".\\bitmaps\\soma\\walk_stop_2_reverse.bmp",RGB(0,88,24));
	anima_walk_stop_left.AddBitmap(".\\bitmaps\\soma\\walk_stop_3_reverse.bmp",RGB(0,88,24));
	anima_walk_stop_left.AddBitmap(".\\bitmaps\\soma\\walk_stop_4_reverse.bmp",RGB(0,88,24));
	anima_walk_stop_left.AddBitmap(".\\bitmaps\\soma\\walk_stop_0_reverse.bmp",RGB(0,88,24));
	anima_walk_stop_left.AddBitmap(".\\bitmaps\\soma\\walk_stop_0_reverse.bmp",RGB(0,88,24));

	anima_jump_right.SetDelayCount(3);
	anima_jump_right.AddBitmap(".\\bitmaps\\soma\\jump_0.bmp",RGB(0,88,24));
	anima_jump_right.AddBitmap(".\\bitmaps\\soma\\jump_1.bmp",RGB(0,88,24));

	anima_jump_left.SetDelayCount(3);
	anima_jump_left.AddBitmap(".\\bitmaps\\soma\\jump_0_reverse.bmp",RGB(0,88,24));
	anima_jump_left.AddBitmap(".\\bitmaps\\soma\\jump_1_reverse.bmp",RGB(0,88,24));

	anima_drop_star_right.SetDelayCount(6);
	anima_drop_star_right.AddBitmap(".\\bitmaps\\soma\\jump_2.bmp",RGB(0,88,24));
	anima_drop_star_right.AddBitmap(".\\bitmaps\\soma\\jump_3.bmp",RGB(0,88,24));
	anima_drop_star_right.AddBitmap(".\\bitmaps\\soma\\jump_4.bmp",RGB(0,88,24));

	anima_drop_star_left.SetDelayCount(6);
	anima_drop_star_left.AddBitmap(".\\bitmaps\\soma\\jump_2_reverse.bmp",RGB(0,88,24));
	anima_drop_star_left.AddBitmap(".\\bitmaps\\soma\\jump_3_reverse.bmp",RGB(0,88,24));
	anima_drop_star_left.AddBitmap(".\\bitmaps\\soma\\jump_4_reverse.bmp",RGB(0,88,24));

	anima_drop_right.SetDelayCount(3);
	anima_drop_right.AddBitmap(".\\bitmaps\\soma\\jump_5.bmp",RGB(0,88,24));
	anima_drop_right.AddBitmap(".\\bitmaps\\soma\\jump_6.bmp",RGB(0,88,24));

	anima_drop_left.SetDelayCount(3);
	anima_drop_left.AddBitmap(".\\bitmaps\\soma\\jump_5_reverse.bmp",RGB(0,88,24));
	anima_drop_left.AddBitmap(".\\bitmaps\\soma\\jump_6_reverse.bmp",RGB(0,88,24));

	anima_drop_end_right.SetDelayCount(3);
	anima_drop_end_right.AddBitmap(".\\bitmaps\\soma\\jump_end_0.bmp",RGB(0,88,24));
	anima_drop_end_right.AddBitmap(".\\bitmaps\\soma\\jump_end_1.bmp",RGB(0,88,24));
	anima_drop_end_right.AddBitmap(".\\bitmaps\\soma\\jump_end_2.bmp",RGB(0,88,24));
	anima_drop_end_right.AddBitmap(".\\bitmaps\\soma\\jump_end_3.bmp",RGB(0,88,24));
	anima_drop_end_right.AddBitmap(".\\bitmaps\\soma\\jump_end_4.bmp",RGB(0,88,24));
	anima_drop_end_right.AddBitmap(".\\bitmaps\\soma\\jump_end_5.bmp",RGB(0,88,24));

	anima_drop_end_left.SetDelayCount(3);
	anima_drop_end_left.AddBitmap(".\\bitmaps\\soma\\jump_end_0_reverse.bmp",RGB(0,88,24));
	anima_drop_end_left.AddBitmap(".\\bitmaps\\soma\\jump_end_1_reverse.bmp",RGB(0,88,24));
	anima_drop_end_left.AddBitmap(".\\bitmaps\\soma\\jump_end_2_reverse.bmp",RGB(0,88,24));
	anima_drop_end_left.AddBitmap(".\\bitmaps\\soma\\jump_end_3_reverse.bmp",RGB(0,88,24));
	anima_drop_end_left.AddBitmap(".\\bitmaps\\soma\\jump_end_4_reverse.bmp",RGB(0,88,24));
	anima_drop_end_left.AddBitmap(".\\bitmaps\\soma\\jump_end_5_reverse.bmp",RGB(0,88,24));

	anima_injured_right.SetDelayCount(1);
	anima_injured_right.AddBitmap(".\\bitmaps\\soma\\injured_0.bmp",RGB(0,88,24));
	anima_injured_right.AddBitmap(".\\bitmaps\\soma\\injured_1.bmp",RGB(0,88,24));
	anima_injured_right.AddBitmap(".\\bitmaps\\soma\\injured_2.bmp",RGB(0,88,24));
	anima_injured_right.AddBitmap(".\\bitmaps\\soma\\injured_3.bmp",RGB(0,88,24));

	anima_injured_left.SetDelayCount(1);
	anima_injured_left.AddBitmap(".\\bitmaps\\soma\\injured_0_reverse.bmp",RGB(0,88,24));
	anima_injured_left.AddBitmap(".\\bitmaps\\soma\\injured_1_reverse.bmp",RGB(0,88,24));
	anima_injured_left.AddBitmap(".\\bitmaps\\soma\\injured_2_reverse.bmp",RGB(0,88,24));
	anima_injured_left.AddBitmap(".\\bitmaps\\soma\\injured_3_reverse.bmp",RGB(0,88,24));

	anima_attack_right.SetDelayCount(1);
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_00.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_01.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_02.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_03.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_03.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_03.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_04.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_05.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_06.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_07.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_08.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_09.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_09.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_10.bmp",RGB(0,88,24));
	anima_attack_right.AddBitmap(".\\bitmaps\\soma\\attack_knife_11.bmp",RGB(0,88,24));

	anima_attack_left.SetDelayCount(1);
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_00_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_01_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_02_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_03_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_03_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_03_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_04_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_05_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_06_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_07_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_08_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_09_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_09_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_10_reverse.bmp",RGB(0,88,24));
	anima_attack_left.AddBitmap(".\\bitmaps\\soma\\attack_knife_11_reverse.bmp",RGB(0,88,24));

	anima_cast_right.SetDelayCount(3);
	anima_cast_right.AddBitmap(".\\bitmaps\\soma\\cast_0.bmp",RGB(0,88,24));
	anima_cast_right.AddBitmap(".\\bitmaps\\soma\\cast_1.bmp",RGB(0,88,24));
	anima_cast_right.AddBitmap(".\\bitmaps\\soma\\cast_2.bmp",RGB(0,88,24));
	anima_cast_right.AddBitmap(".\\bitmaps\\soma\\cast_2.bmp",RGB(0,88,24));
	anima_cast_right.AddBitmap(".\\bitmaps\\soma\\cast_2.bmp",RGB(0,88,24));

	anima_cast_left.SetDelayCount(3);
	anima_cast_left.AddBitmap(".\\bitmaps\\soma\\cast_0_reverse.bmp",RGB(0,88,24));
	anima_cast_left.AddBitmap(".\\bitmaps\\soma\\cast_1_reverse.bmp",RGB(0,88,24));
	anima_cast_left.AddBitmap(".\\bitmaps\\soma\\cast_2_reverse.bmp",RGB(0,88,24));
	anima_cast_left.AddBitmap(".\\bitmaps\\soma\\cast_2_reverse.bmp",RGB(0,88,24));
	anima_cast_left.AddBitmap(".\\bitmaps\\soma\\cast_2_reverse.bmp",RGB(0,88,24));

	injured_num.LoadBitmap(1);
}

void CHeroSoma::OnMove(CGameMap *InMap)
{
	int VeloMax = 3;
	//
	// 受傷
	//
	if (invincible_time>15)
	{
		WalkEnd = false;
		anima_attack_right.Reset();
		anima_attack_left.Reset();
		Attacking = false;
		anima_cast_right.Reset();
		anima_cast_left.Reset();
		Casting = false;
		if (invincible_time<=18)
		{
			if (isFacingRight)
			{
				anima_injured_right.OnMove();
			}
			else
			{
				anima_injured_left.OnMove();
			}
		}
	}
	//
	// 攻擊
	//
	else if (Attacking)
	{		
		if (DropEnd)
			DropEnd= false;
		if (WalkEnd)
			WalkEnd = false;
		Attack(InMap);
		if (Droping&&KeyRightDown&&!KeyLeftDown)
			goto DropRight;
		else if (Droping&&KeyLeftDown&&!KeyRightDown)
			goto DropLeft;
		else if (Droping)
			DropDown(InMap);
		else if (Jumping&&KeyRightDown&&!KeyLeftDown)
			goto JumpRight;
		else if (Jumping&&KeyLeftDown&&!KeyRightDown)
			goto JumpLeft;
		else if (Jumping)
			JumpUp(InMap);
	}
	//
	// 施法
	//
	else if (Casting)
	{
		if (DropEnd)
			DropEnd= false;
		if (WalkEnd)
			WalkEnd = false;
		if (isFacingRight)
		{
			anima_cast_right.OnMove();
		}
		else
		{
			anima_cast_left.OnMove();
		}
		if (anima_cast_right.IsFinalBitmap()||anima_cast_left.IsFinalBitmap())
		{
			anima_cast_right.Reset();
			anima_cast_left.Reset();
			Casting = false;
		}
		if (Droping&&KeyRightDown&&!KeyLeftDown)
			goto DropRight;
		else if (Droping&&KeyLeftDown&&!KeyRightDown)
			goto DropLeft;
		else if (Droping)
			DropDown(InMap);
		else if (Jumping&&KeyRightDown&&!KeyLeftDown)
			goto JumpRight;
		else if (Jumping&&KeyLeftDown&&!KeyRightDown)
			goto JumpLeft;
		else if (Jumping)
			JumpUp(InMap);
	}
	//
	// 落下
	//
	else if (Droping&&KeyRightDown&&!KeyLeftDown)
	{
		isFacingRight = true;
		DropRight:
		WalkStarEnd = true;	
		if (VeloWalk<VeloMax)
			VeloWalk+=1;
		for (int i=0; i<VeloWalk; i++)
		{
			if (ImpactCheck(InMap,2,0))
			{
				x+=2;
			}
			if (x>=239&&x<InMap->GetWidth()-240)
				InMap->SetScreenX(x-239-80);
		}
		DropDown(InMap);
	}
	else if (Droping&&KeyLeftDown&&!KeyRightDown)
	{
		isFacingRight = false;
		DropLeft:
		WalkStarEnd = true;
		if (VeloWalk<VeloMax)
			VeloWalk+=1;
		for (int i=0; i<VeloWalk; i++)
		{
			if (ImpactCheck(InMap,-2,0))
			{
				x-=2;
			}
			if (x>=239&&x<InMap->GetWidth()-240)
				InMap->SetScreenX(x-239-80);
		}
		DropDown(InMap);
	}
	else if (Droping)
	{
		DropDown(InMap);
	}
	//
	// 跳躍
	//
	else if (Jumping&&KeyRightDown&&!KeyLeftDown)
	{
		isFacingRight = true;
		JumpRight:
		WalkStarEnd = true;
		if (VeloWalk<VeloMax)
			VeloWalk+=1;
		for (int i=0; i<VeloWalk; i++)
		{
			if (ImpactCheck(InMap,2,0))
			{
				x+=2;
			}
			if (x>=239&&x<InMap->GetWidth()-240)
				InMap->SetScreenX(x-239-80);
		}
		JumpUp(InMap);
	}
	else if (Jumping&&KeyLeftDown&&!KeyRightDown)
	{
		isFacingRight = false;
		JumpLeft:
		WalkStarEnd = true;
		if (VeloWalk<VeloMax)
			VeloWalk+=1;
		for (int i=0; i<VeloWalk; i++)
		{
			if (ImpactCheck(InMap,-2,0))
			{
				x-=2;
			}
			if (x>=239&&x<InMap->GetWidth()-240)
				InMap->SetScreenX(x-239-80);
		}
		JumpUp(InMap);
	}
	else if (Jumping)
	{
		JumpUp(InMap);
	}
	//
	// 行走
	//
	else if (KeyRightDown&&!KeyLeftDown)
	{
		DropEnd = false;
		if (VeloWalk<VeloMax)
			VeloWalk+=1;
		WalkRight(InMap);
	}
	else if (KeyLeftDown&&!KeyRightDown)
	{
		DropEnd = false;
		if (VeloWalk<VeloMax)
			VeloWalk+=1;
		WalkLeft(InMap);
	}
	//
	// 落下結束
	//
	else if (DropEnd)
	{
		WalkEnd = false;
		if (isFacingRight)
		{
			anima_drop_end_right.OnMove();
			if (anima_drop_end_right.IsFinalBitmap())
			{
				DropEnd = false;
			}
		}
		else
		{
			anima_drop_end_left.OnMove();
			if (anima_drop_end_left.IsFinalBitmap())
			{
				DropEnd = false;
			}
		}
	}
	//
	// 行走結束
	//
	else if (WalkEnd)
	{
		VeloWalk = 0;
		if (isFacingRight)
		{
			anima_walk_stop_right.OnMove();
			if (anima_walk_stop_right.IsFinalBitmap())
			{
				WalkEnd = false;
			}
		}
		else
		{
			anima_walk_stop_left.OnMove();
			if (anima_walk_stop_left.IsFinalBitmap())
			{
				WalkEnd = false;
			}
		}
	}
	//
	// 一般站立
	//
	else
	{
		if (isFacingRight)
		{
			anima_stand.OnMove();
		}
		else
		{
			anima_stand_left.OnMove();
		}
	}
	//
	// 處理回魔
	//
	if (ManaPoint<ManaPointMax)
		ManaPoint++;
	//
	// 處理無敵時間
	//
	if (invincible_time>0)
		invincible_time-=1;
	else
		Invincible = false;
	//
	// 處理受傷數字
	//
	if (injured_num_time>0)
	{
		if (injured_num_time%2==1)
		{
			injured_num_y = injured_num_y+1;
		}
		else
		{
			injured_num_y = injured_num_y-3;
		}
		injured_num_time-=1;
	}
}

void CHeroSoma::OnShow(CGameMap *InMap)
{
	int X = x-39;
	int Y = y-79;
	//
	// 受傷
	//
	if (invincible_time>15)
	{
		if (isFacingRight)
		{
			anima_injured_right.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_injured_right.OnShow();
		}
		else
		{
			anima_injured_left.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_injured_left.OnShow();
		}
	}
	//
	// 攻擊
	//
	else if (Attacking)
	{
		int Xa = x-63;
		int Ya = y-79;
		if (isFacingRight)
		{
			anima_attack_right.SetTopLeft(InMap->ScreenX(Xa),InMap->ScreenY(Ya));
			anima_attack_right.OnShow();
		}
		else
		{
			anima_attack_left.SetTopLeft(InMap->ScreenX(Xa),InMap->ScreenY(Ya));
			anima_attack_left.OnShow();
		}
	}
	//
	// 施法
	//
	else if (Casting)
	{
		int Xc = x-63;
		int Yc = y-79;
		if (isFacingRight)
		{
			anima_cast_right.SetTopLeft(InMap->ScreenX(Xc),InMap->ScreenY(Yc));
			anima_cast_right.OnShow();
		}
		else
		{
			anima_cast_left.SetTopLeft(InMap->ScreenX(Xc),InMap->ScreenY(Yc));
			anima_cast_left.OnShow();
		}
	}
	//
	// 落下
	//
	else if (Droping)
	{
		if (isFacingRight)
		{
			if (anima_drop_star_right.IsFinalBitmap())
			{
				anima_drop_right.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
				anima_drop_right.OnShow();
			}
			else
			{
				anima_drop_star_right.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
				anima_drop_star_right.OnShow();
			}
		}
		else
		{			
			if (anima_drop_star_left.IsFinalBitmap())
			{
				anima_drop_left.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
				anima_drop_left.OnShow();
			}
			else
			{
				anima_drop_star_left.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
				anima_drop_star_left.OnShow();
			}
		}
	}
	//
	// 跳躍
	//
	else if (Jumping)
	{
		if (isFacingRight)
		{
			anima_jump_right.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_jump_right.OnShow();
		}
		else
		{			
			anima_jump_left.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_jump_left.OnShow();
		}
	}
	//
	// 右行
	//
	else if (KeyRightDown&&!KeyLeftDown)
	{
		if (WalkStarEnd)
		{
			anima_walk_right.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_walk_right.OnShow();
		}
		else
		{
			anima_walk_star.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_walk_star.OnShow();
			if (anima_walk_star.IsFinalBitmap())
			{
				WalkStarEnd = true;
			}
		}
	}
	//
	// 左行
	//
	else if (KeyLeftDown&&!KeyRightDown)
	{
		if (WalkStarEnd)
		{
			anima_walk_left.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_walk_left.OnShow();
		}
		else
		{
			anima_walk_star_left.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_walk_star_left.OnShow();
			if (anima_walk_star_left.IsFinalBitmap())
			{
				WalkStarEnd = true;
			}
		}
	}
	//
	// 落下結尾
	//
	else if (DropEnd)
	{
		if (isFacingRight)
		{
			anima_drop_end_right.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_drop_end_right.OnShow();
		}
		else
		{
			anima_drop_end_left.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_drop_end_left.OnShow();
		}
	}
	//
	// 行走結尾
	//
	else if (WalkEnd)
	{
		if (isFacingRight)
		{
			anima_walk_stop_right.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_walk_stop_right.OnShow();
		}
		else
		{
			anima_walk_stop_left.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_walk_stop_left.OnShow();
		}
	}
	//
	// 一般站立
	//
	else
	{
		if (isFacingRight)
		{
			anima_stand.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_stand.OnShow();
		}
		else
		{
			anima_stand_left.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
			anima_stand_left.OnShow();
		}
	}
	//
	// 貼上受傷數字
	//
	if (injured_num_time>0)
	{
		X = x+injured_num_x;
		Y = y+injured_num_y;

		injured_num.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
		injured_num.ShowBitmap();
	}
}

void CHeroSoma::WalkRight(CGameMap *InMap)
{
	isFacingRight = true;
	for (int i=0; i<VeloWalk; i++)
	{
		//
		// 處理斜面行走
		//
		if  (ImpactCheck(InMap,2,2))
		{
			//
			// 檢測踩空
			//
			if (ImpactCheck(InMap,2,4))
			{
				Droping = true;
			}
			x+=2;
			y+=2;
		}
		else if (ImpactCheck(InMap,2,0))
		{
			x+=2;
		}
		else if  (ImpactCheck(InMap,2,-2))
		{
			x+=2;
			y-=2;
		}
		//
		// 移動鏡頭
		//
		if (x>=239&&x<InMap->GetWidth()-240)
			InMap->SetScreenX(x-239-80);
		if (y>=223&&y<InMap->GetHeight()-96)
			InMap->SetScreenY(y-223-80);
	}
	if (WalkStarEnd)
	{
		anima_walk_right.OnMove();	
	}
	else
	{
		anima_walk_star.OnMove();
	}

}

void CHeroSoma::WalkLeft(CGameMap *InMap)
{
	isFacingRight = false;
	for (int i=0; i<VeloWalk; i++)
	{
		//
		// 處理斜面行走
		//
		if  (ImpactCheck(InMap,-2,2))
		{
			//
			// 檢測踩空
			//
			if (ImpactCheck(InMap,-2,4))
			{
				Droping = true;
			}
			x-=2;
			y+=2;
		}
		else if (ImpactCheck(InMap,-2,0))
		{
			x-=2;
		}
		else if  (ImpactCheck(InMap,-2,-2))
		{
			x-=2;
			y-=2;
		}
		//
		// 移動鏡頭
		//
		if (x>=239&&x<InMap->GetWidth()-240)
			InMap->SetScreenX(x-239-80);
		if (y>=223&&y<InMap->GetHeight()-96)
			InMap->SetScreenY(y-223-80);
	}
	if (WalkStarEnd)
	{
		anima_walk_left.OnMove();
	}
	else
	{
		anima_walk_star_left.OnMove();
	}
}

void CHeroSoma::JumpUp(CGameMap *InMap)
{
	if (VeloJump ==-99)
	{
		anima_jump_right.Reset();
		anima_jump_left.Reset();
		jump_i = 0;
	}
	VeloJump=VeloUp[jump_i];
	jump_i+=1;
	for (int i=0; i<VeloJump; i++)
	{
		if (ImpactCheck(InMap,0,-2))
		{
			y-=2;
		}
		else
		{
			VeloJump = -99;
		}
		//
		// 移動鏡頭
		//
		if (y>=223&&y<InMap->GetHeight()-96)
			InMap->SetScreenY(y-223-80);
	}
	if (VeloJump == -99)
	{
		anima_drop_star_right.Reset();
		anima_drop_star_left.Reset();
		Jumping = false;
		Droping = true;
	}
	if (isFacingRight)
	{
		anima_jump_right.OnMove();
	}
	else
	{
		anima_jump_left.OnMove();
	}
}

void CHeroSoma::DropDown(CGameMap *InMap)
{
	if (VeloJump ==-99)
	{
		anima_drop_right.Reset();
		anima_drop_left.Reset();
		anima_drop_star_right.Reset();
		anima_drop_star_left.Reset();
		anima_drop_end_right.Reset();
		anima_drop_end_left.Reset();
		jump_i = 0;
	}
	VeloJump=VeloDown[jump_i];
	if (jump_i<13)
		jump_i+=1;
	for (int i=0; i<VeloJump; i++)
	{
		if (ImpactCheck(InMap,0,+2))
		{
			y+=2;
		}
		else
		{
			VeloJump = -99;
		}
		//
		// 移動鏡頭
		//
		if (y>=223&&y<InMap->GetHeight()-96)
			InMap->SetScreenY(y-223-80);
	}
	if (VeloJump == -99)
	{
		Droping = false;
		DropEnd = true;
		if (Attacking)
		{
			anima_attack_right.Reset();
			anima_attack_left.Reset();
			Attacking = false;
		}
	}
	if (isFacingRight)
	{
		if (anima_drop_star_right.IsFinalBitmap())
		{
			anima_drop_right.OnMove();
		}
		else
		{
			anima_drop_star_right.OnMove();
		}
	}
	else
	{
		if (anima_drop_star_left.IsFinalBitmap())
		{
			anima_drop_left.OnMove();
		}
		else
		{
			anima_drop_star_left.OnMove();
		}
	}
}

void CHeroSoma::Attack(CGameMap *InMap)
{
	//
	// 製造攻擊範圍
	//
	if (isFacingRight)
	{
		AttackRange[0]=x+34;
		AttackRange[1]=x+52;
		AttackRange[2]=y-54;
		AttackRange[3]=y-52;
		anima_attack_right.OnMove();
	}
	else
	{
		AttackRange[0]=x-52;
		AttackRange[1]=x-34;
		AttackRange[2]=y-54;
		AttackRange[3]=y-52;
		anima_attack_left.OnMove();
	}
	if (anima_attack_right.IsFinalBitmap()||anima_attack_left.IsFinalBitmap())
	{
		anima_attack_right.Reset();
		anima_attack_left.Reset();
		Attacking = false;
	}
}
void CHeroSoma::Injured(int damage)
{
	if (!Invincible)
	{
		//
		// 計算傷害並扣血
		//
		HealthPoint-=damage;
		//
		// 設定無敵時間
		//
		invincible_time = 30;
		Invincible = true;
		//
		// 設定受傷數字
		//
		injured_num.SetNumber(damage);
		injured_num_time = 30;
		injured_num_x = -4;
		injured_num_y = -58;
		//
		// 重置受傷動畫
		//
		anima_injured_right.Reset();
		anima_injured_left.Reset();
	}
}

void CHeroSoma::Restore()
{
	HealthPoint = HealthPointMax;
	ManaPoint = ManaPointMax;
}

void CHeroSoma::CastBall(CBall* ball,CBallData *balldata)
{
	if (!ball->AliveCheck()&&ManaPoint>=100)
	{
		ManaPoint-=100;
		ball->SetXY(x,y-48);
		ball->BeCast(isFacingRight,balldata);
		ball->Shift();
	}
}

void CHeroSoma::OnKeyDown(UINT nChar)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY_SPACE = 0x20; // keyboard空白鍵
	const char KEY_Z	 = 'Z';
	const char KEY_X	 = 'X';
	if (nChar == KEY_RIGHT)
	{
		KeyRightDown = true;
		WalkEnd = false;
	}
	if (nChar == KEY_LEFT)
	{
		KeyLeftDown = true;
		WalkEnd = false;
	}
	if (nChar == KEY_UP)
		KeyUpDown = true;
	if (nChar == KEY_DOWN)
		KeyDownDown = true;
	if (nChar == KEY_SPACE)
	{
		KeySpaceDown = true;
		if (!Droping&&invincible_time<15&&!Attacking)
			Jumping = true;
	}
	if (nChar == KEY_Z)
	{
		KeyZDown = true;
		if (!Casting)
			Attacking = true;
	}
	if (nChar == KEY_X)
	{
		KeyXDown = true;
		if (!Attacking)
			Casting = true;
	}
}

void CHeroSoma::OnKeyUp(UINT nChar)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY_SPACE = 0x20; // keyboard空白鍵
	const char KEY_Z	 = 'Z';
	const char KEY_X	 = 'X';

	if (nChar == KEY_RIGHT && KeyRightDown)
	{
		WalkEnd = true;
		WalkStarEnd = false;
		KeyRightDown = false;
		anima_walk_star.Reset();
		anima_walk_right.Reset();
		anima_walk_stop_right.Reset();
	}
	if (nChar == KEY_LEFT && KeyLeftDown)
	{
		WalkEnd = true;
		WalkStarEnd = false;
		KeyLeftDown = false;
		anima_walk_star.Reset();
		anima_walk_left.Reset();
		anima_walk_stop_left.Reset();
	}

	if (nChar == KEY_UP)
		KeyUpDown = false;
	if (nChar == KEY_DOWN)
		KeyDownDown = false;
	if (nChar == KEY_SPACE)
		KeySpaceDown = false;
	if (nChar == KEY_SPACE)
	{
		KeySpaceDown = false;
		//
		// 跳躍中放開按鍵強制轉為落下
		//
		if (Jumping)
		{
			VeloJump = -99;
			anima_drop_star_right.Reset();
			anima_drop_star_left.Reset();
			Droping = true;
			Jumping = false;
		}
	}
	if (nChar == KEY_Z)
		KeyZDown = false;
	if (nChar == KEY_X)
		KeyXDown = false;
}

bool CHeroSoma::ImpactCheck(CGameMap *InMap, int dx, int dy)
{
	bool LeftTop, RightTop, LeftMid, RightMid, LeftBtn, RightBtn;

	LeftTop = InMap->IsEmpty(x-14+dx,y-63+dy);
	RightTop = InMap->IsEmpty(x+16+dx,y-63+dy);
	LeftMid = InMap->IsEmpty(x-14+dx,y-32+dy);
	RightMid = InMap->IsEmpty(x+16+dx,y-32+dy);
	if (Jumping)
	{
		LeftBtn = InMap->IsEmpty(x-14+dx,y+dy);
		RightBtn = InMap->IsEmpty(x+16+dx,y+dy);
	}
	else
	{
		LeftBtn = InMap->IsEmptySp(x-14+dx,y+dy);
		RightBtn = InMap->IsEmptySp(x+16+dx,y+dy);
	}

	return(LeftTop&&RightTop&&LeftMid&&RightMid&&LeftBtn&&RightBtn);
}

bool CHeroSoma::AliveCheck()
{
	return HealthPoint>0;
}

bool CHeroSoma::AttackCheck()
{
	if (isFacingRight)
		return (anima_attack_right.GetCurrentBitmapNumber()==5);
	else
		return (anima_attack_left.GetCurrentBitmapNumber()==5);
}

bool CHeroSoma::CastCheck()
{
	if (isFacingRight)
		return (anima_cast_right.GetCurrentBitmapNumber()==3);
	else
		return (anima_cast_left.GetCurrentBitmapNumber()==3);
}

void CHeroSoma::SetX(int x)
{
	this->x = x;
}

void CHeroSoma::SetY(int y)
{
	this->y = y;
}

int CHeroSoma::GetX()
{
	return x;
}

int CHeroSoma::GetY()
{
	return y;
}

int CHeroSoma::GetScreenX(CGameMap *InMap)
{
	return InMap->ScreenX(x);
}

int CHeroSoma::GetScreenY(CGameMap *InMap)
{
	return InMap->ScreenY(y);
}

int CHeroSoma::GetX1()
{
	return x-8;
}

int CHeroSoma::GetX2()
{
	return x+8;
}

int CHeroSoma::GetY1()
{
	return y-64;
}

int CHeroSoma::GetY2()
{
	return y;
}
int CHeroSoma::GetAttackX1()
{
	return AttackRange[0];
}

int CHeroSoma::GetAttackX2()
{
	return AttackRange[1];
}

int CHeroSoma::GetAttackY1()
{
	return AttackRange[2];
}

int CHeroSoma::GetAttackY2()
{
	return AttackRange[3];
}

int CHeroSoma::GetHp()
{
	return HealthPoint;
}

int CHeroSoma::GetHpMax()
{
	return HealthPointMax;
}

int CHeroSoma::GetMp()
{
	return ManaPoint;
}

int CHeroSoma::GetMpMax()
{
	return ManaPointMax;
}

int CHeroSoma::GetAtk()
{
	return AttackPoint;
}

/////////////////////////////////////////////////////////////////////////////
// CLifeBar: LifeBar class
/////////////////////////////////////////////////////////////////////////////
CLifeBar::CLifeBar()
{
	Initialize();
}

void CLifeBar::Initialize()
{
	red_lenth = 300;
	blue_lenth = 300;
	yellow_lenth = 300;
}

void CLifeBar::LoadBitmap()
{
	black_bar[0].LoadBitmap(".\\bitmaps\\black_bar.bmp",RGB(0,88,24));
	black_bar[1].LoadBitmap(".\\bitmaps\\black_bar.bmp",RGB(0,88,24));
	red_bar.LoadBitmap(".\\bitmaps\\red_bar.bmp",RGB(0,88,24));
	yellow_bar.LoadBitmap(".\\bitmaps\\yellow_bar.bmp",RGB(0,88,24));
	blue_bar.LoadBitmap(".\\bitmaps\\blue_bar.bmp",RGB(0,88,24));
}
void CLifeBar::OnMove(CHeroSoma * soma)
{
	ChangeHp(soma->GetHp(),soma->GetHpMax());
	ChangeMp(soma->GetMp(),soma->GetMpMax());
}
void CLifeBar::OnShow()
{
	black_bar[0].SetTopLeft(6,90);
	black_bar[0].ShowBitmap();
	black_bar[1].SetTopLeft(566,90);
	black_bar[1].ShowBitmap();
	yellow_bar.SetTopLeft(6,90+300-yellow_lenth);
	yellow_bar.ShowBitmap();
	red_bar.SetTopLeft(6,90+300-red_lenth);
	red_bar.ShowBitmap();
	blue_bar.SetTopLeft(566,90+300-blue_lenth);
	blue_bar.ShowBitmap();
}

void CLifeBar::ChangeHp(int hp,int maxhp)
{
	int lenth = int(300*double(hp)/double(maxhp));
	//
	// 回血時紅條緩速向上
	//
	for (int i=0; i<4; i++)
		if (red_lenth>=lenth)
		{
			red_lenth = lenth;
		}
		else
		{
			red_lenth +=1;
		}
	//
	// 受傷時黃條緩速向下
	//
	for (int i=0; i<2; i++)
		if (yellow_lenth>=lenth)
		{
			yellow_lenth-=1;
		}
		else
		{
			yellow_lenth +=1;
		}
}

void CLifeBar::ChangeMp(int mp,int maxmp)
{
	int lenth = int(300*double(mp)/double(maxmp));
	//
	// 回魔時藍條緩速向上
	//
	for (int i=0; i<4; i++)
		if (blue_lenth>=lenth)
		{
			blue_lenth = lenth;
		}
		else
		{
			blue_lenth +=1;
		}
}

/////////////////////////////////////////////////////////////////////////////
// CFrameBody: FrameBody class
/////////////////////////////////////////////////////////////////////////////
CFrameBody::CFrameBody()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

CFrameBody::CFrameBody(int x,int y,int width,int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

int CFrameBody::X()
{
	return x;
}
	
int CFrameBody::Y()
{
	return y;
}	

int CFrameBody::Width()
{
	return width;
}

int CFrameBody::Height()
{
	return height;
}

/////////////////////////////////////////////////////////////////////////////
// CFrameTrigger: FrameTrigger class
/////////////////////////////////////////////////////////////////////////////
CFrameTrigger::CFrameTrigger()
{
	_goto = 0;
}

CFrameTrigger::CFrameTrigger(int x,int y,int width,int height,int _goto) : CFrameBody(x,y,width,height)
{
	this->_goto = _goto;
}

int CFrameTrigger::Goto()
{
	return _goto;
}

/////////////////////////////////////////////////////////////////////////////
// CFrameBpoint: FrameBpoint class
/////////////////////////////////////////////////////////////////////////////
CFrameBallPoint::CFrameBallPoint()
{
	x = 0;
	y = 0;
	id = 0;
}

CFrameBallPoint::CFrameBallPoint(int x,int y,int id)
{
	this->x = x;
	this->y = y;
	this->id = id;
}

int CFrameBallPoint::X()
{
	return x;
}

int CFrameBallPoint::Y()
{
	return y;
}

int CFrameBallPoint::ID()
{
	return id;
}

/////////////////////////////////////////////////////////////////////////////
// CFrame: Frame class
/////////////////////////////////////////////////////////////////////////////
CFrame::CFrame()
{
	kind = pic = wait = next = dvx = dvy = 0;
	TimerExist = false;
	TimerReset = false;
	TimerTriggedTime = 0;
	TimerGoTo = 0;
}

void CFrame::SetFrame(int kind,int pic,int wait,int next,int dvx,int dvy)
{
	this->kind = kind;
	this->pic = pic;
	this->wait = wait;
	this->next = next; 
	this->dvx = dvx;
	this->dvy = dvy;
}

int CFrame::Kind()
{
	return kind;
}

int CFrame::Pic()
{
	return pic;
}

int CFrame::Wait()
{
	return wait;
}

int CFrame::Next()
{
	return next;
}

int CFrame::Dvx()
{
	return dvx;
}

int CFrame::Dvy()
{
	return dvy;
}

void CFrame::AddBody(int x,int y,int width,int height)
{
	CFrameBody add_body(x,y,width,height);
	bdy.insert(bdy.end(), add_body);
}

void CFrame::AddAttack(int x,int y,int width,int height)
{
	CFrameBody add_atk(x,y,width,height);
	atk.insert(atk.end(), add_atk);
}

void CFrame::AddTrigger(int x,int y,int width,int height,int _goto)
{
	CFrameTrigger add_trig(x,y,width,height,_goto);
	trig.insert(trig.end(), add_trig);
}

void CFrame::AddBodyTrigger(int x,int y,int width,int height,int _goto)
{
	CFrameTrigger add_bdt(x,y,width,height,_goto);
	bdt.insert(bdt.end(), add_bdt);
}

void CFrame::AddBallPoint(int x,int y,int id)
{
	CFrameBallPoint add_bpoint(x,y,id);
	bpoint.insert(bpoint.end(), add_bpoint);
}

/////////////////////////////////////////////////////////////////////////////
// CMonsterBitmaps: MonsterBitmap class
/////////////////////////////////////////////////////////////////////////////
CMonsterBitmap::CMonsterBitmap()
{

}

void CMonsterBitmap::LoadBitmap(int id,char *path,COLORREF color)
{
	//
	// 同時讀取雙面向圖片
	//
	pic_right[id].LoadBitmap(path,color);
	path[strlen(path)-4] = '\0';
	strcat(path,"_rev.bmp");
	pic_left[id].LoadBitmap(path,color);
}

void CMonsterBitmap::ShowBitmap(bool facing,int id,int x,int y)
{
	if (facing)
	{
		pic_right[id].SetTopLeft(x,y);
		pic_right[id].ShowBitmap();
	}
	else
	{
		pic_left[id].SetTopLeft(x,y);
		pic_left[id].ShowBitmap();
	}
}

int CMonsterBitmap::GetPicWidth(bool facing,int id)
{
	return facing*pic_left[id].Width()+!facing*pic_right[id].Width();
}

int CMonsterBitmap::GetPicHeight(bool facing,int id)
{
	return facing*pic_left[id].Height()+!facing*pic_right[id].Height();
}

/////////////////////////////////////////////////////////////////////////////
// CMonsterData: MonsterData class
/////////////////////////////////////////////////////////////////////////////
CMonsterData::CMonsterData()
{
	HealthPointMax = 0;
	AttackPoint = 0;
	DefencePoint = 0;
	DeathFrame = 0;
	MonsterBitmaps = NULL;
}

void CMonsterData::LoadData(char* data_path,CMonsterBitmap* bitmap)
{
	MonsterBitmaps = bitmap;
	//
	// 從外部讀入怪物的資料
	//
	char tag[128], str[128];
	int R, G, B, frame_num;

	ifstream input(data_path,ios::in);
	//
	// 讀入圖檔
	//
	input >> tag;//<bmp_begin>
	input >> tag >> R >> G >> B >> str;//設定透明色
	for (int i=0; strcmp(str,"<bmp_end>"); i++)
	{
		MonsterBitmaps->LoadBitmap(i,str,RGB(R,G,B));
		input >> str;
	}
	//
	// 讀入怪物屬性
	//
	input
	>> tag//<attribute>
	>> tag >> HealthPointMax
	>> tag >> AttackPoint
	>> tag >> DefencePoint
	>> tag >> DeathFrame
	>> tag;//<attribute_end>
	//
	// 讀入畫格
	//
	while (!input.eof())
	{
		input >> str;
		if (!strcmp(str,"<frame>"))
		{
			//
			// 畫格基本屬性
			//
			int kind, pic, wait, next, dvx, dvy;
			input >> frame_num >> tag;
			input 
			>> tag >> kind
			>> tag >> pic
			>> tag >> wait
			>> tag >> next
			>> tag >> dvx
			>> tag >> dvy
			>> tag;
			frame[frame_num].SetFrame(kind,pic,wait,next,dvx,dvy);
			while (strcmp(tag,"<frame_end>"))
			{
				//
				// Body矩形
				//
				if (!strcmp(tag,"bdy:"))
				{
					int bdx, bdy, bdw, bdh;
					input 
					>> tag >> bdx
					>> tag >> bdy
					>> tag >> bdw
					>> tag >> bdh
					>> tag >> tag;
					frame[frame_num].AddBody(bdx,bdy,bdw,bdh);
				}
				//
				// Attack矩形
				//
				if (!strcmp(tag,"atk:"))
				{
					int atkx, atky, atkw, atkh;
					input 
					>> tag >> atkx
					>> tag >> atky
					>> tag >> atkw
					>> tag >> atkh
					>> tag >> tag;
					frame[frame_num].AddAttack(atkx,atky,atkw,atkh);
				}
				//
				// Trigger矩形
				//
				if (!strcmp(tag,"trig:"))
				{
					int trigx, trigy, trigw, trigh, trig_goto;
					input
					>> tag >> trigx
					>> tag >> trigy
					>> tag >> trigw
					>> tag >> trigh
					>> tag >> trig_goto
					>> tag >> tag;
					frame[frame_num].AddTrigger(trigx,trigy,trigw,trigh,trig_goto);
				}
				//
				// BodyTrigger矩形
				//
				if (!strcmp(tag,"bdt:"))
				{
					int bdtx, bdty, bdtw, bdth, bdt_goto;
					input
					>> tag >> bdtx
					>> tag >> bdty
					>> tag >> bdtw
					>> tag >> bdth
					>> tag >> bdt_goto
					>> tag >> tag;
					frame[frame_num].AddBodyTrigger(bdtx,bdty,bdtw,bdth,bdt_goto);
				}
				//
				// BallPoint點
				//
				if (!strcmp(tag,"bpoint:"))
				{
					int bpx, bpy, bpid;
					input
					>> tag >> bpx
					>> tag >> bpy
					>> tag >> bpid
					>> tag >> tag;
					frame[frame_num].AddBallPoint(bpx,bpy,bpid);
				}
				//
				// Timer計時器
				//
				if (!strcmp(tag,"timer:"))
				{
					frame[frame_num].TimerExist = true;
					input
					>> tag >> frame[frame_num].TimerReset
					>> tag >> frame[frame_num].TimerTriggedTime
					>> tag >> frame[frame_num].TimerGoTo
					>> tag >> tag;
				}
			}
		}
	}
	input.close();
}

CFrame* CMonsterData::GetFrame(int frame_num)
{
	return &frame[frame_num];
}

CMonsterBitmap* CMonsterData::GetBitmaps()
{
	return MonsterBitmaps;
}

int CMonsterData::GetHPMax()
{
	return HealthPointMax;
}

int CMonsterData::GetATK()
{
	return AttackPoint;
}

int CMonsterData::GetDEF()
{
	return DefencePoint;
}

int CMonsterData::GetDeathFrame()
{
	return DeathFrame;
}

/////////////////////////////////////////////////////////////////////////////
// CMonster: Monster class
/////////////////////////////////////////////////////////////////////////////
CMonster::CMonster()
{
	WaitTime = 0;
	HealthPoint = 0;
	FrameNum = 0;
	Alive = false;
	isFacingRight = false;
	DropTime = 0;
	BallInvTime = 0;
}

void CMonster::LoadBitmap()
{
	dam_num.LoadBitmap(0);
}

void CMonster::Add(CMonsterData* data)
{
	this->Data = data;
	HealthPoint = Data->GetHPMax();
	Alive = true;
}

void CMonster::OnMove(CGameMap* InMap, CHeroSoma* soma)
{
	if (Alive == false)
		return;
	CFrame *frame = Data->GetFrame(FrameNum);
	int Sx = soma->GetX();
	int Sx1 = soma->GetX1();
	int Sx2 = soma->GetX2();
	int Sy1 = soma->GetY1();
	int Sy2 = soma->GetY2();
	//
	// Kind檢測
	//
	switch (frame->Kind())
	{
	//
	// 怪物是否轉向主角
	//
	case 0:
		isFacingRight = Sx>x;
		break;
	//
	// 使怪物可落下
	//
	case 2:
		isFacingRight = Sx>x;
	case 3:
		DropDown(InMap);
		break;
	//
	// 使怪物死亡
	//
	case -1:
		Alive = false;
		break;
	}
	//
	// 依照dvx,dvy移動怪物
	//
	if (frame->Dvx())
	{
		int temp_x = abs(frame->Dvx());
		int walk_step = 2*(frame->Dvx()/temp_x);
		if (!isFacingRight)
			walk_step=walk_step*-1;
		for (int i=0; i<temp_x;i++)
		{
			if (InMap->IsEmpty(x-14+walk_step,y)&&InMap->IsEmpty(x+16+walk_step,y))
				x=x+walk_step;
			else if  (InMap->IsEmpty(x-14+walk_step,y-2)&&InMap->IsEmpty(x+16+walk_step,y-2)&&(frame->Kind()==2||frame->Kind()==3))
			{
				x=x+walk_step;
				y-=2;
			}
		}
	}
	if (frame->Dvy())
	{
		int temp_y = abs(frame->Dvy());
		int walk_step = -2*(frame->Dvy()/temp_y);
		for (int i=0; i<temp_y;i++)
		{
			y+=walk_step;
		}
	}
	//
	// Timer檢測
	//
	if (frame->TimerExist)
	{
		//
		// 重設Timer
		//
		if (frame->TimerReset)
		{
			Timmer = 0;
		}
		//
		//  如果到達Timer的觸發時間，跳至TimerGoTo畫格
		//
		else
		{
			if (Timmer<frame->TimerTriggedTime)
			{
				Timmer++;
			}
			else
			{
				FrameNum = frame->TimerGoTo;
				Timmer = 0;
				return;
			}
		}
	}
	//
	// Trigger檢測
	//
	if (frame->trig.size())
	{
		CMonsterBitmap *bitmap = Data->GetBitmaps();
		bool beHit = false;
		for (frame->trig_iter=frame->trig.begin(); frame->trig_iter!=frame->trig.end(); frame->trig_iter++)
		{
			int Tx1, Tx2;
			int Ty1 = y-bitmap->GetPicHeight(isFacingRight,frame->Pic())+1+frame->trig_iter->Y();
			int Ty2 = Ty1+frame->trig_iter->Height();
			if (isFacingRight)
			{
				Tx1 = x-bitmap->GetPicWidth(isFacingRight,frame->Pic())/2+1+frame->trig_iter->X();
				Tx2 = Tx1+frame->trig_iter->Width();
			}
			else
			{
				Tx2 = x+bitmap->GetPicWidth(isFacingRight,frame->Pic())/2+1-frame->trig_iter->X();
				Tx1 = Tx2-frame->trig_iter->Width();
			}
			//
			// 如果主角接觸到Triger矩形，跳至TrigGoTo畫格
			//
			if (Sx2 >= Tx1 && Sx1 <= Tx2 && Sy2 >= Ty1 && Sy1 <= Ty2)
			{
				FrameNum = frame->trig_iter->Goto();
				WaitTime = 0;
				return;
			}
		}
	}
	//
	// 如果到達畫格時間，前往下一個畫格
	//
	if (WaitTime<frame->Wait())
	{
		WaitTime++;
	}
	else
	{
		FrameNum = frame->Next();
		WaitTime = 0;
	}
}

void CMonster::OnShow(CGameMap *InMap)
{
	if (!Alive)
		return;
	CFrame *frame = Data->GetFrame(FrameNum);
	CMonsterBitmap *bitmap = Data->GetBitmaps();
	int X = x-bitmap->GetPicWidth(isFacingRight,frame->Pic())/2+1;
	int Y = y-bitmap->GetPicHeight(isFacingRight,frame->Pic())+1;

	bitmap->ShowBitmap(isFacingRight,frame->Pic(),InMap->ScreenX(X),InMap->ScreenY(Y));
}

void CMonster::OnShowNumber(CGameMap *InMap)
{
	int X = dam_num_x-0;
	int Y = dam_num_y-64;

	if (dam_num_time)
	{
		dam_num.SetTopLeft(InMap->ScreenX(X),InMap->ScreenY(Y));
		dam_num.ShowBitmap();
		dam_num_time-=1;
	}
}

void CMonster::HurtHero(CHeroSoma *soma)
{
	if (Alive)
	{
		CFrame *frame = Data->GetFrame(FrameNum);
		CMonsterBitmap *bitmap = Data->GetBitmaps();
		int PW = bitmap->GetPicWidth(isFacingRight,frame->Pic());
		int PH = bitmap->GetPicHeight(isFacingRight,frame->Pic());
		bool beHit = false;
		int Sx1 = soma->GetX1()
			, Sx2 = soma->GetX2()
			, Sy1 = soma->GetY1()
			, Sy2 = soma->GetY2();
		//
		// 檢測所有Atk
		//
		for (frame->atk_iter=frame->atk.begin(); frame->atk_iter!=frame->atk.end(); frame->atk_iter++)
		{
			int Ax1, Ax2;
			int Ay1 = y-PH+1+frame->atk_iter->Y();
			int Ay2 = Ay1+frame->atk_iter->Height();
			if (isFacingRight)
			{
				Ax1 = x-PW/2+1+frame->atk_iter->X();
				Ax2 = Ax1+frame->atk_iter->Width();
			}
			else
			{
				Ax2 = x+PW/2+1-frame->atk_iter->X();
				Ax1 = Ax2-frame->atk_iter->Width();
			}
			beHit = beHit||(Ax2 >= Sx1 && Ax1 <= Sx2 && Ay2 >= Sy1 && Ay1 <= Sy2);
		}
		//
		// 檢測所有Bdy
		//
		for (frame->bdy_iter=frame->bdy.begin(); frame->bdy_iter!=frame->bdy.end(); frame->bdy_iter++)
		{
			int Bx1, Bx2;
			int By1 = y-PH+1+frame->bdy_iter->Y();
			int By2 = By1+frame->bdy_iter->Height();
			if (isFacingRight)
			{
				Bx1 = x-PW/2+1+frame->bdy_iter->X();
				Bx2 = Bx1+frame->bdy_iter->Width();
			}
			else
			{
				Bx2 = x+PW/2+1-frame->bdy_iter->X();
				Bx1 = Bx2-frame->bdy_iter->Width();
			}
			beHit = beHit||(Bx2 >= Sx1 && Bx1 <= Sx2 && By2 >= Sy1 && By1 <= Sy2);
		}
		if (beHit)
		{
			soma->Injured(Data->GetATK());
		}
	}
}

void CMonster::Hurted(CHeroSoma *soma,CBall *ball)
{
	if (!Alive)
		return;
	CFrame *frame = Data->GetFrame(FrameNum);
	CMonsterBitmap *bitmap = Data->GetBitmaps();
	int PW = bitmap->GetPicWidth(isFacingRight,frame->Pic());
	int PH = bitmap->GetPicHeight(isFacingRight,frame->Pic());
	//
	// 檢查英雄武器攻擊
	//
	if (soma->AttackCheck())
	{
		bool beHit = false;
		int bdt_goto = -1;
		int Sx1 = soma->GetAttackX1()
			, Sx2 = soma->GetAttackX2()
			, Sy1 = soma->GetAttackY1()
			, Sy2 = soma->GetAttackY2();
		//
		// 檢查所有Body
		//
		for (frame->bdy_iter=frame->bdy.begin(); frame->bdy_iter!=frame->bdy.end(); frame->bdy_iter++)
		{
			int Bx1, Bx2;
			int By1 = y-PH+1+frame->bdy_iter->Y();
			int By2 = By1+frame->bdy_iter->Height();
			if (isFacingRight)
			{
				Bx1 = x-PW/2+1+frame->bdy_iter->X();
				Bx2 = Bx1+frame->bdy_iter->Width();
			}
			else
			{
				Bx2 = x+PW/2+1-frame->bdy_iter->X();
				Bx1 = Bx2-frame->bdy_iter->Width();
			}
			beHit = beHit||(Bx2 >= Sx1 && Bx1 <= Sx2 && By2 >= Sy1 && By1 <= Sy2);
		}
		//
		// 檢查所有BodyTrigger
		//
		for (frame->bdt_iter=frame->bdt.begin(); frame->bdt_iter!=frame->bdt.end(); frame->bdt_iter++)
		{
			int BTx1, BTx2;
			int BTy1 = y-PH+1+frame->bdt_iter->Y();
			int BTy2 = BTy1+frame->bdt_iter->Height();
			if (isFacingRight)
			{
				BTx1 = x-PW/2+1+frame->bdt_iter->X();
				BTx2 = BTx1+frame->bdt_iter->Width();
			}
			else
			{
				BTx2 = x+PW/2+1-frame->bdt_iter->X();
				BTx1 = BTx2-frame->bdt_iter->Width();
			}
			if (BTx2 >= Sx1 && BTx1 <= Sx2 && BTy2 >= Sy1 && BTy1 <= Sy2)
				bdt_goto = frame->bdt_iter->Goto();
			beHit = beHit||(BTx2 >= Sx1 && BTx1 <= Sx2 && BTy2 >= Sy1 && BTy1 <= Sy2);
		}
		//
		// 成功受傷
		//
		if (beHit)
		{
			int damage = soma->GetAtk()-Data->GetDEF();
			if (damage<=0)
				damage = 1;
			HealthPoint-=damage;
			if (bdt_goto!=-1)
			{
				SetFrame(bdt_goto);
			}
			if (HealthPoint<=0)
			{
				FrameNum = Data->GetDeathFrame();
				WaitTime = 0;
			}
			dam_num.SetNumber(damage);
			dam_num_time = 15;
			dam_num_x = x;
			dam_num_y = y;
		}
	}
	//
	// 檢查英雄魔法攻擊
	//
	if (BallInvTime)
	{
		BallInvTime--;
		return;
	}
	if (ball->AliveCheck())
	{
		bool beHit = false;
		int bdt_goto = -1;
		bool BallFacing = ball->GetFacing();
		int Ax = ball->GetX();
		int Ay = ball->GetY();
		int Aw = ball->GetPicWidth();
		int Ah = ball->GetPicHeight();
		list<CFrameBody> atk = ball->GetFrameAtk();
		list<CFrameBody>::iterator atk_iter;
		//
		// 所有球的攻擊區塊(Attack)
		//
		for (atk_iter=atk.begin(); atk_iter!=atk.end(); atk_iter++)
		{
			int Ax1, Ax2;
			int Ay1 = Ay-Ah+1+atk_iter->Y();
			int Ay2 = Ay1+atk_iter->Height();
			if (BallFacing)
			{
				Ax1 = Ax-Aw/2+1+atk_iter->X();
				Ax2 = Ax1+atk_iter->Width();
			}
			else
			{
				Ax2 = Ax+Aw/2+1-atk_iter->X();
				Ax1 = Ax2-atk_iter->Width();
			}
			//
			// 檢查所有Body
			//
			for (frame->bdy_iter=frame->bdy.begin(); frame->bdy_iter!=frame->bdy.end(); frame->bdy_iter++)
			{

				int Bx1, Bx2;
				int By1 = y-PH+1+frame->bdy_iter->Y();
				int By2 = By1+frame->bdy_iter->Height();
				if (isFacingRight)
				{
					Bx1 = x-PW/2+1+frame->bdy_iter->X();
					Bx2 = Bx1+frame->bdy_iter->Width();
				}
				else
				{
					Bx2 = x+PW/2+1-frame->bdy_iter->X();
					Bx1 = Bx2-frame->bdy_iter->Width();
				}
				beHit = beHit||(Bx2 >= Ax1 && Bx1 <= Ax2 && By2 >= Ay1 && By1 <= Ay2);
			}
			//
			// 檢查所有BodyTrigger
			//
			for (frame->bdt_iter=frame->bdt.begin(); frame->bdt_iter!=frame->bdt.end(); frame->bdt_iter++)
			{
				int BTx1, BTx2;
				int BTy1 = y-PH+1+frame->bdt_iter->Y();
				int BTy2 = BTy1+frame->bdt_iter->Height();
				if (isFacingRight)
				{
					BTx1 = x-PW/2+1+frame->bdt_iter->X();
					BTx2 = BTx1+frame->bdt_iter->Width();
				}
				else
				{
					BTx2 = x+PW/2+1-frame->bdt_iter->X();
					BTx1 = BTx2-frame->bdt_iter->Width();
				}
				if (BTx2 >= Ax1 && BTx1 <= Ax2 && BTy2 >= Ay1 && BTy1 <= Ay2)
					bdt_goto = frame->bdt_iter->Goto();
				beHit = beHit||(BTx2 >= Ax1 && BTx1 <= Ax2 && BTy2 >= Ay1 && BTy1 <= Ay2);
			}
		}
		//
		// 成功受傷
		//
		if (beHit)
		{
			int damage = ball->GetAtk()-Data->GetDEF();
			if (damage<=0)
				damage = 1;
			HealthPoint-=damage;
			if (bdt_goto!=-1)
			{
				SetFrame(bdt_goto);
			}
			if (HealthPoint<=0)
			{
				FrameNum = Data->GetDeathFrame();
				WaitTime = 0;
			}
			BallInvTime = 30;
			dam_num.SetNumber(damage);
			dam_num_time = 15;
			dam_num_x = x;
			dam_num_y = y;
		}
	}
}

void CMonster::DropDown(CGameMap *InMap)
{
	if (InMap->IsEmpty(x-14,y+2)&&InMap->IsEmpty(x+16,y+2))
	{
		DropTime+=1;
		int VeloDrop = int(double(DropTime)/2.0);
		for (int i=0; i<VeloDrop; i++)
		{
			if (InMap->IsEmpty(x-14,y+2)&&InMap->IsEmpty(x+16,y+2))
			{
				y+=2;
			}
		}
	}
	else
	{
		DropTime = 0;
	}
}

void CMonster::CastBallCheck(list<CBall> *MonsterBall,CBallData *BallData)
{
	if (!Alive)
		return;
	CFrame *frame = Data->GetFrame(FrameNum);
	CMonsterBitmap *bitmap = Data->GetBitmaps();
	int PW = bitmap->GetPicWidth(isFacingRight,frame->Pic());
	int PH = bitmap->GetPicHeight(isFacingRight,frame->Pic());
	//
	// 檢測BallPoint
	//
	for (frame->bpoint_iter=frame->bpoint.begin();frame->bpoint_iter!=frame->bpoint.end();frame->bpoint_iter++)
	{
		int Bx, By = y-PH+1+frame->bpoint_iter->Y();
		if (isFacingRight)
			Bx = x-PW/2+1+frame->bpoint_iter->X();
		else
			Bx = x+PW/2+1-frame->bpoint_iter->X();
		CBall add_ball;
		add_ball.BeCast(isFacingRight,&BallData[frame->bpoint_iter->ID()]);
		add_ball.SetXY(Bx,By);
		MonsterBall->insert(MonsterBall->end(),add_ball);
	}
}

void CMonster::SetXY(int x,int y)
{
	this->x = x;
	this->y = y;
}

void CMonster::SetFrame(int frame)
{
	FrameNum = frame;
	WaitTime = 0;
}

bool CMonster::AliveCheck()
{
	return Alive;
}

/////////////////////////////////////////////////////////////////////////////
// CGameMenu: GameMenu class
/////////////////////////////////////////////////////////////////////////////
CGameMenu::CGameMenu()
{
	Initialize();
}

void CGameMenu::Initialize()
{
	menu_page = 0;
	btn_state = 0;
	btn_state_equip = 0;
	equip_ball_type = 2;
}

void CGameMenu::LoadBitmap()
{
	menu_bg.LoadBitmap(".\\Bitmaps\\menu\\menu_bg.bmp",RGB(0,255,0));
	menu_bg.SetTopLeft(112,112);
	text_bar.LoadBitmap(".\\Bitmaps\\menu\\text_bar.bmp",RGB(0,255,0));
	text_bar.SetTopLeft(112,112+208+16);
	button.LoadBitmap(".\\Bitmaps\\menu\\button.bmp");
	button_light.LoadBitmap(".\\Bitmaps\\menu\\button_light.bmp");
	button_text[0].LoadBitmap(".\\Bitmaps\\menu\\button_state.bmp",RGB(0,255,0));
	button_text[1].LoadBitmap(".\\Bitmaps\\menu\\button_equip.bmp",RGB(0,255,0));
	button_text[2].LoadBitmap(".\\Bitmaps\\menu\\button_help.bmp",RGB(0,255,0));
	button_text[3].LoadBitmap(".\\Bitmaps\\menu\\button_exitgame.bmp",RGB(0,255,0));
	state_bg.LoadBitmap(".\\Bitmaps\\menu\\state_bg.bmp",RGB(0,255,0));
	state_bg.SetTopLeft(112+128+16,112);
	help_bg.LoadBitmap(".\\Bitmaps\\menu\\help_bg.bmp",RGB(0,255,0));
	help_bg.SetTopLeft(112+128+16,112);
	equip_bg.LoadBitmap(".\\Bitmaps\\menu\\menu_bg.bmp",RGB(0,255,0));
	equip_bg.SetTopLeft(112+128+16,112);
	ball_button.LoadBitmap(".\\Bitmaps\\menu\\ball_button.bmp");
	ball_button_light.LoadBitmap(".\\Bitmaps\\menu\\ball_button_light.bmp");
	ball_button_icon[0].LoadBitmap(".\\Bitmaps\\menu\\beam_icon.bmp",RGB(0,255,0));
	ball_button_icon[1].LoadBitmap(".\\Bitmaps\\menu\\fireball_icon.bmp",RGB(0,255,0));
	ball_button_icon[2].LoadBitmap(".\\Bitmaps\\menu\\axe_icon.bmp",RGB(0,255,0));
	ball_button_icon[3].LoadBitmap(".\\Bitmaps\\menu\\hatchet_icon.bmp",RGB(0,255,0));
	equip_icon.LoadBitmap(".\\Bitmaps\\menu\\equip_icon.bmp");
	HP.LoadBitmap();
	HP.SetTopLeft(326,238);
	HPMAX.LoadBitmap();
	HPMAX.SetTopLeft(392,238);
	MP.LoadBitmap();
	MP.SetTopLeft(326,272);
	MPMAX.LoadBitmap();
	MPMAX.SetTopLeft(392,272);
}
void CGameMenu::OnShow()
{
	menu_bg.ShowBitmap();
	text_bar.ShowBitmap();
	switch (menu_page)
	{
	case 1: //State頁面
		state_bg.ShowBitmap();
		HP.ShowBitmap();
		HPMAX.ShowBitmap();
		MP.ShowBitmap();
		MPMAX.ShowBitmap();
		break;
	case 2: //Help頁面
		help_bg.ShowBitmap();
		break;
	case 3: //Equip頁面
		equip_bg.ShowBitmap();
		for (int i=0; i<4; i++)
		{
			ball_button.SetTopLeft(128+128+16,128+i*48);
			ball_button_light.SetTopLeft(128+128+16,128+i*48);
			ball_button_icon[i].SetTopLeft(128+128+16,128+i*48);
			ball_button.ShowBitmap();
			if (btn_state_equip==i)
				ball_button_light.ShowBitmap();
			ball_button_icon[i].ShowBitmap();
			if (equip_ball_type==i)
			{
				equip_icon.SetTopLeft(128+128+16,128+i*48);
				equip_icon.ShowBitmap();
			}
		}
		break;
	}
	for (int i=0; i<4; i++)
	{
		button.SetTopLeft(128,128+i*48);
		button_light.SetTopLeft(128,128+i*48);
		button_text[i].SetTopLeft(128,128+i*48);
		button.ShowBitmap();
		if (btn_state==i)
			button_light.ShowBitmap();
		button_text[i].ShowBitmap();
	}
}

int CGameMenu::OnKeyDown(UINT nChar)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY_SPACE = 0x20; // keyboard空白鍵
	const char KEY_Z	 = 'Z';
	const char KEY_X	 = 'X';
	switch (menu_page)
	{
	case 0:
		if (nChar==KEY_DOWN)
		{
			if (btn_state!=3)
				btn_state += 1;
			else
				btn_state -= 3;
		}
		if (nChar==KEY_UP)
		{
			if (btn_state!=0)
				btn_state -= 1;
			else
				btn_state += 3;
		}
		//
		if (nChar==KEY_Z)
			return btn_state;
		if (nChar==KEY_X)
			return 9;
		break;
	case 1: //State頁面
	case 2: //Help頁面
		if (nChar==KEY_Z||nChar==KEY_X)
			menu_page = 0;
		break;
	case 3: //Equip頁面
		if (nChar==KEY_DOWN)
		{
			if (btn_state_equip!=3)
				btn_state_equip += 1;
			else
				btn_state_equip -= 3;
		}
		if (nChar==KEY_UP)
		{
			if (btn_state_equip!=0)
				btn_state_equip -= 1;
			else
				btn_state_equip+=3;
		}
		if (nChar==KEY_Z)
		{
			equip_ball_type = btn_state_equip;
			return btn_state_equip+4;
		}
		if (nChar==KEY_X)
			menu_page = 0;
		break;
	}
	return 10;
}

void CGameMenu::OpenState(CHeroSoma *soma)
{
	menu_page = 1;
	HP.SetNumber(soma->GetHp());
	HPMAX.SetNumber(soma->GetHpMax());
	MP.SetNumber(soma->GetMp()/10);
	MPMAX.SetNumber(soma->GetMpMax()/10);
}

void CGameMenu::OpenHelp()
{
	menu_page = 2;
}

void CGameMenu::OpenEquip(int ball_type)
{
	menu_page = 3;
	btn_state_equip = 0;
	equip_ball_type = ball_type;
}
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////
CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0);	// 一開始的loading進度為0%
	//
	// 開始載入資料
	//
	logo.LoadBitmapA(".//bitmaps//title.bmp");
	ShowInitProgress(3);
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ESC = 27;
	const char KEY_SPACE = ' ';
	if (nChar == KEY_SPACE)
		GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
	else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// 關閉遊戲
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
	//
	// 貼上logo
	//
	logo.SetTopLeft(0,0);
	logo.ShowBitmap();
}								

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{
}

void CGameStateOver::OnMove()
{
	counter--;
	if (counter < 0)
		GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
	counter = 30*6; // 6 seconds
	CAudio::Instance()->Stop(AUDIO_VampireKiller);
	CAudio::Instance()->Stop(AUDIO_Boss);
	CAudio::Instance()->Play(AUDIO_GameOver);
}

void CGameStateOver::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	CAudio::Instance()->Load(AUDIO_GameOver, ".//Sounds//GameOver.mp3");
	pic_gameover.LoadBitmap(".//Bitmaps//gameover.bmp");
	ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
	pic_gameover.ShowBitmap();					// 顯示GameOver圖片
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g)
{
	Monster = NULL;
	Mx = NULL;
	My = NULL;
	Mf = NULL;
}

CGameStateRun::~CGameStateRun()
{
	delete [] Monster;
	delete [] Mx;
	delete [] My;
	delete [] Mf;
}

void CGameStateRun::OnBeginState()
{
	soma.Initialize();
	map_index = 0;
	heroball_type = 2;
	gamemap[map_index].Initialize();
	lifebar.Initialize();
	boss_door_y = 128;
	BossDoorLock = false;
	BossDead = false;
	BossMusic = false;
	menu_switch = false;
	TimeCount = 30*6;
	AddMonster(gamemap[map_index].GetMapName());
	CAudio::Instance()->Play(AUDIO_VampireKiller,true);
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	if (menu_switch)
		return;
	//
	// Hero
	//
	soma.OnMove(&gamemap[map_index]);
	if (soma.CastCheck())
	{
		soma.CastBall(&heroball,&BallData[heroball_type]);
	}
	heroball.OnMove(&gamemap[map_index]);
	//
	// Map
	//
	gamemap[map_index].OnMove(&soma,&heroball);
	//
	// LifeBar
	//
	lifebar.OnMove(&soma);
	//
	// Monster On Move
	//
	for (int i=0; i<monster_num; i++)
	{
		Monster[i].Hurted(&soma,&heroball);
		Monster[i].HurtHero(&soma);
		Monster[i].CastBallCheck(&MonsterBall,BallData);
		Monster[i].OnMove(&gamemap[map_index],&soma);
	}
	//
	// Monster Ball On Move
	//
	for (list<CBall>::iterator MonBallIter=MonsterBall.begin();MonBallIter!=MonsterBall.end();MonBallIter++)
	{
		MonBallIter->OnMove(&gamemap[map_index]);
		MonBallIter->HurtHero(&soma);
	}
	//
	// 雜項處裡
	//
	switch (map_index)
	{
	case 0:
		if ((soma.GetX()-223)*(soma.GetX()-255)<=0)
			soma.Restore();
		anime_bonfire.OnMove();
		break;
	case 3:
		if (soma.GetX()>=1408&&boss_door_y>32)
			boss_door_y-=2;
		else if (soma.GetX()<1408&&boss_door_y<128)
			boss_door_y+=2;
		gamemap[3].Blocker[0].SetTopLeft(gamemap[3].Blocker[0].GetX1(),boss_door_y);
		break;
	case 4:
		if (soma.GetX()>=63)
			BossDoorLock = true;
		if (BossDoorLock&&boss_door_y<128)
			boss_door_y+=8;
		if (!BossDoorLock)
		{
			boss_door_y=32;
		}
		else if (!BossMusic)
		{
			CAudio::Instance()->Play(AUDIO_Boss,true);
			BossMusic = true;
		}
		gamemap[4].Blocker[0].SetTopLeft(gamemap[4].Blocker[0].GetX1(),boss_door_y);
		if (!Monster[0].AliveCheck()&&!BossDead)
		{
			BossDead = true;
			CAudio::Instance()->Stop(AUDIO_Boss);
			CAudio::Instance()->Play(AUDIO_StageClear);
		}
		if (BossDead)
			TimeCount--;
		break;
	}
	//
	// 處理地圖移動
	//
	MapChange(soma.GetX(),soma.GetY());
	//
	// 偵測是否為GAMEOVER
	//
	if (!soma.AliveCheck()||TimeCount<=0)
	{
		CAudio::Instance()->Stop(AUDIO_VampireKiller);
		GotoGameState(GAME_STATE_OVER);
	}
}

void CGameStateRun::OnInit()  							// 遊戲的初值及圖形設定
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(3);	// 接個前一個狀態的進度

	CAudio::Instance()->Load(AUDIO_VampireKiller, ".\\sounds\\VampireKiller.mp3");
	CAudio::Instance()->Load(AUDIO_Boss, ".\\sounds\\Boss.mp3");
	CAudio::Instance()->Load(AUDIO_StageClear, ".\\sounds\\StageClear.mp3");
	ShowInitProgress(5);

	frame_480_320.LoadBitmap(".\\bitmaps\\frame.bmp",RGB(64,64,104));
	stageclear.LoadBitmap(".\\bitmaps\\stageclear.bmp",RGB(0,96,0));
	stageclear.SetTopLeft(210,226);
	ShowInitProgress(7);

	soma.LoadBitmap();
	ShowInitProgress(30);

	lifebar.LoadBitmap();
	game_menu.LoadBitmap();
	ShowInitProgress(35);
	//
	// 讀取大地圖(關卡)資料
	//
	ifstream input(".\\Data\\Maps\\BigMap.txt",ios::in);
	int mapID;
	char mapName[64], tag[64];
	input >> tag >> MapAmount;
	while (!input.eof())
	{
		input >> tag;
		if (!strcmp(tag,"<map>"))
		{
			input >> mapID >> mapName >> tag;
			if (!strcmp(tag,"left:"))
				input >> MapLeft[mapID] >> tag;
			if (!strcmp(tag,"right:"))
				input >> MapRight[mapID] >> tag;
			gamemap[mapID].Initialize();
			gamemap[mapID].SetMap(mapName);
		}
		ShowInitProgress(35+5*MapAmount);
	}
	input.close();
	ShowInitProgress(60);
	//
	// 阻擋物
	//
	gamemap[0].AddBlocker();
	gamemap[3].AddBlocker();
	gamemap[4].AddBlocker();
	ShowInitProgress(70);
	//
	// 篝火
	//
	anime_bonfire.SetDelayCount(3);
	anime_bonfire.AddBitmap(".\\Bitmaps\\map_bonfire\\bonfire_0.bmp",RGB(0,40,0));
	anime_bonfire.AddBitmap(".\\Bitmaps\\map_bonfire\\bonfire_1.bmp",RGB(0,40,0));
	anime_bonfire.AddBitmap(".\\Bitmaps\\map_bonfire\\bonfire_2.bmp",RGB(0,40,0));
	anime_bonfire.SetTopLeft(214+80,208+80);
	ShowInitProgress(80);
	//
	// 怪物
	//
	LoadMonsterData();
	//
	// 球:法術
	//
	LoadBallBitmap();
	ShowInitProgress(85);
	LoadBallData();
	ShowInitProgress(90);
	//
	// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
	//
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_X = 'X';
	const char KEY_M = 'M';

	if (nChar==KEY_M)
	{
		menu_switch = !menu_switch;
		game_menu.Initialize();
	}

	if (menu_switch)
	{
		//
		// 處理Menu
		//
		int state = game_menu.OnKeyDown(nChar);
		switch(state)
		{
		case 0:
			game_menu.OpenState(&soma);
			break;
		case 1:
			game_menu.OpenEquip(heroball_type);
			break;
		case 2:
			game_menu.OpenHelp();
			break;
		case 3:
			GotoGameState(GAME_STATE_INIT);
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			heroball_type = state-4;
			break;
		case 9:
			menu_switch = false;
			break;
		}
	}
	else
	{
		soma.OnKeyDown(nChar);
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	soma.OnKeyUp(nChar);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{

}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{

}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{

}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{

}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{

}

void CGameStateRun::LoadBallBitmap()
{
	ifstream input(".\\Data\\Balls\\ball_bitmaps.txt",ifstream::in);
	int id, r, g, b, count = 0;
	char str[128],tag[128];
	while (!input.eof())
	{
		input >> str;
		if (!strcmp(str,"<bmp_begin>"))
		{
			input >> id >> tag >> tag >> r >> g >> b >> str;
			while (strcmp(str,"<bmp_end>"))
			{
				BallBitmap[id].LoadBitmap(count,str,RGB(r,g,b));
				input >> str;
				count++;
			}
		}
		count = 0;
	}
	input.close();
}

void CGameStateRun::LoadBallData()
{
	ifstream input(".\\Data\\Balls\\ball_list.txt",ifstream::in);
	int id = -1;
	char str[128],tag[128];
	while (!input.eof())
	{
		input >> tag;
		if (!strcmp(tag,"<ball>"))
		{
			input >> id >> tag >> str >> tag;
			BallData[id].LoadData(str,BallBitmap);
		}
	}
	input.close();
}

void CGameStateRun::LoadMonsterData()
{
	ifstream input(".\\Data\\Monsters\\monster_list.txt",ifstream::in);
	int id = -1;
	char str[128],tag[128];
	while (!input.eof())
	{
		input >> tag;
		if (!strcmp(tag,"<monster>"))
		{
			input >> id >> tag >> str >> tag;
			MonsterData[id].LoadData(str,&MonsterBitmap[id]);
		}
	}
	input.close();
}

void CGameStateRun::AddMonster(char *MapName)
{
	delete [] Monster;
	delete [] Mx;
	delete [] My;
	delete [] Mf;
	char str[64], tag[64];
	char MapPath[64] = ".\\Data\\Maps\\";
	int counter = 0;
	//
	// 從外部資料讀入地圖的怪物
	//
	strcat(MapPath,MapName);
	strcat(MapPath,".mst");
	ifstream input(MapPath,ios::in);
	//
	// 取得怪物總數量
	//
	input >> tag >> monster_num;
	//
	// 依照怪物數量新增CMonster及其參數
	//
	Monster = new CMonster[monster_num];
	Mx = new int[monster_num];
	My = new int[monster_num];
	Mf = new int[monster_num];
	int id = 0;
	//
	// 設定怪物
	//
	while (!input.eof())
	{
		input >> tag;
		if (!strcmp(tag,"<monster>"))
		{
			Monster[counter].LoadBitmap();
			input >> str >> tag >> id;
			Monster[counter].Add(&MonsterData[id]);//設定怪物類型
			input >> tag >> Mf[counter];
			Monster[counter].SetFrame(Mf[counter]);//設定怪物初始畫格
			input >> tag >> Mx[counter] >> tag >> My[counter];
			Monster[counter].SetXY(Mx[counter],My[counter]);//設定怪物初始座標
			counter++;
		}
	}
	input.close();
	MonsterBall.clear();
}

void CGameStateRun::MapChange(int Cx,int Cy)
{
	if (Cx<0)
	{
		heroball.DeleteBall();
		map_index = MapLeft[map_index];
		AddMonster(gamemap[map_index].GetMapName());
		gamemap[map_index].SetScreenX(gamemap[map_index].GetWidth()-560);
		soma.SetX(gamemap[map_index].GetWidth()-1);
		soma.SetY(223);
	}
	if (Cx>=gamemap[map_index].GetWidth())
	{
		heroball.DeleteBall();
		map_index = MapRight[map_index];
		AddMonster(gamemap[map_index].GetMapName());
		gamemap[map_index].SetScreenX(-80);
		soma.SetX(0);
		soma.SetY(223);
		if (map_index==4)
		{
			CAudio::Instance()->Stop(AUDIO_VampireKiller);
		}
	}
}

void CGameStateRun::OnShow()
{
	//
	//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
	//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
	//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
	//
	gamemap[map_index].OnShow();
	//
	// Monster On Show
	//
	for (int i=0; i<monster_num; i++)
	{
		Monster[i].OnShow(&gamemap[map_index]);
	}
	for (int i=0; i<monster_num; i++)
	{
		Monster[i].OnShowNumber(&gamemap[map_index]);
	}
	//
	// Monster Ball On Show
	//
	for (list<CBall>::iterator MonBallIter=MonsterBall.begin();MonBallIter!=MonsterBall.end();MonBallIter++)
	{
		MonBallIter->OnShow(&gamemap[map_index]);
	}
	if (!map_index)
		anime_bonfire.OnShow();

	soma.OnShow(&gamemap[map_index]);
	heroball.OnShow(&gamemap[map_index]);

	lifebar.OnShow();

	if (menu_switch)
		game_menu.OnShow();
	frame_480_320.ShowBitmap();
	if (BossDead)
	{
		stageclear.ShowBitmap();
	}
}

}