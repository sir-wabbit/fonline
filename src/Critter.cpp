#include "stdafx.h"

#include "Critter.h"
#include "common.h"
/********************************************************************
	created:	2005   22:04
	edit:		2007   15:15

	author:		Oleg Mareskin
	edit:		Denis Balikhin, Anton Tsvetinsky
	
	purpose:	
*********************************************************************/

//!Cvet +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ переделано и добавлено практически всё
void CCritter::Initialization()
{
	for(int ts=0; ts<ALL_STATS ; ts++) st[ts]=0;
	for(	ts=0; ts<ALL_SKILLS; ts++) sk[ts]=0;
	for(	ts=0; ts<ALL_PERKS ; ts++) pe[ts]=0;

	cur_afrm=0;
	move_type=MOVE_WALK; 
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
}

void CCritter::AddObject(BYTE aslot,DWORD o_id,DWORD broken_info,DWORD time_wear,stat_obj* s_obj)
{
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

int CCritter::Move(BYTE dir)
{
	//проверяем направление
	if(dir>5 && dir<0) return MOVE_ERROR;

	//если не находим анимацию на бег, то криттер идет пешком
	if(move_type==MOVE_RUN)
		if(!lpSM->CrAnim[type][1][20]) 
			if(!lpSM->LoadAnimCr(type,1,20)) move_type=MOVE_WALK;

	if(move_type==MOVE_WALK)
		if(!lpSM->CrAnim[type][weapon][2]) 
			if(!lpSM->LoadAnimCr(type,weapon,2)) return MOVE_ERROR;

	if(move_type==MOVE_WALK)
	{
		cur_anim=lpSM->CrAnim[type][weapon][2];
		cnt_per_turn=4;
		ticks_per_turn=cur_anim->ticks/2;
		Tick_Start(ticks_per_turn);

//		cur_id=cur_anim->ind[cur_anim->dir_offs[cur_dir]+cur_afrm];
	}
	else if(move_type==MOVE_RUN)
	{
		cur_anim=lpSM->CrAnim[type][1][20];
		cnt_per_turn=3;
		ticks_per_turn=cur_anim->ticks/3;
		Tick_Start(ticks_per_turn);

//		cur_id=cur_anim->ind[cur_anim->dir_offs[cur_dir]+cur_afrm];
	}
	else return MOVE_ERROR;

	cur_dir=dir;

	anim_tkr=GetTickCount();
	change_tkr=GetTickCount();

	return move_type;
}

void CCritter::Action(Byte action, DWORD action_tick)
{
	Tick_Start(action_tick);

	if(action==ANIM2_USE || action==ANIM2_SIT) weapon=1;

	if(!lpSM->CrAnim[type][weapon][action]) 
		if(!lpSM->LoadAnimCr(type,weapon,action)) return;

	cur_anim=lpSM->CrAnim[type][weapon][action];

	ticks_per_turn=cur_anim->ticks;
	cnt_per_turn=cur_anim->cnt_frames;

	cur_id=cur_anim->ind[cur_anim->dir_offs[cur_dir]];

	SetCur_offs(cur_anim->next_x[cur_anim->dir_offs[cur_dir]],
		cur_anim->next_y[cur_anim->dir_offs[cur_dir]]);

	anim_tkr=GetTickCount(); //начало анимации
	change_tkr=GetTickCount();//смена кадра
	cur_afrm=0;
}

void CCritter::Animate(BYTE action, BYTE num_frame)
{
	if(!lpSM->CrAnim[type][weapon][action])
		if(!lpSM->LoadAnimCr(type,weapon,action)) return;

	if(num_frame==LAST_FRAME) num_frame=lpSM->CrAnim[type][weapon][action]->cnt_frames-1;

	cur_id=lpSM->CrAnim[type][weapon][action]->ind[lpSM->CrAnim[type][weapon][action]->dir_offs[cur_dir]+num_frame];

	SetCur_offs(lpSM->CrAnim[type][weapon][action]->next_x[lpSM->CrAnim[type][weapon][action]->dir_offs[cur_dir]+num_frame],
		lpSM->CrAnim[type][weapon][action]->next_y[lpSM->CrAnim[type][weapon][action]->dir_offs[cur_dir]+num_frame]);

	cur_afrm=0;
}

void CCritter::RefreshWeap()
{
	if(a_obj->type==OBJ_TYPE_WEAPON)
		weapon=a_obj->object->p[OBJ_WEAP_ANIM1];
	else
		weapon=1;
}

void CCritter::RefreshType()
{
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

void CCritter::SetDir(BYTE dir)
{
	if(dir>5 && dir<0) dir=0;
	cur_dir=dir;

	SetAnimation();
}

void CCritter::RotCW()
{
	cur_dir++;
	if(cur_dir==6) cur_dir=0;

	SetAnimation();
}

void CCritter::RotCCW()
{
	if(!cur_dir) cur_dir=5;
	else cur_dir--;

	SetAnimation();
}

void CCritter::Process()
{
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

			WORD or_offs=cur_anim->dir_offs[cur_dir];
			cur_id=cur_anim->ind[or_offs+cur_afrm];

			ChangeCur_offs(cur_anim->next_x[or_offs+cur_afrm],cur_anim->next_y[or_offs+cur_afrm]);

			cur_afrm++;

			if(cur_afrm>=cur_anim->cnt_frames) cur_afrm=0;
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
	cur_ox+=change_ox;
	cur_oy+=change_oy;

//	WriteLog("==================================\n");
//	WriteLog("cur_ox+=change_ox\n");
//	WriteLog("%d+=%d\n",cur_ox,change_ox);
//	WriteLog("cur_oy+=change_oy\n");
//	WriteLog("%d+=%d\n",cur_oy,change_oy);
//	WriteLog("==================================\n");
	
	drect.l+=change_ox;
	drect.r+=change_ox;
	drect.t+=change_oy;
	drect.b+=change_oy;
}

void CCritter::SetCur_offs(short set_ox, short set_oy)
{
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

	drect.l+=diff_x;
	drect.r+=diff_x;
	drect.t+=diff_y;
	drect.b+=diff_y;
}

void CCritter::AccamulateCur_offs()
{
//	if(!cur_anim) return;
//	if(cur_afrm<cnt_per_turn) return;

//	for(int i=cur_afrm-cnt_per_turn;i<=cur_afrm;i++)
//	{
//		if(i<0) i=0;
	//	SetCur_offs(cur_anim->next_x[cur_anim->dir_offs[cur_dir]+cur_afrm],
	//		cur_anim->next_y[cur_anim->dir_offs[cur_dir]+cur_afrm]);
//	}
}

void CCritter::SetText(char* str, DWORD color)
{
	SAFEDELA(text_str);
	text_str=new char[strlen(str)+1];
	strcpy(text_str,str);
	SetTime=GetTickCount();

	text_delay=opt_text_delay+strlen(str)*100;

	text_color=color;
}

void CCritter::DrawText(CFOFont* lpfnt)
{
	if(!text_str && FLAG(flags,FCRIT_MOB)) return;
	if(visible)
	{
		int x=drect.l+((drect.r-drect.l)>>1)-100+cmn_scr_ox;
		int y=drect.t-73+cmn_scr_oy;
	
		RECT r={x,y,x+200,y+70};
		lpfnt->MyDrawText(r,text_str?text_str:name,FT_CENTERX|FT_BOTTOM,text_color);
	}

	if(GetTickCount()-SetTime>=text_delay)
	{
		SAFEDELA(text_str);
		text_color=COLOR_CRITNAME;
	}
}
//!Cvet ------------------------------------------------------- переделано и добавлено практически всё