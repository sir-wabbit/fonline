#include "stdafx.h"

#include "Critter.h"
#include "common.h"

#define random(a) (rand()*a/(RAND_MAX+1))

//для работы с битами (по игре - флагами)
#define BITS(x,y) ((x)&(y))
#define FLAG BITS

#define SET_BITS(x,y) (x)=(x)|(y)
#define SETFLAG SET_BITS

//#define UNSET_BITS(x,y) {if((x)&(y)) (x)=(x)^(y);}
#define UNSET_BITS(x,y) (x)=((x)|(y))^(y)
#define UNSETFLAG UNSET_BITS

//#include <SimpleLeakDetector/SimpleLeakDetector.hpp>
/********************************************************************
	created:	2005   22:04
	edit:		2007   15:15

	author:		Oleg Mareskin
	edit:		Denis Balikhin, Anton Tsvetinsky
	
	purpose:	
*********************************************************************/

//!Cvet +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ переделано и добавлено практически всё
void CCritter::Init()
{
	for(int ts=0; ts<ALL_STATS ; ts++) st[ts]=0;
	for(int ts=0; ts<ALL_SKILLS; ts++) sk[ts]=0;
	for(int ts=0; ts<ALL_PERKS ; ts++) pe[ts]=0;

	currentFrame=0;
	movementType=MOVE_WALK; 
	cur_ox=0; 
	cur_oy=0;
	rate_object=1;

	Tick_start=0;
	Tick_count=0;

	a_obj2=NULL;
	m_obj=NULL;

	text_delay=0;

	text_color=COLOR_TEXT_DEFAULT;

	human=false;

	next_step[0]=0xFF;
	next_step[1]=0xFF;
	next_step[2]=0xFF;
	next_step[3]=0xFF;
	cur_step=0;

	alpha=0xFF;

	SetAnimation();
	
	__invariant();
}

void CCritter::AddObject(uint8_t aslot,uint32_t o_id,uint32_t broken_info,uint32_t time_wear,stat_obj* s_obj)
{
  __invariant();
	dyn_obj* new_obj=new dyn_obj;
	new_obj->id=o_id;
	new_obj->object=s_obj;
	new_obj->type=new_obj->object->type;
	new_obj->time_wear=time_wear;
	new_obj->broken_info=broken_info;

	switch(aslot)
	{
	case DOBJ_SLOT_INV:
		obj[o_id]=new_obj;
		break;
	case DOBJ_SLOT_HAND1:
		if(new_obj->type!=OBJ_TYPE_ARMOR) a_obj=new_obj;
		break;
	case DOBJ_SLOT_HAND2:
		break;
	case DOBJ_SLOT_TWOHAND:
		break;
	case DOBJ_SLOT_ARMOR:
		if(new_obj->type==OBJ_TYPE_ARMOR) a_obj_arm=new_obj;
		break;
	}

	SetAnimation();
}

int CCritter::GetMaxDistance()
{
  __invariant();
	if(a_obj->type==OBJ_TYPE_WEAPON)
	{
		switch(rate_object)
		{
		case 1: return a_obj->object->p[OBJ_WEAP_PA_MAX_DIST];
		case 2: return a_obj->object->p[OBJ_WEAP_SA_MAX_DIST];
		case 3: return a_obj->object->p[OBJ_WEAP_TA_MAX_DIST];
		default: return 1;
		}
	}

	return 1;
}

int CCritter::Move(uint8_t dir)
{
  __invariant();
	//проверяем направление
	if(dir>5 && dir<0) return MOVE_ERROR;

	//если не находим анимацию на бег, то криттер идет пешком
	if(movementType==MOVE_RUN)
		if(!lpSM->CrAnim[type][1][20]) 
			if(!lpSM->LoadAnimCr(type,1,20)) movementType=MOVE_WALK;

	if(movementType==MOVE_WALK)
		if(!lpSM->CrAnim[type][weapon][2]) 
			if(!lpSM->LoadAnimCr(type,weapon,2)) return MOVE_ERROR;

	if(movementType==MOVE_WALK)
	{
		cur_anim=lpSM->CrAnim[type][weapon][2];
		cur_anim->__invariant();
		cnt_per_turn=4;
		ticks_per_turn=cur_anim->ticks/2;
		Tick_Start(ticks_per_turn);

//		cur_id=cur_anim->ind[cur_anim->dir_offs[cur_dir]+currentFrame];
	}
	else if(movementType==MOVE_RUN)
	{
		cur_anim=lpSM->CrAnim[type][1][20];
		cur_anim->__invariant();
		cnt_per_turn=3;
		ticks_per_turn=cur_anim->ticks/3;
		Tick_Start(ticks_per_turn);

//		cur_id=cur_anim->ind[cur_anim->dir_offs[cur_dir]+currentFrame];
	}
	else return MOVE_ERROR;

	cur_dir=dir;

	anim_tkr=GetTickCount();
	change_tkr=GetTickCount();

	return movementType;
}

void CCritter::Action(uint8_t action, uint32_t action_tick)
{
  __invariant();
	Tick_Start(action_tick);

	if(action==ANIM2_USE || action==ANIM2_SIT) weapon=1;

	if(!lpSM->CrAnim[type][weapon][action]) 
		if(!lpSM->LoadAnimCr(type,weapon,action)) return;

	cur_anim=lpSM->CrAnim[type][weapon][action];
	cur_anim->__invariant();

	ticks_per_turn=cur_anim->ticks;
	cnt_per_turn=cur_anim->cnt_frames;

	cur_id=cur_anim->ind[cur_anim->dir_offs[cur_dir]];

	SetCur_offs(cur_anim->next_x[cur_anim->dir_offs[cur_dir]],
		cur_anim->next_y[cur_anim->dir_offs[cur_dir]]);

	anim_tkr=GetTickCount(); //начало анимации
	change_tkr=GetTickCount();//смена кадра
	currentFrame=0;
}

void CCritter::Animate(uint8_t action, uint8_t num_frame)
{
  __invariant();
	if(!lpSM->CrAnim[type][weapon][action])
		if(!lpSM->LoadAnimCr(type,weapon,action)) return;
		
  CritFrames* anim = lpSM->CrAnim[type][weapon][action];
  anim->__invariant();

	if(num_frame==LAST_FRAME) num_frame=anim->cnt_frames-1;

	cur_id = anim->ind[anim->dir_offs[cur_dir]+num_frame];

	SetCur_offs(anim->next_x[anim->dir_offs[cur_dir]+num_frame],
		          anim->next_y[anim->dir_offs[cur_dir]+num_frame]);

	currentFrame=0;
}

void CCritter::RefreshWeap()
{
  __invariant();
	if(a_obj->type==OBJ_TYPE_WEAPON)
		weapon=a_obj->object->p[OBJ_WEAP_ANIM1];
	else
		weapon=1;
}

void CCritter::RefreshType()
{
  __invariant();
	if(!st[ST_GENDER])
	{
		type=a_obj_arm->object->p[OBJ_ARM_ANIM0_MALE];
		type_ext=a_obj_arm->object->p[OBJ_ARM_ANIM0_MALE2];
	}
	else
	{
		type=a_obj_arm->object->p[OBJ_ARM_ANIM0_FEMALE];
		type_ext=a_obj_arm->object->p[OBJ_ARM_ANIM0_FEMALE2];
	}
}

void CCritter::SetAnimation()
{
  __invariant();
	RefreshWeap();
	RefreshType();

	if(cond==COND_LIFE)
	{
		switch (cond_ext)
		{
		case COND_LIFE_NONE:
			Animate(ANIM1_STAY,FIRST_FRAME);
			break;
		case COND_LIFE_ACTWEAP:
			if(a_obj->object->p[OBJ_WEAP_TIME_ACTIV])
				Animate(8,LAST_FRAME);
			else
				Animate(ANIM1_STAY,FIRST_FRAME);
			break;
		case COND_LIFE_USEOBJ:
			Animate(12,LAST_FRAME);
			break;
		default:
			break;
		}
	}
	else if(cond==COND_KNOCK_OUT)
	{
		weapon=2;
		switch (cond_ext)
		{
		case COND_KO_UP:
			Animate(1,LAST_FRAME);
			break;
		case COND_KO_DOWN:
			Animate(2,LAST_FRAME);
			break;
		default:
			break;
		}
	}
	else if(cond==COND_DEAD)
	{
		weapon=2;
		switch (cond_ext)
		{
		case COND_DEAD_NORMAL_UP:
			Animate(15,LAST_FRAME);
			break;
		case COND_DEAD_NORMAL_DOWN:
			Animate(16,LAST_FRAME);
			break;
		case COND_DEAD_CR_NORMAL_UP:
			Animate(4,LAST_FRAME);
			break;
		case COND_DEAD_BRUST:
			Animate(7,LAST_FRAME);
			break;
		case COND_DEAD_CR_BRUST:
			Animate(6,LAST_FRAME);
			break;
		default:
			Animate(15,FIRST_FRAME);
			break;
		}
	}
	else WriteLog("Critter - SetAnimation - UnKnown Condition |%d|\n",cond);

//	WriteLog("SetAnimation - cond=%d,cond_ext=%d\n",cond,cond_ext);
}

void CCritter::SetDirection(uint8_t dir)
{
  __invariant();
	if(dir>5 && dir<0) dir=0;
	cur_dir=dir;

	SetAnimation();
}

void CCritter::RotateClockwise()
{
  __invariant();
	cur_dir++;
	if(cur_dir==6) cur_dir=0;

	SetAnimation();
}

void CCritter::RotateCounterClockwise()
{
  __invariant();
	if(!cur_dir) cur_dir=5;
	else cur_dir--;

	SetAnimation();
}

void CCritter::Process()
{
  __invariant();
	//stay анимация
	if(!cur_anim)
	{
		if(cond==COND_LIFE)
		{
			switch (cond_ext)
			{
			case COND_LIFE_NONE:
				stay_wait+=random(3);
				if(stay_wait>30000)
				{
					Action(1,2500);
					Tick_Null();
				}	
				break;
			case COND_LIFE_ACTWEAP:
				break;
			case COND_LIFE_USEOBJ:
				break;
			default:
				break;
			}
		}
		else if(cond==COND_KNOCK_OUT)
		{
			switch (cond_ext)
			{
			case COND_KO_UP:
				break;
			case COND_KO_DOWN:
				break;
			default:
				break;
			}
		}
		else if(cond==COND_DEAD)
		{
			switch (cond_ext)
			{
			case COND_DEAD_NORMAL_UP:
				break;
			case COND_DEAD_NORMAL_DOWN:
				break;
			case COND_DEAD_CR_NORMAL_UP:
				break;
			case COND_DEAD_BRUST:
				break;
			case COND_DEAD_CR_BRUST:
				break;
			default:
				break;
			}
		}

		return;
	}

	//установлена какая-то анимация
	if(GetTickCount()-anim_tkr<ticks_per_turn)
	{
		if(GetTickCount()-change_tkr>(ticks_per_turn/cnt_per_turn))
		{
			change_tkr=GetTickCount();

			uint16_t or_offs=cur_anim->dir_offs[cur_dir];
			cur_id=cur_anim->ind[or_offs+currentFrame];

			ChangeCur_offs(cur_anim->next_x[or_offs+currentFrame],cur_anim->next_y[or_offs+currentFrame]);

			currentFrame++;

			if(currentFrame>=cur_anim->cnt_frames) currentFrame=0;
		}
	}
	else
	{
		cur_anim=NULL;
		stay_wait=random(5000);

		if(human==false)
		{
			if(cur_step<4)
			{
				if(next_step[cur_step]!=0xFF)
				{
	//				cur_step++;
					return;
				}
			}

	//		cur_step++;
		}

		SetAnimation();
	}
}

void CCritter::ChangeCur_offs(short change_ox, short change_oy)
{
  __invariant();
	cur_ox+=change_ox;
	cur_oy+=change_oy;

//	WriteLog("==================================\n");
//	WriteLog("cur_ox+=change_ox\n");
//	WriteLog("%d+=%d\n",cur_ox,change_ox);
//	WriteLog("cur_oy+=change_oy\n");
//	WriteLog("%d+=%d\n",cur_oy,change_oy);
//	WriteLog("==================================\n");
	
	drect.left += change_ox;
	drect.right += change_ox;
	drect.top += change_oy;
	drect.bottom += change_oy;
}

void CCritter::SetCur_offs(short set_ox, short set_oy)
{
  __invariant();
	int diff_x=set_ox-cur_ox;
	int diff_y=set_oy-cur_oy;

//	WriteLog("----------------------------------\n");
//	WriteLog("diff_x=cur_ox-set_ox\n");
//	WriteLog("%d=%d-%d\n",diff_x,cur_ox,set_ox);
//	WriteLog("diff_y=cur_oy-set_oy\n");
//	WriteLog("%d=%d-%d\n",diff_y,cur_oy,set_oy);
//	WriteLog("----------------------------------\n");

//	cur_ox=diff_x;
//	cur_oy=diff_y;
	cur_ox=set_ox;
	cur_oy=set_oy;

	drect.left += diff_x;
	drect.right += diff_x;
	drect.top += diff_y;
	drect.bottom += diff_y;
}

void CCritter::AccamulateCur_offs()
{
//	if(!cur_anim) return;
//	if(currentFrame<cnt_per_turn) return;

//	for(int i=currentFrame-cnt_per_turn;i<=currentFrame;i++)
//	{
//		if(i<0) i=0;
	//	SetCur_offs(cur_anim->next_x[cur_anim->dir_offs[cur_dir]+currentFrame],
	//		cur_anim->next_y[cur_anim->dir_offs[cur_dir]+currentFrame]);
//	}
}

void CCritter::SetText(char* str, uint32_t color)
{
  __invariant();
  if (text_str != NULL) {
    delete [] text_str;
    text_str = NULL;
  }
	text_str=new char[strlen(str)+1];
	strcpy(text_str,str);
	SetTime=GetTickCount();

	text_delay=opt_text_delay+strlen(str)*100;

	text_color=color;
}

void CCritter::DrawText(CFOFont* lpfnt)
{
  __invariant();
	if(!text_str && FLAG(flags,FCRIT_MOB)) return;
	if(visible)
	{
		int x=drect.left+((drect.right-drect.left)>>1)-100+cmn_scr_ox;
		int y=drect.top-73+cmn_scr_oy;
	
		RECT r={x,y,x+200,y+70};
		lpfnt->RenderText(r,text_str?text_str:name,FT_CENTERX|FT_BOTTOM,text_color);
	}

	if(GetTickCount()-SetTime>=text_delay)
	{
    if (text_str != NULL) {
      delete [] text_str;
      text_str = NULL;
    }
		text_color=COLOR_CRITNAME;
	}
}
//!Cvet ------------------------------------------------------- переделано и добавлено практически всё