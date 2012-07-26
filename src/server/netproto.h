#ifndef __NETPROTO_H__
#define __NETPROTO_H__


/********************************************************************
	created:	21:12:2004   00:14

	author:		Oleg Mareskin
	
	edit:		Denis Balikhin, Anton Tsvetinsky
	
	purpose:	
*********************************************************************/

//!Cvet +++++++++++++++++++++++++++++++
#define MAX_NPC			5000
#define MAX_MOB			5000
#define NPC_MIN_ID		101
#define NPC_MAX_ID		(NPC_MIN_ID+MAX_NPC)
#define MOB_MIN_ID		(NPC_MAX_ID+1)
#define MOB_MAX_ID		(MOB_MIN_ID+MAX_MOB)
#define USERS_START_ID	(MOB_MAX_ID+2)

#define MAX_ANSWERS		100

#define TALK_MAX_TIME	60000

const BYTE TALK_NPC_DISTANCE	=4;

//const
//Global map
const WORD GM_MAXX				=2100;
const WORD GM_MAXY				=2850;
const WORD GM_ZONE_LEN			=50; //������ ���� ������ GM_MAXX � GM_MAXY
const WORD GM_MAXZONEX			=GM_MAXX/GM_ZONE_LEN;
const WORD GM_MAXZONEY			=GM_MAXY/GM_ZONE_LEN;
const BYTE GM_MAX_GROUP			=21;
const DWORD GM_MOVE_1KM_TIME	=30000;
const DWORD GM_ZONE_TIME_PROC	=1000;

const BYTE GM_SPEED_SLOW		=0;
const BYTE GM_SPEED_NORM		=1;
const BYTE GM_SPEED_FAST		=2;

#define GM_ZONE(x) ((x)/GM_ZONE_LEN)

//Flags Tile (Hex)
#define FT_BLOCK		BIN16(00000000,00000001) //������������
#define FT_NOTRAKE		BIN16(00000000,00000010) //�����������������
#define FT_TRANSIT		BIN16(00000000,00000100) //����-�������
#define FT_PLAYER		BIN16(00000000,00001000) //�����, ���, ���

#define FT_TILECONT		BIN16(00000000,00010000) //����-���������
#define FT_CONT			BIN16(00000000,00100000) //����

#define FT_ACONT_CLOSE	BIN16(00000000,01000000) //������������� ���� ������
#define FT_ACONT_OPEN	BIN16(00000000,10000000) //������������� ���� ������

#define FT_DOOR_CLOSE	BIN16(00000001,00000000) //����� �������
#define FT_DOOR_OPEN	BIN16(00000010,00000000) //����� �������

#define FT_NOWAY		BIN16(00000001,11101001) //����� ��� ������������
#define FT_NOSHOOT		BIN16(00000001,00000010) //����� ��� �����������������

//�� ��� ������������ ����
const BYTE USE_OBJ_ON_CRITTER	=1;
const BYTE USE_OBJ_ON_OBJ		=2;

//��������� ����� ��� ��������
const DWORD ACTTIME_DROP_OBJ	=2000;
const DWORD ACTTIME_USE_DOOR	=3000;
const DWORD ACTTIME_USE_CONT	=3000;
const DWORD ACTTIME_PICK_OBJ	=6000;

//��������
const BYTE MOVE_ERROR		=0;
const BYTE MOVE_WALK		=1;
const BYTE MOVE_RUN			=2;

//move_params
const WORD PMOVE_0			=BIN16(00000000,00000000);
const WORD PMOVE_1			=BIN16(00000000,00000001);
const WORD PMOVE_2			=BIN16(00000000,00000010);
const WORD PMOVE_3			=BIN16(00000000,00000011);
const WORD PMOVE_4			=BIN16(00000000,00000100);
const WORD PMOVE_5			=BIN16(00000000,00000101);
const WORD PMOVE_RUN		=BIN16(10000000,00000000);

//����� � ����
const BYTE TIME_MULTIPLER	=8;

//����� ������ ��� �������
const BYTE NPC_SAY_NONE		=0;
const BYTE NPC_SAY_ERROR	=1;
const BYTE NPC_SAY_IMBYSY	=2;
const BYTE NPC_SAY_HELLO	=3;
const BYTE NPC_SAY_IMHAPPY	=4;
const BYTE NPC_SAY_ILOVEYOU	=5;
const BYTE NPC_SAY_LEAVEME	=6;
const BYTE NPC_SAY_FUCKOFF	=7;
const BYTE NPC_SAY_IHATEYOU	=8;

//��� ������
#define SAY_NORM	0
#define SAY_SHOUT	1
#define SAY_EMOTE	2
#define SAY_WHISP	3 
#define SAY_SOCIAL	4

//��������� ���������
const BYTE COND_LIFE				=1;//��������� �����
	const BYTE COND_LIFE_NONE			=1;//���������
	const BYTE COND_LIFE_ACTWEAP		=2;//���������
	const BYTE COND_LIFE_USEOBJ			=3;//���������
	const BYTE COND_LIFE_OFFLINE		=4;//���������
const BYTE COND_KNOCK_OUT			=2;//��������� �������
	const BYTE COND_KO_UP				=1;//��������� ������� ����� �����
	const BYTE COND_KO_DOWN				=2;//��������� ������� ���� �����
const BYTE COND_DEAD				=3;//��������� ������
	const BYTE COND_DEAD_NORMAL_UP		=1;//��������� ������
	const BYTE COND_DEAD_NORMAL_DOWN	=2;//��������� ������
	const BYTE COND_DEAD_CR_NORMAL_UP	=3;//��������� ������
	const BYTE COND_DEAD_BRUST			=4;//��������� ������
	const BYTE COND_DEAD_CR_BRUST		=5;//��������� ������
	const BYTE COND_DEAD_LASER			=6;//��������� ������
	const BYTE COND_DEAD_FIRE			=7;//��������� ������
	const BYTE COND_DEAD_PLASMA			=8;//��������� ������
	const BYTE COND_DEAD_ELECTR			=9;//��������� ������
	const BYTE COND_DEAD_EMP			=10;//��������� ������
	const BYTE COND_DEAD_EXPLODE		=11;//��������� ������
const BYTE COND_NOT_IN_GAME			=4;//��������� ��� ����

//����� ���������
#define FCRIT_PLAYER			BIN8(00000001)
#define FCRIT_NPC				BIN8(00000010)
#define FCRIT_MOB				BIN8(00000100)
#define FCRIT_DISCONNECT		BIN8(00001000)
#define FCRIT_CHOSEN			BIN8(00010000)
#define FCRIT_RULEGROUP			BIN8(00100000)

//������ ������
const BYTE SAY_SHORT		=1;//�������� ������� ��...
const BYTE SAY_SHORT_RANGE	=2;//...������(�)
const BYTE SAY_NORMAL		=2;//�������� ���������, ������ ��� ��� ���� �����
const BYTE SAY_ALL			=3;//�������� ������, ������ �� ��� �����

//������ �����
const BYTE TYPE_STAT		=1;
const BYTE TYPE_SKILL		=2;
const BYTE TYPE_PERK		=3;

//����� ���-�� ����������
const BYTE ALL_STATS		=39;
const BYTE ALL_SKILLS		=18;
const BYTE ALL_PERKS		=140;

//����� ����������
typedef map<string, BYTE, less<string> > params_map;
typedef map<BYTE, string, less<BYTE> > params_str_map;

//�����. Stats
const BYTE ST_STRENGHT				=0;//{����}
const BYTE ST_PERCEPTION			=1;//{����������}
const BYTE ST_ENDURANCE				=2;//{������������}
const BYTE ST_CHARISMA				=3;//{�������}
const BYTE ST_INTELLECT				=4;//{��}
const BYTE ST_AGILITY				=5;//{�����������}
const BYTE ST_LUCK					=6;//{�����}
//===========================
const BYTE ST_MAX_LIFE				=7;//{����. ����� �����}
const BYTE ST_MAX_COND				=8;//{������������ ��������}
const BYTE ST_ARMOR_CLASS			=9;//{����� �����}
const BYTE ST_MELEE_DAMAGE			=10;//{���� �������������}
const BYTE ST_WEAPON_DAMAGE			=11;//{���� �������}
const BYTE ST_CARRY_WEIGHT			=12;//{����. ����}
const BYTE ST_SEQUENCE				=13;//{�������}
const BYTE ST_HEALING_RATE			=14;//{�������}
const BYTE ST_CRITICAL_CHANCE		=15;//{���� �����������.}
const BYTE ST_MAX_CRITICAL			=16;//{������������ �����������}
const BYTE ST_INGURE_ABSORB			=17;//{����� �������}
const BYTE ST_LASER_ABSORB			=18;//{����� ����������� �������}
const BYTE ST_FIRE_ABSORB			=19;//{����� ����������� �����}
const BYTE ST_PLASMA_ABSORB			=20;//{����� ����������� �������}
const BYTE ST_ELECTRO_ABSORB		=21;//{����� ����������� ��������������}
const BYTE ST_EMP_ABSORB			=22;//{����� ����������� EMP}
const BYTE ST_BLAST_ABSORB			=23;//{����� ����������� ��� ������}
const BYTE ST_INGURE_RESIST			=24;//{���������������� �������}
const BYTE ST_LASER_RESIST			=25;//{���������������� ������� �������}
const BYTE ST_FIRE_RESIST			=26;//{���������������� ������� �����}
const BYTE ST_PLASMA_RESIST			=27;//{���������������� ������� �������}
const BYTE ST_ELECTRO_RESIST		=28;//{���������������� ������� ��������������}
const BYTE ST_EMP_RESIST			=29;//{���������������� ������� EMP}
const BYTE ST_BLAST_RESIST			=30;//{���������������� ������� ��� ������}
const BYTE ST_RADIATION_RESISTANCE	=31;//{���������������� ��������}
const BYTE ST_POISON_RESISTANCE		=32;//{���������������� ����}
const BYTE ST_AGE					=33;//{�������}
const BYTE ST_GENDER				=34;//{���}
const BYTE ST_CURRENT_HP			=35;//{������� ����� �����}
const BYTE ST_POISONING_LEVEL		=36;//{������� ������� ����}
const BYTE ST_RADIATION_LEVEL		=37;//{������� ������� ��������}
const BYTE ST_CURRENT_STANDART		=38;//{������� ��������}

//������. Skills
const BYTE SK_SMALL_GUNS			=0;//{������ ������}
const BYTE SK_BIG_GUNS				=1;//{������� ������}
const BYTE SK_ENERGY_WEAPONS		=2;//{�������������� ������}
const BYTE SK_UNARMED				=3;//{�� ��������}
const BYTE SK_MELEE_WEAPONS			=4;//{�������� ������}
const BYTE SK_THROWING				=5;//{������}
const BYTE SK_FIRST_AID				=6;//{������}
const BYTE SK_DOCTOR				=7;//{����}
const BYTE SK_SNEAK					=8;//{����������}
const BYTE SK_LOCKPICK				=9;//{��������}
const BYTE SK_STEAL					=10;//{������}
const BYTE SK_TRAPS					=11;//{�������}
const BYTE SK_SCIENCE				=12;//{�����}
const BYTE SK_REPAIR				=13;//{������}
const BYTE SK_SPEECH				=14;//{����}
const BYTE SK_BARTER				=15;//{�����} ��������� �� ���
const BYTE SK_GAMBLING				=16;//{������} ��������� �� ���
const BYTE SK_OUTDOORSMAN			=17;//{������ �������� ����}

//�����. Perks
const BYTE PE_FAST_METABOLISM		=0;//{����� �������}
const BYTE PE_BRUISER				=1;//{�����}
const BYTE PE_SMALL_FRAME			=2;//{"���������"}
const BYTE PE_ONE_HANDER			=3;//{���������}
const BYTE PE_FINESSE				=4;//{��������}
const BYTE PE_KAMIKAZE				=5;//{���������}
const BYTE PE_HEAVY_HANDED			=6;//{������� ����}
const BYTE PE_FAST_SHOT				=7;//{������� �������}
const BYTE PE_BLOODY_MESS			=8;//{�������� ������}
const BYTE PE_JINXED				=9;//{������������}
const BYTE PE_GOOD_NATURED			=10;//{�����������}
const BYTE PE_CHEM_RELIANT			=11;//{����������}
const BYTE PE_CHEM_RESISTANT		=12;//{����������}
const BYTE PE_NIGHT_PERSON			=13;//{��������������}  # was: Night Person
const BYTE PE_SKILLED				=14;//{������}
const BYTE PE_GIFTED				=15;//{���������}
const BYTE PE_AWARENESS				=16;//{���������������}
const BYTE PE_A_MELEE_ATT			=17;//{���. ���������� �����}
const BYTE PE_A_MELEE_DAM			=18;//{���. ���� � ����������}
const BYTE PE_A_MOVE				=19;//{���. ��������}
const BYTE PE_A_DAM					=20;//{���. �����������}
const BYTE PE_A_SPEED				=21;//{���. �������� ���������}
//-----------------------------------
const BYTE PE_PASS_FRONT			=22;//{���������� ������}
const BYTE PE_RAPID_HEAL			=23;//{������� �������}
const BYTE PE_MORE_CRIT_DAM			=24;//{������ ����������� ���������}
const BYTE PE_NIGHT_SIGHT			=25;//{������ �������}
const BYTE PE_PRESENCE				=26;//{�����������}
const BYTE PE_RES_NUKLEAR			=27;//{������������� ��������}
const BYTE PE_ENDURENCE				=28;//{������������}
const BYTE PE_STR_BACK				=29;//{������� �����}
const BYTE PE_MARKSMAN				=30;//{������ �������}
const BYTE PE_STEALHING				=31;//{��������� ���}
const BYTE PE_LIFEFULL				=32;//{�������}
const BYTE PE_MERCHANT				=33;//{��������������}
const BYTE PE_FORMED				=34;//{������������}
const BYTE PE_HEALER				=35;//{������}
const BYTE PE_TR_DIGGER				=36;//{�������������}
const BYTE PE_BEST_HITS				=37;//{������ �����}
const BYTE PE_COMPASION				=38;//{����������}
const BYTE PE_KILLER				=39;//{������}
const BYTE PE_SNIPER				=40;//{�������}
const BYTE PE_SILENT_DEATH			=41;//{���������� ������}
const BYTE PE_C_FIGHTER				=42;//{��������� ����}
const BYTE PE_MIND_BLOCK			=43;//{���������� ��������}
const BYTE PE_PROLONGATION_LIFE		=44;//{��������� �����}
const BYTE PE_RECOURCEFULNESS		=45;//{��������������}
const BYTE PE_SNAKE_EATER			=46;//{���������� ����}
const BYTE PE_REPEARER				=47;//{���������}
const BYTE PE_MEDIC					=48;//{�����}
const BYTE PE_SKILLED_THIEF			=49;//{������ ���}
const BYTE PE_SPEAKER				=50;//{������}
const BYTE PE_GUTCHER				=51;//{�����!}
const BYTE PE_UNKNOWN_1				=52;//{}  # was -- Friendly Foe
const BYTE PE_PICK_POCKER			=53;//{���������}
const BYTE PE_GHOST					=54;//{�������}
const BYTE PE_CHAR_CULT				=55;//{����� ��������}
const BYTE PE_THIFER				=56;//{�������}
const BYTE PE_DISCOVER				=57;//{�������������}
const BYTE PE_THE_PURETY			=58;//{���� �������}
const BYTE PE_OVERROAD				=59;//{���������}
const BYTE PE_ANIMAL_FRIENDSHIP		=60;//{���� ��������}
const BYTE PE_SCOUT					=61;//{�����}
const BYTE PE_MIST_CHAR				=62;//{������������ ����������}
const BYTE PE_RANGER				=63;//{��������}
const BYTE PE_PICK_POCKET_2			=64;//{���������}
const BYTE PE_INTERLOCUTER			=65;//{����������}
const BYTE PE_NOVICE				=66;//{��������� ������}
const BYTE PE_PRIME_SKILL			=67;//{�������� ������!}
const BYTE PE_MUTATION				=68;//{�������!}
const BYTE PE_NARC_NUKACOLA			=69;//{����������� � NukaCola}
const BYTE PE_NARC_BUFFOUT			=70;//{����������� � �������}
const BYTE PE_NARC_MENTAT			=71;//{����������� � �������}
const BYTE PE_NARC_PSYHO			=72;//{����������� � �����}
const BYTE PE_NARC_RADAWAY			=73;//{����������� �������}
const BYTE PE_DISTANT_WEAP			=74;//{������������ ������}
const BYTE PE_ACCURARY_WEAP			=75;//{������ ������}
const BYTE PE_PENETRATION_WEAP		=76;//{����������� ������}
const BYTE PE_KILLER_WEAP			=77;//{������� ������}
const BYTE PE_ENERGY_ARMOR			=78;//{�������������� �����}
const BYTE PE_BATTLE_ARMOR			=79;//{������ �����}
const BYTE PE_WEAP_RANGE			=80;//{��������������}
const BYTE PE_RAPID_RELOAD			=81;//{������� �����������}
const BYTE PE_NIGHT_SPYGLASS		=82;//{������ � ������ ��������}
const BYTE PE_FLAMER				=83;//{�������}
const BYTE PE_APA_I					=84;//{���������� ����� I}
const BYTE PE_APA_II				=85;//{���������� ����� II}
const BYTE PE_FORCEAGE				=86;//{������ ��������}
const BYTE PE_DEADLY_NARC			=87;//{����������� �����������}
const BYTE PE_CHARMOLEANCE			=88;//{������� }
const BYTE PE_GEKK_SKINER			=89;//{������� �����}
const BYTE PE_SKIN_ARMOR			=90;//{����-�����}
const BYTE PE_A_SKIN_ARMOR			=91;//{��������� ����- �����.}
const BYTE PE_SUPER_ARMOR			=92;//{����� �����}
const BYTE PE_A_SUPER_ARMOR			=93;//{��������� ����� �����}
const BYTE PE_VAULT_INOCUL			=94;//{�������� �����}
const BYTE PE_ADRENALIN_RUSH		=95;//{������ ����������}
const BYTE PE_CAREFULL				=96;//{������������}
const BYTE PE_INTELEGENCE			=97;//{���������}
const BYTE PE_PYROKASY				=98;//{���������}
const BYTE PE_DUDE					=99;//{�����}
//==========================
const BYTE PE_A_STR					=100;//{������ ����}
const BYTE PE_A_PER					=101;//{������ ����������}
const BYTE PE_A_END					=102;//{������ ������������}
const BYTE PE_A_CHA					=103;//{������ �������}
const BYTE PE_A_INT					=104;//{������ ���}
const BYTE PE_A_AGL					=105;//{������ ��������}
const BYTE PE_A_LUC					=106;//{������ �����}
//==========================
const BYTE PE_PURERER				=107;//{���� ����������}
const BYTE PE_IMAG					=108;//{����������}
const BYTE PE_EVASION				=109;//{���������}
const BYTE PE_DROSHKADRAT			=110;//{���������� �� ����- �����}
const BYTE PE_KARMA_GLOW			=111;//{�������� �����}
const BYTE PE_SILENT_STEPS			=112;//{������ ����}
const BYTE PE_ANATOMY				=113;//{��������}
const BYTE PE_CHAMER				=114;//{�������������� ��������}
const BYTE PE_ORATOR				=115;//{������}
const BYTE PE_PACKER				=116;//{���������}
const BYTE PE_EDD_GAYAN_MANIAC		=117;//{������- ������}
const BYTE PE_FAST_REGENERATION		=118;//{������� �������������}
const BYTE PE_VENDOR				=119;//{��������}
const BYTE PE_STONE_WALL			=120;//{�������� �����}
const BYTE PE_THIEF_AGAIN			=121;//{���}
const BYTE PE_WEAPON_SKILL			=122;//{�������� �������}
const BYTE PE_MAKE_VAULT			=123;//{���������� �����}
const BYTE PE_ALC_BUFF_1			=124;//{�������� �������� ����� �����}
const BYTE PE_ALC_BUFF_2			=125;//{�������� �������� ����� �����}

const BYTE PE_1				=126;//{�������� �������� ����� �����}
const BYTE PE_2				=127;//{�������� �������� ����� ����� II}
const BYTE PE_3				=128;//{������� ������� ����� �����}
const BYTE PE_4				=129;//{������� ������� ����� ����� II}
const BYTE PE_5				=130;//{������� ������� ����� �����}
const BYTE PE_6				=131;//{������� ������� ����� ����� II}
const BYTE PE_7				=132;//{��������� ��������}
const BYTE PE_8				=133;//{������ ��������� ������� ����}
const BYTE PE_9				=134;//{������������}
const BYTE PE_10			=135;//{�������� (������� �����)}
const BYTE PE_11			=136;//{������ (������)}
const BYTE PE_12			=137;//{����� ������ ����}
const BYTE PE_13			=138;//{����� ����� ����}

const BYTE PE_HIDE_MODE		=139;//{��������� ���������� (����/���)}

//���� �����������
const BYTE DAMAGE_TYPE_NORMAL	=0;
const BYTE DAMAGE_TYPE_LASER	=1;
const BYTE DAMAGE_TYPE_FIRE		=2;
const BYTE DAMAGE_TYPE_ELECTR	=3;
const BYTE DAMAGE_TYPE_EMP		=4;
const BYTE DAMAGE_TYPE_EXPLODE	=5;

//��������
//����
const BYTE OBJ_TYPE_ARMOR		=0;
const BYTE OBJ_TYPE_CONTAINER	=1;
const BYTE OBJ_TYPE_DRUG		=2;
const BYTE OBJ_TYPE_WEAPON		=3;
const BYTE OBJ_TYPE_AMMO		=4;
const BYTE OBJ_TYPE_MISC		=5;
const BYTE OBJ_TYPE_KEY			=6;
const BYTE OBJ_TYPE_DOOR		=7;
const BYTE OBJ_TYPE_CRAFTING	=8;
const BYTE OBJ_TYPE_GRID		=9;

const BYTE SOBJ_MAX_PARAMS		=80;
//=============================================== GENERAL
const BYTE OBJ_NAME				=0;//{��� �����}
const BYTE OBJ_INFO				=1;//{���� �����}

const BYTE OBJ_TIME_SHOW		=2;//{����� ���������� �������}
const BYTE OBJ_TIME_HIDE		=3;//{����� ��������� �������}

const BYTE OBJ_DISTANCE_LIGHT	=4;//{��������� �����}
const BYTE OBJ_INTENSITY_LIGHT	=5;//{������������� �����}

const BYTE OBJ_PASSED			=6;//{������������ �������}
const BYTE OBJ_RAKED			=7;//{����������������� �������}
const BYTE OBJ_TRANSPARENT		=8;//{������������ �������} wall, glass, steam, energy
const BYTE OBJ_CAN_USE			=9;//{������ ����� ������������}
const BYTE OBJ_CAN_PICK_UP		=10;//{������ ����� �������}
const BYTE OBJ_CAN_USE_ON_SMTH	=11;//{������ ����� ��������� �� ��� ����}

const BYTE OBJ_HIDDEN			=12;//{�������}

const BYTE OBJ_WEIGHT			=13;//{���}
const BYTE OBJ_SIZE				=14;//{����� � ���������}
const BYTE OBJ_TWOHANDS			=15;//{��������� ������}

const BYTE OBJ_PIC_MAP			=16;//{������ �������� �� �����}
const BYTE OBJ_ANIM_ON_MAP		=17;//{����� �������� �� ����� ���� ����}
const BYTE OBJ_PIC_INV			=18;//{������ �������� � ���������}
const BYTE OBJ_SOUND			=19;//{������ �����}

const BYTE OBJ_LIVETIME			=20;//{��������� ����� �����}

const BYTE OBJ_COST				=21;//{������� ����}
const BYTE OBJ_MATERIAL			=22;//{�������� ��������}
//----------------------------------------------- type==OBJ_TYPE_ARMOR
const BYTE OBJ_ARM_ANIM0_MALE	=30;//{������ �������� ���� male}
const BYTE OBJ_ARM_ANIM0_MALE2	=31;//{������ �������� ���� male ������}
const BYTE OBJ_ARM_ANIM0_FEMALE	=32;//{������ �������� ���� female}
const BYTE OBJ_ARM_ANIM0_FEMALE2=33;//{������ �������� ���� female ������}

const BYTE OBJ_ARM_AC			=40;//{����� �����}
const BYTE OBJ_ARM_PERK			=41;//{���� �� �����}

const BYTE OBJ_ARM_DR_NORMAL	=50;//{������������� ����������}
const BYTE OBJ_ARM_DR_LASER		=51;//{������������� ������}
const BYTE OBJ_ARM_DR_FIRE		=52;//{������������� ����}
const BYTE OBJ_ARM_DR_PLASMA	=53;//{������������� ������}
const BYTE OBJ_ARM_DR_ELECTR	=54;//{������������� �������������}
const BYTE OBJ_ARM_DR_EMP		=55;//{������������� ���}
const BYTE OBJ_ARM_DR_EXPLODE	=56;//{������������� ������}

const BYTE OBJ_ARM_DT_NORMAL	=60;//{����� ����������� ����������}
const BYTE OBJ_ARM_DT_LASER		=61;//{����� ����������� ������}
const BYTE OBJ_ARM_DT_FIRE		=62;//{����� ����������� ����}
const BYTE OBJ_ARM_DT_PLASMA	=63;//{����� ����������� ������}
const BYTE OBJ_ARM_DT_ELECTR	=64;//{����� ����������� �������������}
const BYTE OBJ_ARM_DT_EMP		=65;//{����� ����������� ���}
const BYTE OBJ_ARM_DT_EXPLODE	=66;//{����� ����������� ������}
//----------------------------------------------- type==OBJ_TYPE_CONTAINER
const BYTE OBJ_CONT_SIZE		=30;//{����� ����������}
const BYTE OBJ_CONT_FLAG		=31;//{���� ����������}
//----------------------------------------------- type==OBJ_TYPE_DRUG
const BYTE OBJ_DRUG_STAT0		=30;//{}
const BYTE OBJ_DRUG_STAT1		=31;//{}
const BYTE OBJ_DRUG_STAT2		=32;//{}

const BYTE OBJ_DRUG_AMOUNT0_S0	=40;//{}
const BYTE OBJ_DRUG_AMOUNT0_S1	=41;//{}
const BYTE OBJ_DRUG_AMOUNT0_S2	=42;//{}

const BYTE OBJ_DRUG_DURATION1	=50;//{}
const BYTE OBJ_DRUG_AMOUNT1_S0	=51;//{}
const BYTE OBJ_DRUG_AMOUNT1_S1	=52;//{}
const BYTE OBJ_DRUG_AMOUNT1_S2	=53;//{}

const BYTE OBJ_DRUG_DURATION2	=60;//{}
const BYTE OBJ_DRUG_AMOUNT2_S0	=61;//{}
const BYTE OBJ_DRUG_AMOUNT2_S1	=62;//{}
const BYTE OBJ_DRUG_AMOUNT2_S2	=63;//{}

const BYTE OBJ_DRUG_ADDICTION	=70;//{}
const BYTE OBJ_DRUG_W_EFFECT	=71;//{}
const BYTE OBJ_DRUG_W_ONSET		=72;//{}
//----------------------------------------------- type==OBJ_TYPE_WEAPON
const BYTE OBJ_WEAP_ANIM1		=30;//{������ �������� �������}

const BYTE OBJ_WEAP_TIME_ACTIV	=31;//{����� ���������� � �������������}
const BYTE OBJ_WEAP_TIME_UNACTIV=32;//{����� ������ �������������}

const BYTE OBJ_WEAP_VOL_HOLDER	=33;//{������� �������� ������}
const BYTE OBJ_WEAP_CALIBER		=34;//{������ � �������� ������}
const BYTE OBJ_WEAP_VOL_HOLDER_E=35;//{������� �������������� ������}
const BYTE OBJ_WEAP_CALIBER_E	=36;//{������ � �������������� ������}

const BYTE OBJ_WEAP_CR_FAILTURE	=37;//{����������� ������� ��� �������������}

const BYTE OBJ_WEAP_TYPE_ATTACK	=38;//{��� ����� ������}

const BYTE OBJ_WEAP_COUNT_ATTACK=39;//{���������� ����} | 0 - ��� | 1 - PA | 2 - PA && SA | 3 - PA && SA && TA |

const BYTE OBJ_WEAP_PA_SKILL	=40;//{����� ���������� �� �������������}
const BYTE OBJ_WEAP_PA_HOLDER	=41;//{������������ ������}
const BYTE OBJ_WEAP_PA_PIC		=42;//{������� �������������}
const BYTE OBJ_WEAP_PA_DMG_MIN	=43;//{����������� �����������}
const BYTE OBJ_WEAP_PA_DMG_MAX	=44;//{������������ �����������}
const BYTE OBJ_WEAP_PA_MAX_DIST	=45;//{������������ ���������}
const BYTE OBJ_WEAP_PA_EFF_DIST	=46;//{����������� ���������}
const BYTE OBJ_WEAP_PA_ANIM2	=47;//{�������� �����}
const BYTE OBJ_WEAP_PA_TIME		=48;//{������� ����� �����}
const BYTE OBJ_WEAP_PA_AIM		=49;//{������� � ����� ����������� ��������}
const BYTE OBJ_WEAP_PA_ROUND	=50;//{������ �������� �� �����}
const BYTE OBJ_WEAP_PA_REMOVE	=51;//{�������� �������� ����� �����}

const BYTE OBJ_WEAP_SA_SKILL	=53;//{����� ���������� �� �������������}
const BYTE OBJ_WEAP_SA_HOLDER	=54;//{������������ ������}
const BYTE OBJ_WEAP_SA_PIC		=55;//{������� �������������}
const BYTE OBJ_WEAP_SA_DMG_MIN	=56;//{����������� �����������}
const BYTE OBJ_WEAP_SA_DMG_MAX	=57;//{������������ �����������}
const BYTE OBJ_WEAP_SA_MAX_DIST	=58;//{������������ ���������}
const BYTE OBJ_WEAP_SA_EFF_DIST	=59;//{����������� ���������}
const BYTE OBJ_WEAP_SA_ANIM2	=60;//{�������� �����}
const BYTE OBJ_WEAP_SA_TIME		=61;//{������� ����� �����}
const BYTE OBJ_WEAP_SA_AIM		=62;//{������� � ����� ����������� ��������}
const BYTE OBJ_WEAP_SA_ROUND	=63;//{������ �������� �� �����}
const BYTE OBJ_WEAP_SA_REMOVE	=64;//{�������� �������� ����� �����}

const BYTE OBJ_WEAP_TA_SKILL	=66;//{����� ���������� �� �������������}
const BYTE OBJ_WEAP_TA_HOLDER	=67;//{������������ ������}
const BYTE OBJ_WEAP_TA_PIC		=68;//{������� �������������}
const BYTE OBJ_WEAP_TA_DMG_MIN	=69;//{����������� �����������}
const BYTE OBJ_WEAP_TA_DMG_MAX	=70;//{������������ �����������}
const BYTE OBJ_WEAP_TA_MAX_DIST	=71;//{������������ ���������}
const BYTE OBJ_WEAP_TA_EFF_DIST	=72;//{����������� ���������}
const BYTE OBJ_WEAP_TA_ANIM2	=73;//{�������� �����}
const BYTE OBJ_WEAP_TA_TIME		=74;//{������� ����� �����}
const BYTE OBJ_WEAP_TA_AIM		=75;//{������� � ����� ����������� ��������}
const BYTE OBJ_WEAP_TA_ROUND	=76;//{������ �������� �� �����}
const BYTE OBJ_WEAP_TA_REMOVE	=77;//{�������� �������� ����� �����}
//----------------------------------------------- type==OBJ_TYPE_AMMO
const BYTE OBJ_AMMO_CALIBER		=30;//{������}
const BYTE OBJ_AMMO_TYPE_DAMAGE	=31;//{��� �����������}
const BYTE OBJ_AMMO_QUANTITY	=32;//{����������}
const BYTE OBJ_AMMO_AC			=33;//{AC}
const BYTE OBJ_AMMO_DR			=34;//{DR}
const BYTE OBJ_AMMO_DM			=35;//{��������� ��� DR}
const BYTE OBJ_AMMO_DD			=36;//{�������� ��� DR}
//----------------------------------------------- type==OBJ_TYPE_MISC

//----------------------------------------------- type==OBJ_TYPE_KEY
//const BYTE OBJ_KEY_DOOR			=30;//{�� ����� ����� ����}
//----------------------------------------------- OBJ_TYPE_DOOR
const BYTE OBJ_DOOR_ANIMATION		=30;//{�������� �����}
//----------------------------------------------- OBJ_TYPE_CRAFTING

//----------------------------------------------- type==OBJ_TYPE_CRAFTING
/*
crafting
=======================================
time_crafting

chance

item0
item1
item2
item3
item4
item5
item6
item7
item8
item9
item10
item11
item12
item13
item14
item15
item16
item17
item18
item19

stat0no
stat0count
stat1no
stat1count
stat2no
stat2count

skill0no
skill0count
skill1no
skill1count
skill2no
skill2count

perk0no
perk1no
perk2no
perk3no
perk4no
*/
//===============================================

//������� ���������� ������
const BYTE OBJ_USE_ON_CRITTER	=0;
const BYTE OBJ_USE_ON_ITEM		=0;
const BYTE OBJ_USE_ON_SCENERY	=0;

//!Cvet -------------------------------

//defines
#define MAX_NAME	20
#define MIN_NAME	4 //!Cvet
#define MAX_LOGIN	10
#define MIN_LOGIN	4 //!Cvet
#define MAX_TEXT	1024

#define MSGTYPE		BYTE
#define CrID		DWORD
#define CrTYPE		BYTE
#define HexTYPE		WORD //!Cvet
#define DHexTYPE	DWORD //!Cvet
#define MapTYPE		WORD //!Cvet
//!Cvet +++++++++++++++++++++++++++++++
#define TICK		DWORD

#define MAX_SCENERY 5000

const BYTE ACT_NULL				=1;
const BYTE ACT_SHOW_OBJ			=2;
const BYTE ACT_HIDE_OBJ			=3;
const BYTE ACT_ACTIVATE_OBJ		=4;
const BYTE ACT_DACTIVATE_OBJ	=5;
const BYTE ACT_USE_OBJ			=6;
const BYTE ACT_CHANGE_ARM		=7;
const BYTE ACT_DEFEAT			=8;
	const BYTE ACT_DEFEAT_MISS		=1;
	const BYTE ACT_DEFEAT_FRONT		=2;
	const BYTE ACT_DEFEAT_REAR		=3;
	const BYTE ACT_DEFEAT_KO_FRONT	=4;
	const BYTE ACT_DEFEAT_KO_REAR	=5;
const BYTE ACT_REFRESH			=9;
const BYTE ACT_DEAD				=10;
const BYTE ACT_CR_DEAD			=11;
const BYTE ACT_DISCONNECT		=12;
const BYTE ACT_DROP_OBJ			=13;
const BYTE ACT_PICK_OBJ_UP		=14;
const BYTE ACT_PICK_OBJ_DOWN	=15;

//typedef pair<BYTE,int> param_pair;
//typedef vector<param_pair> param_value;

//typedef vector<WORD*> type_of_use_vect;

struct stat_obj
{
	stat_obj():id(0),type(0){for(int i=0;i<SOBJ_MAX_PARAMS;i++)p[i]=0;};

	WORD id;
	BYTE type;

	int p[SOBJ_MAX_PARAMS];
};

///////////////////////////////////////////////////////////////////////////////////////////

//BYTE accessory
const BYTE DOBJ_ACCESSORY_CRIT		=1;
const BYTE DOBJ_ACCESSORY_HEX		=2;
const BYTE DOBJ_ACCESSORY_CONT		=3;

const BYTE DOBJ_SLOT_INV			=0;
const BYTE DOBJ_SLOT_HAND1			=1;
const BYTE DOBJ_SLOT_HAND2			=2;
const BYTE DOBJ_SLOT_TWOHAND		=3;
const BYTE DOBJ_SLOT_ARMOR			=4;

///////////////////////////////////////////////////////////////////////////////////////////

struct dyn_obj
{
//	dyn_obj():id(0),object(NULL),player(0),active(0),map(0),x(0),y(0),tick(0),last_tick(0),
//		holder(0),id_bull(0),holder_ext(0),id_bull_ext(0),broken(0){};

	DWORD id;
	stat_obj* object;

	DWORD time_wear;
	DWORD last_tick;
	DWORD broken_info;

	BYTE accessory;
	union
	{
		struct
		{
			WORD map;
			HexTYPE x;
			HexTYPE y;
		} ACC_HEX;

		struct
		{
			CrID id;
			BYTE slot;
//			BYTE active;
		} ACC_CRITTER;

		struct
		{
			DWORD id;
		} ACC_CONTAINER;
	};

	BYTE type;
	union
	{
		struct
		{
			DWORD reserved;
		} ITEM_ARMOR;

		struct
		{
			DWORD reserved;
		} ITEM_CONTAINER;

		struct
		{
			DWORD reserved;
		} ITEM_DRUG;

		struct
		{
			WORD AmmoPID;      // �������� ���������� ��������.
			WORD AmmoCount;    // ���������� ���������� ��������
			WORD AmmoPID_ext;      // �������� ���������� ��������.
			WORD AmmoCount_ext;    // ���������� ���������� ��������
		} ITEM_WEAPON;

		struct
		{
			WORD  Count;        // ���-�� �������� � ������
		} ITEM_AMMO;

		struct
		{
			WORD  Count;        // ������� ���������� ������� (�.�., ��������� �������).
		} ITEM_MISC;

		struct
		{
			DWORD   doorID;     // ��� �����, ������� ��������� ����
		} ITEM_KEY;

		struct
		{
			DWORD   doorID;     // ��� �����
		} ITEM_DOOR;

		struct
		{
			DWORD   doorID;     // ��� �����
		} ITEM_CRAFTING;

		struct
		{
			DWORD   MapID;      // ����� �����, �� ������� �������� �����.
			WORD    PosX;       // ������� ������ �� ����� �����
			WORD    PosY;
			BYTE    dir;        // ���������� ������
		} ITEM_GRID;
	};
};

typedef map<DWORD, dyn_obj*, less<DWORD> > dyn_map;
typedef map<WORD, stat_obj*, less<WORD> > stat_map;

typedef set<WORD> list_ind;

//!Cvet -------------------------------

struct crit_info
{
	crit_info():a_obj(&def_obj1),a_obj_arm(&def_obj2),base_type(0),map(0),x(0),y(0),ori(0),look(0),
		start_bt(0),break_time(0),id(0),idchannel(0),cond(COND_NOT_IN_GAME),cond_ext(0),flags(0),
		access(0),world_x(100),world_y(100){};

	CrTYPE base_type; //������� ��� ��������

	MapTYPE map; //����� �����
	HexTYPE x; //������� � ������
	HexTYPE y;

	WORD world_x;
	WORD world_y;

	BYTE ori; //����������
	BYTE look; //!Cvet ��������� ���������
	char name[MAX_NAME+1]; //���
	char cases[5][MAX_NAME+1]; //��� ���������� ��������

	BYTE access; //!Cvet ������� ������� � ����

	DWORD start_bt; //!Cvet
	int break_time; //!Cvet

	CrID id; //!Cvet �������������
	CrID idchannel; //!Cvet ������������� ������ � �������� ��������� �����
	char login[MAX_LOGIN+1]; //!Cvet �����
	char pass[MAX_LOGIN+1]; //!Cvet ������
	WORD st[ALL_STATS ]; //!Cvet ����� 4-� ������� XXXX
	WORD sk[ALL_SKILLS]; //!Cvet ����� 3-� ������� XXX
	BYTE pe[ALL_PERKS ]; //!Cvet ����� 1-� ������� X

	BYTE cond; //!Cvet ��������� ��������
	BYTE cond_ext; //!Cvet �������������� ��������� ��������
	WORD flags; //!Cvet ����� ��������

	list_ind obj_id;

	//���� �� ��������
	dyn_map obj; //!Cvet ����� ������������ ��������
	//int Slot1, Slot2, ArmorSlot; 
	dyn_obj* a_obj; //�������� ������ � �����
	dyn_obj* a_obj_arm; //�������� ������ � ����� �����

	dyn_obj def_obj1;
	dyn_obj def_obj2;
};

#define NETMSG_LOGIN 1
//////////////////////////////////////////////////////////////////////////
// ������ ��� ����������� ���� ��� ���������, ������� ����� � ������
// params:
// char login[MAX_LOGIN]
// char pass[MAX_LOGIN]
//////////////////////////////////////////////////////////////////////////

#define NETMSG_LOGINOK 2
//////////////////////////////////////////////////////////////////////////
// ����� ������� �� NETMSG_NAME - ����� � ������ ������
// params:
//////////////////////////////////////////////////////////////////////////

#define NETMSG_CREATE_CLIENT 3 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ ������� �� �������� ��������
// params:
// char login[MAX_LOGIN]; //!Cvet �����
// char pass[MAX_LOGIN]; //!Cvet ������
// char name[MAX_NAME]; // ���
// char cases[5][MAX_NAME]; // ��� ���������� ��������
// CrTYPE base_type
// WORD s[MAX_STATS] //�����
//////////////////////////////////////////////////////////////////////////
#define NETMSG_LOGMSG 4
//////////////////////////////////////////////////////////////////////////
// ����� ������� �� NETMSG_NAME - ��� �� �������
// params:
// BYTE LogMsg
//////////////////////////////////////////////////////////////////////////

#define NETMSG_NAMEERR 5
//////////////////////////////////////////////////////////////////////////
// ����� ������� �� NETMSG_NAME - ��� �� �������
// params:
//////////////////////////////////////////////////////////////////////////

#define NETMSG_ADDCRITTER 6 //!Cvet edit
//////////////////////////////////////////////////////////////////////////
// �������� �������
// params:
// CrID id
// CrTYPE base_type
// WORD id_stat
// WORD id_stat_arm
// HexTYPE x
// HexTYPE y
// BYTE ori
// BYTE st[ST_GENDER] ���
// BYTE cond
// BYTE cond_ext
// WORD flags
// char[MAX_NAME] name
// char[5][MAX_NAME] cases
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#define MSG_ADDCRITTER_LEN (sizeof(CrID)+sizeof(CrTYPE)+5+MAX_NAME)
//////////////////////////////////////////////////////////////////////////
// ��������� ����� ������������ ��������

#define NETMSG_REMOVECRITTER 7
//////////////////////////////////////////////////////////////////////////
// ������ �������
// params:
// CrID id

#define NETMSG_TEXT 8
//////////////////////////////////////////////////////////////////////////
// ��������� ���������
// params:
// CrID crid
// BYTE how_say
// WORD len
// char[len] str

#define NETMSG_CRITTERTEXT 9
//////////////////////////////////////////////////////////////////////////
// ����� ������� ���� ���������� ��� ���������
// params:
// CrID id
// WORD len
// char[len] str

#define NETMSG_DIR 10 //!Cvet edit
//////////////////////////////////////////////////////////////////////////
// ��������� �����������
// params:
// BYTE dir

#define NETMSG_CRITTER_DIR 11 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ����������� ��������
// params:
// CrID id
// BYTE dir

#define NETMSG_SEND_MOVE 12
//////////////////////////////////////////////////////////////////////////
// �������� �� ������ ����������� ��� ���� ������
// params:
// BYTE dir
// BYTE move_params - dir,how_move,stop&go
#define NETMSG_CRITTER_MOVE 13
//////////////////////////////////////////////////////////////////////////
// �������� ����������� ��� ������ ���������
// params:
// CrID id
// BYTE move_params - dir,how_move,stop&go
// HexTYPE x
// HexTYPE y
#define NETMSG_QUIT 14
//////////////////////////////////////////////////////////////////////////
// ������ � ����������������� ������ �������
// params:
// CrID id

#define NETMSG_XY 15 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ � �������� ��������� ������
// params:
// HexTYPE Chex_x
// HexTYPE Chex_y
// BYTE dir

#define NETMSG_ALL_PARAMS 16 //!Cvet
//////////////////////////////////////////////////////////////////////////
// �������� ����������
// params:
// BYTE type_param - ��� ���������
// BYTE all_send - ����� ������ ����������
// BYTE num_param - ����� ���������
// WORD or BYTE count_param - ��� ���-��
#define NETMSG_PARAM 17 //!Cvet
//////////////////////////////////////////////////////////////////////////
// �������� ���������� ���������
// params:
// BYTE type_param - ��� ���������
// WORD num_param - ����� ���������
// WORD or BYTE count_param - ��� ���-��

#define NETMSG_ADD_OBJECT 18 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ � ��� ��� ����� ������� ������
// params:
// DWORD id; -dyn
// WORD id; -stat
// BYTE slot -0-inv 1-a_slot
// WORD holder;
// WORD id_bull;
// WORD holder_ext;
// WORD id_bull_ext;
// TICK tick;
// BYTE broken;
#define NETMSG_REMOVE_OBJECT 19 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ � ��� ��� ����� ������� ������
// params:
// DWORD id; -dyn
#define NETMSG_WEAR_OBJECT 20 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ � ������ ��������
// params:
// DWORD id; -dyn;
// TICK tick;
// BYTE broken;

#define NETMSG_ADD_OBJECT_ON_MAP 21 //!Cvet
//////////////////////////////////////////////////////////////////////////
// 
// params:
// HexTYPE x;
// HexTYPE y;
// BYTE id_stat;
// WORD tile_flags;
#define NETMSG_CHANGE_OBJECT_ON_MAP 22 //!Cvet
//////////////////////////////////////////////////////////////////////////
// 
// params:
// HexTYPE x;
// HexTYPE y;
// BYTE id_stat;
// WORD tile_flags;
#define NETMSG_REMOVE_OBJECT_FROM_MAP 23 //!Cvet
//////////////////////////////////////////////////////////////////////////
// 
// params:
// HexTYPE x;
// HexTYPE y;
// BYTE id_stat;

#define NETMSG_SEND_CHANGE_OBJECT 24 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ ������ � ���������� ��������� ��������
// params:

#define NETMSG_SEND_PICK_OBJECT 25 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ ������ � ���������� ��������� ��������
// params:

#define NETMSG_SEND_USE_OBJECT 26 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ ������ � ���������� ��������� ��������
// params:

#define NETMSG_SEND_USE_SKILL 27 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ � ���������� ������
// params:
// BYTE skill; -����� ������
// DWORD target_id; -����� ��� �������
// BYTE ori; -����������� ����������

#define NETMSG_CRITTER_ACTION 28 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ � ��� ��� ������� ���������� ������� ��������
// params:
// CrID crid; -�� ��������
// BYTE ori; -����������� ��������
// BYTE type_action; -����� �������� -��������� �����
// BYTE type; -��� �������� -�������� ��������
// BYTE weap; -����� ���� ������ � ����� -������������� �����
// WORD number_of_action; -��� �� �������� ��������

#define NETMSG_SEND_TALK_NPC 29 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ ������ � ������
// CrID id_talk - �� ��� � ������� ��������������
// BYTE variant - ������� ������
#define NETMSG_TALK_NPC 30 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ����� ��� -> ������
// BYTE all_answers - ����� ��������� ������, ���� 0 - ������ ��������
// DWORD main_text - ����� ���
// DWORD answ_text � ���-�� all_answers - �������� ������

#define NETMSG_SEND_GET_TIME 31 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ �� ������� �����
#define NETMSG_GAME_TIME 32 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ���������� ������ ���������� ������� ����� � ������ ������� �����
// WORD minutes - ���������� ������� �����
// BYTE day - ������� ����
// BYTE month - ������� �����
// WORD year - ������� ���

#define NETMSG_LOADMAP 33 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ���������� ������ ������� �������� �����
// WORD num_map - ����� �����
// WORD num_version - ����� ������ �����
// �� ������� !!!!!!!!
// HexTYPE width - ������ �� �
// HexTYPE height - ������ �� �
#define NETMSG_MAP 34 //!Cvet
//////////////////////////////////////////////////////////////////////////
// �����
// WORD num_map - ����� �����
#define NETMSG_SEND_GIVE_ME_MAP 35 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ �� ������� �����
// WORD num_map - ����� �����
#define NETMSG_SEND_LOAD_MAP_OK 36 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ��������� � ������� �������� �����

//GLOBAL MAP
const BYTE GM_INFO_CITIES	=BIN8(00000001);
const BYTE GM_INFO_CRITS	=BIN8(00000010);
const BYTE GM_INFO_PARAM	=BIN8(00000100);
const BYTE GM_INFO_ALL		=BIN8(00000111);
//const BYTE GM_ADDINFO_CITY	=BIN8(00001000);
//const BYTE GM_DELINFO_CITY	=BIN8(00010000);
#define NETMSG_SEND_GIVE_GLOBAL_INFO 37 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ������ ���� �� �������
// BYTE info_flag;
#define NETMSG_GLOBAL_INFO 38 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ���� �� �������
// BYTE info_flag;
// info_flag & GM_INFO_CITIES || info_flag & GM_INFO_ALL
//		WORD count_cities;
//			for count_cities
//			WORD city_num
//			WORD city_x
//			WORD city_y
//			BYTE city_radius
// info_flag & GM_ADDINFO_CITY
//			WORD city_num
//			WORD city_x
//			WORD city_y
//			BYTE city_radius
// info_flag & GM_DELINFO_CITY
//			WORD city_num
// info_flag & GM_INFO_CRITS || info_flag & GM_INFO_ALL
//		BYTE count_group;
//			for count_group
//			CrID id_crit;
//			char[MAX_NAME] name;
//			WORD flags_crit;
// info_flag & GM_INFO_PARAM || info_flag & GM_INFO_ALL
//		WORD group_x;
//		WORD group_y;
//		WORD move_x;
//		WORD move_y;
//		BYTE speed;
//		int speed_x;
//		int speed_y;
// BYTE 0xAA - end_info
const BYTE GM_RULE_COMMAND_SETMOVE		=1; //+r-a*x,y//���������
const BYTE GM_RULE_COMMAND_STOP			=2; //+r-a//������������
const BYTE GM_RULE_COMMAND_TOLOCAL		=3; //+r-a*num_city,num_map//������� �� �������� �����
const BYTE GM_RULE_COMMAND_KICKCRIT		=4; //+r-a*cr_id//�������� ����� �� ������
const BYTE GM_RULE_COMMAND_LOCKCRIT		=5; //+r-a*cr_id//����������� �����
const BYTE GM_RULE_COMMAND_UNLOCKCRIT	=6; //+r-a*cr_id//�������������� �����
const BYTE GM_RULE_COMMAND_IGNORECRIT	=7; //+r+a*cr_id//������������ �����
const BYTE GM_RULE_COMMAND_LISTENCRIT	=8; //+r+a*cr_id//������� �����
const BYTE GM_RULE_COMMAND_READY		=9; //+r+a//���������� � ������� �� ��������� �����
const BYTE GM_RULE_COMMAND_TOGEMMATE	=10;//-r+a//������������� �� ������
const BYTE GM_RULE_COMMAND_SETSPEED		=11;//+r-a*speed//������ �������� ������
const BYTE GM_RULE_COMMAND_ADDPREPCRIT	=12;//+r-a*cr_id//������� � ������ �����
const BYTE GM_RULE_COMMAND_DELPREPCRIT	=13;//+r-a*cr_id//������� �� ������ �����
#define NETMSG_SEND_RULE_GLOBAL 39 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ���������� �������
// BYTE command;
// DWORD param1;
// DWORD param2;

//states

#define STATE_CONN			0
#define STATE_DROPLINK		1
#define STATE_DISCONNECT	2
#define STATE_GAME			3

//������ ����� � ������ ������
#define STATE_LOGINOK		5 //!Cvet

#define STATE_INIT_NET		6


#endif //__NETPROTO_H__