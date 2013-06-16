/********************************************************************
	created:	10:02:2007   15:22;

	author:		Anton Tsvetinsky

	purpose:
*********************************************************************/

#include "stdafx.h"

#include <string.h>
#include <stdio.h>

#include "Critter.h"

CCritter::CCritter(): i_npc(NULL), i_mob(NULL), group_move(NULL), target(NULL) {
	s = -1;
}

CCritter::~CCritter() {
	SAFEDEL(i_npc);
	SAFEDEL(i_mob);
}

int CCritter::InitClient() {
	GenParam();
	GenLook();

	group.xf=info.world_x;
	group.yf=info.world_y;
	group.xi=info.world_x;
	group.yi=info.world_y;

	group.move_x=info.world_x;
	group.move_y=info.world_y;
	group.rule=this;

	return 1;
}

int CCritter::InitNPC()
{
	GenParam();
	GenLook();

	group.xf=info.world_x;
	group.yf=info.world_y;
	group.xi=info.world_x;
	group.yi=info.world_y;

	group.move_x=info.world_x;
	group.move_y=info.world_y;
	group.rule=this;

	return 1;
}

int CCritter::InitMOB()
{
	GenParam();
	GenLook();

	group.xf=info.world_x;
	group.yf=info.world_y;
	group.xi=info.world_x;
	group.yi=info.world_y;

	group.move_x=info.world_x;
	group.move_y=info.world_y;
	group.rule=this;

	return 1;
}

inline void CCritter::GenParam()
{
//s[7]..s[38]
	info.st[ST_MAX_LIFE			]=(info.st[ST_STRENGHT]*1)+(info.st[ST_ENDURANCE]*2)+15;
	info.st[ST_MAX_COND			]=0;
	info.st[ST_ARMOR_CLASS		]=info.st[ST_AGILITY];
	info.st[ST_MELEE_DAMAGE		]=info.st[ST_STRENGHT]/2+1;
	info.st[ST_WEAPON_DAMAGE	]=0;
	info.st[ST_CARRY_WEIGHT		]=info.st[ST_STRENGHT]*8;
	info.st[ST_SEQUENCE			]=0;
	info.st[ST_HEALING_RATE		]=info.st[ST_ENDURANCE];
	info.st[ST_CRITICAL_CHANCE	]=0;
	info.st[ST_MAX_CRITICAL		]=0;
	info.st[ST_INGURE_ABSORB	]=0;
	info.st[ST_LASER_ABSORB		]=0;
	info.st[ST_FIRE_ABSORB		]=0;
	info.st[ST_PLASMA_ABSORB	]=0;
	info.st[ST_ELECTRO_ABSORB	]=0;
	info.st[ST_EMP_ABSORB		]=0;
	info.st[ST_BLAST_ABSORB		]=0;
	info.st[ST_INGURE_RESIST	]=0;
	info.st[ST_LASER_RESIST		]=0;
	info.st[ST_FIRE_RESIST		]=0;
	info.st[ST_PLASMA_RESIST	]=0;
	info.st[ST_ELECTRO_RESIST	]=0;
	info.st[ST_EMP_RESIST		]=0;
	info.st[ST_BLAST_RESIST		]=0;
	info.st[ST_RADIATION_RESISTANCE]=0;
	info.st[ST_POISON_RESISTANCE]=0;
}

inline void CCritter::GenLook()
{
	info.look=20+info.st[ST_PERCEPTION]*2;//*(1-(0.4*acl->info.pe[422]+
		//0.4*acl->info.pe[423]))+(10*acl->info.pe[353]);
}

void CCritter::CopyParamsTo(CCritter* acl)
{
	acl->info.base_type=info.base_type;

	acl->info.map=info.map;
	acl->info.x=info.x;
	acl->info.y=info.y;

	acl->info.world_x=info.world_x;
	acl->info.world_y=info.world_y;

	acl->info.ori=info.ori;
	acl->info.look=info.look;

	strcpy(acl->info.name,info.name);
	strcpy(acl->info.cases[0],info.cases[0]);
	strcpy(acl->info.cases[1],info.cases[1]);
	strcpy(acl->info.cases[2],info.cases[2]);
	strcpy(acl->info.cases[3],info.cases[3]);
	strcpy(acl->info.cases[4],info.cases[4]);

	acl->info.access=info.access;

	acl->info.start_bt=info.start_bt;
	acl->info.break_time=info.break_time;

	acl->info.id=info.id;

	strcpy(acl->info.login,info.login);
	strcpy(acl->info.pass,info.pass);

	for(int bi=0; bi<ALL_STATS;  bi++) acl->info.st[bi]=info.st[bi];
	for(int bi=0; bi<ALL_SKILLS; bi++) acl->info.sk[bi]=info.sk[bi];
	for(int bi=0; bi<ALL_PERKS;  bi++) acl->info.pe[bi]=info.pe[bi];

	acl->info.cond=info.cond;
	acl->info.cond_ext=info.cond_ext;
	acl->info.flags=info.flags;

	acl->info.obj=info.obj;

	acl->info.a_obj=info.a_obj;
	acl->info.a_obj_arm=info.a_obj_arm;

	acl->group=group;

	if(group_move==&group)
	{
		acl->group_move=&acl->group;
		acl->group.rule=acl;
	}
	else
		acl->group_move=group_move;

	acl->known_cities=known_cities;

	acl->vis_cl.clear();
}
