#ifndef __NETPROTO_H__
#define __NETPROTO_H__

#include <set>
#include <map>

#include <stdint.h>

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

const uint8_t TALK_NPC_DISTANCE	=4;

//const
//Global map
const uint16_t GM_MAXX				=2100;
const uint16_t GM_MAXY				=2850;
const uint16_t GM_ZONE_LEN			=50; //должна быть кратна GM_MAXX и GM_MAXY
const uint16_t GM_MAXZONEX			=GM_MAXX/GM_ZONE_LEN;
const uint16_t GM_MAXZONEY			=GM_MAXY/GM_ZONE_LEN;
const uint8_t GM_MAX_GROUP			=21;
const uint32_t GM_MOVE_1KM_TIME	=30000;
const uint32_t GM_ZONE_TIME_PROC	=1000;

const uint8_t GM_SPEED_SLOW		=0;
const uint8_t GM_SPEED_NORM		=1;
const uint8_t GM_SPEED_FAST		=2;

#define GM_ZONE(x) ((x)/GM_ZONE_LEN)

//Flags Tile (Hex)
#define FT_BLOCK		BIN16(00000000,00000001) //проходимость
#define FT_NOTRAKE		BIN16(00000000,00000010) //простреливаемость
#define FT_TRANSIT		BIN16(00000000,00000100) //хекс-переход
#define FT_PLAYER		BIN16(00000000,00001000) //игрок, нпц, моб

#define FT_TILECONT		BIN16(00000000,00010000) //хекс-контейнер
#define FT_CONT			BIN16(00000000,00100000) //ящик

#define FT_ACONT_CLOSE	BIN16(00000000,01000000) //анимированный ящик закрыт
#define FT_ACONT_OPEN	BIN16(00000000,10000000) //анимированный ящик открыт

#define FT_DOOR_CLOSE	BIN16(00000001,00000000) //дверь закрыта
#define FT_DOOR_OPEN	BIN16(00000010,00000000) //дверь открыта

#define FT_NOWAY		BIN16(00000001,11101001) //комбо для проходимости
#define FT_NOSHOOT		BIN16(00000001,00000010) //комбо для простреливаемости

//на чем используется итем
const uint8_t USE_OBJ_ON_CRITTER	=1;
const uint8_t USE_OBJ_ON_OBJ		=2;

//дефолтное время для действий
const uint32_t ACTTIME_DROP_OBJ	=2000;
const uint32_t ACTTIME_USE_DOOR	=3000;
const uint32_t ACTTIME_USE_CONT	=3000;
const uint32_t ACTTIME_PICK_OBJ	=6000;

//движение
const uint8_t MOVE_ERROR		=0;
const uint8_t MOVE_WALK		=1;
const uint8_t MOVE_RUN			=2;

//move_params
const uint16_t PMOVE_0			=BIN16(00000000,00000000);
const uint16_t PMOVE_1			=BIN16(00000000,00000001);
const uint16_t PMOVE_2			=BIN16(00000000,00000010);
const uint16_t PMOVE_3			=BIN16(00000000,00000011);
const uint16_t PMOVE_4			=BIN16(00000000,00000100);
const uint16_t PMOVE_5			=BIN16(00000000,00000101);
const uint16_t PMOVE_RUN		=BIN16(10000000,00000000);

//время в игре
const uint8_t TIME_MULTIPLER	=8;

//общие ответы над головой
const uint8_t NPC_SAY_NONE		=0;
const uint8_t NPC_SAY_ERROR	=1;
const uint8_t NPC_SAY_IMBYSY	=2;
const uint8_t NPC_SAY_HELLO	=3;
const uint8_t NPC_SAY_IMHAPPY	=4;
const uint8_t NPC_SAY_ILOVEYOU	=5;
const uint8_t NPC_SAY_LEAVEME	=6;
const uint8_t NPC_SAY_FUCKOFF	=7;
const uint8_t NPC_SAY_IHATEYOU	=8;

//тип ответа
#define SAY_NORM	0
#define SAY_SHOUT	1
#define SAY_EMOTE	2
#define SAY_WHISP	3 
#define SAY_SOCIAL	4

//состояния криттеров
const uint8_t COND_LIFE				=1;//состояние жизни
	const uint8_t COND_LIFE_NONE			=1;//состояние
	const uint8_t COND_LIFE_ACTWEAP		=2;//состояние
	const uint8_t COND_LIFE_USEOBJ			=3;//состояние
	const uint8_t COND_LIFE_OFFLINE		=4;//состояние
const uint8_t COND_KNOCK_OUT			=2;//состояние накаута
	const uint8_t COND_KO_UP				=1;//состояние накаута вверх лицом
	const uint8_t COND_KO_DOWN				=2;//состояние накаута вниз лицом
const uint8_t COND_DEAD				=3;//состояние смерти
	const uint8_t COND_DEAD_NORMAL_UP		=1;//состояние смерти
	const uint8_t COND_DEAD_NORMAL_DOWN	=2;//состояние смерти
	const uint8_t COND_DEAD_CR_NORMAL_UP	=3;//состояние смерти
	const uint8_t COND_DEAD_BRUST			=4;//состояние смерти
	const uint8_t COND_DEAD_CR_BRUST		=5;//состояние смерти
	const uint8_t COND_DEAD_LASER			=6;//состояние смерти
	const uint8_t COND_DEAD_FIRE			=7;//состояние смерти
	const uint8_t COND_DEAD_PLASMA			=8;//состояние смерти
	const uint8_t COND_DEAD_ELECTR			=9;//состояние смерти
	const uint8_t COND_DEAD_EMP			=10;//состояние смерти
	const uint8_t COND_DEAD_EXPLODE		=11;//состояние смерти
const uint8_t COND_NOT_IN_GAME			=4;//состояние вне игры

//флаги криттеров
#define FCRIT_PLAYER			BIN8(00000001)
#define FCRIT_NPC				BIN8(00000010)
#define FCRIT_MOB				BIN8(00000100)
#define FCRIT_DISCONNECT		BIN8(00001000)
#define FCRIT_CHOSEN			BIN8(00010000)
#define FCRIT_RULEGROUP			BIN8(00100000)

//режимы голоса
const uint8_t SAY_SHORT		=1;//говорить шепетом на...
const uint8_t SAY_SHORT_RANGE	=2;//...клетку(и)
const uint8_t SAY_NORMAL		=2;//говорить нормально, слышат все кто тебя видит
const uint8_t SAY_ALL			=3;//говорить громко, слышно на всю карту

//номера групп
const uint8_t TYPE_STAT		=1;
const uint8_t TYPE_SKILL		=2;
const uint8_t TYPE_PERK		=3;

//общее кол-во параметров
const uint8_t ALL_STATS		=39;
const uint8_t ALL_SKILLS		=18;
const uint8_t ALL_PERKS		=140;

class CompareCharPtr
{
public:
  bool operator() (const char *s, const char *t)const
  {
    return strcmp(s,t)<0;
  }
};

//карта параметров
typedef std::map<char*, uint8_t, CompareCharPtr> params_map;
typedef std::map<uint8_t, std::string> params_str_map;

//Статы. Stats
const uint8_t ST_STRENGHT				=0;//{Сила}
const uint8_t ST_PERCEPTION			=1;//{Восприятие}
const uint8_t ST_ENDURANCE				=2;//{Выносливость}
const uint8_t ST_CHARISMA				=3;//{Обаяние}
const uint8_t ST_INTELLECT				=4;//{Ум}
const uint8_t ST_AGILITY				=5;//{Координация}
const uint8_t ST_LUCK					=6;//{Удача}
//===========================
const uint8_t ST_MAX_LIFE				=7;//{Макс. Баллы Жизни}
const uint8_t ST_MAX_COND				=8;//{Максимальная кондиция}
const uint8_t ST_ARMOR_CLASS			=9;//{Класс Брони}
const uint8_t ST_MELEE_DAMAGE			=10;//{Вред невооруженный}
const uint8_t ST_WEAPON_DAMAGE			=11;//{Вред оружием}
const uint8_t ST_CARRY_WEIGHT			=12;//{Макс. груз}
const uint8_t ST_SEQUENCE				=13;//{Реакция}
const uint8_t ST_HEALING_RATE			=14;//{Лечения}
const uint8_t ST_CRITICAL_CHANCE		=15;//{Шанс критический.}
const uint8_t ST_MAX_CRITICAL			=16;//{Максимальный критический}
const uint8_t ST_INGURE_ABSORB			=17;//{Порог Ранения}
const uint8_t ST_LASER_ABSORB			=18;//{Порог Повреждения лазером}
const uint8_t ST_FIRE_ABSORB			=19;//{Порог Повреждения Огнем}
const uint8_t ST_PLASMA_ABSORB			=20;//{Порог Повреждения Плазмой}
const uint8_t ST_ELECTRO_ABSORB		=21;//{Порог Повреждения Электричеством}
const uint8_t ST_EMP_ABSORB			=22;//{Порог повреждения EMP}
const uint8_t ST_BLAST_ABSORB			=23;//{Порог Повреждения при Взрыве}
const uint8_t ST_INGURE_RESIST			=24;//{Сопротивляемость Ранению}
const uint8_t ST_LASER_RESIST			=25;//{Сопротивляемость Ранению Лазером}
const uint8_t ST_FIRE_RESIST			=26;//{Сопротивляемость Ранению Огнем}
const uint8_t ST_PLASMA_RESIST			=27;//{Сопротивляемость Ранению Плазмой}
const uint8_t ST_ELECTRO_RESIST		=28;//{Сопротивляемость Ранению Электричеством}
const uint8_t ST_EMP_RESIST			=29;//{Сопротивляемость Ранению EMP}
const uint8_t ST_BLAST_RESIST			=30;//{Сопротивляемость Ранению при Взрыве}
const uint8_t ST_RADIATION_RESISTANCE	=31;//{Сопротивляемость Радиации}
const uint8_t ST_POISON_RESISTANCE		=32;//{Сопротивляемость Ядам}
const uint8_t ST_AGE					=33;//{Возраст}
const uint8_t ST_GENDER				=34;//{Род}
const uint8_t ST_CURRENT_HP			=35;//{Текущие Баллы жизни}
const uint8_t ST_POISONING_LEVEL		=36;//{Текущий Уровень Ядов}
const uint8_t ST_RADIATION_LEVEL		=37;//{Текущий Уровень Радиации}
const uint8_t ST_CURRENT_STANDART		=38;//{Текущая кондиция}

//Навыки. Skills
const uint8_t SK_SMALL_GUNS			=0;//{Мелкое Оружие}
const uint8_t SK_BIG_GUNS				=1;//{Крупное Оружие}
const uint8_t SK_ENERGY_WEAPONS		=2;//{Энергетическое Оружие}
const uint8_t SK_UNARMED				=3;//{Не вооружен}
const uint8_t SK_MELEE_WEAPONS			=4;//{Холодное Оружие}
const uint8_t SK_THROWING				=5;//{Бросок}
const uint8_t SK_FIRST_AID				=6;//{Помощь}
const uint8_t SK_DOCTOR				=7;//{Врач}
const uint8_t SK_SNEAK					=8;//{Крадучесть}
const uint8_t SK_LOCKPICK				=9;//{Взломщик}
const uint8_t SK_STEAL					=10;//{Красть}
const uint8_t SK_TRAPS					=11;//{Ловушки}
const uint8_t SK_SCIENCE				=12;//{Наука}
const uint8_t SK_REPAIR				=13;//{Ремонт}
const uint8_t SK_SPEECH				=14;//{Речь}
const uint8_t SK_BARTER				=15;//{Обмен} Действует на НПС
const uint8_t SK_GAMBLING				=16;//{Играть} Действует на НПС
const uint8_t SK_OUTDOORSMAN			=17;//{Знаток Внешнего Мира}

//Перки. Perks
const uint8_t PE_FAST_METABOLISM		=0;//{Обмен Веществ}
const uint8_t PE_BRUISER				=1;//{Бугай}
const uint8_t PE_SMALL_FRAME			=2;//{"Коротышка"}
const uint8_t PE_ONE_HANDER			=3;//{Однорукий}
const uint8_t PE_FINESSE				=4;//{Искусный}
const uint8_t PE_KAMIKAZE				=5;//{Камикадзе}
const uint8_t PE_HEAVY_HANDED			=6;//{Тяжелая Рука}
const uint8_t PE_FAST_SHOT				=7;//{Быстрый Выстрел}
const uint8_t PE_BLOODY_MESS			=8;//{Кровавое Месиво}
const uint8_t PE_JINXED				=9;//{Несчастливый}
const uint8_t PE_GOOD_NATURED			=10;//{Добродушный}
const uint8_t PE_CHEM_RELIANT			=11;//{Привыкание}
const uint8_t PE_CHEM_RESISTANT		=12;//{Устойчивый}
const uint8_t PE_NIGHT_PERSON			=13;//{Сексапильность}  # was: Night Person
const uint8_t PE_SKILLED				=14;//{Умелый}
const uint8_t PE_GIFTED				=15;//{Одаренный}
const uint8_t PE_AWARENESS				=16;//{Осведомленность}
const uint8_t PE_A_MELEE_ATT			=17;//{Доп. рукопашная атака}
const uint8_t PE_A_MELEE_DAM			=18;//{Доп. урон в рукопашную}
const uint8_t PE_A_MOVE				=19;//{Доп. движение}
const uint8_t PE_A_DAM					=20;//{Доп. повреждение}
const uint8_t PE_A_SPEED				=21;//{Доп. скорость выстрелов}
//-----------------------------------
const uint8_t PE_PASS_FRONT			=22;//{Пропустить вперед}
const uint8_t PE_RAPID_HEAL			=23;//{Быстрое лечение}
const uint8_t PE_MORE_CRIT_DAM			=24;//{Больше критических выстрелов}
const uint8_t PE_NIGHT_SIGHT			=25;//{Ночное видение}
const uint8_t PE_PRESENCE				=26;//{Присутствие}
const uint8_t PE_RES_NUKLEAR			=27;//{Сопротивление радиации}
const uint8_t PE_ENDURENCE				=28;//{Выносливость}
const uint8_t PE_STR_BACK				=29;//{Сильная спина}
const uint8_t PE_MARKSMAN				=30;//{Меткий стрелок}
const uint8_t PE_STEALHING				=31;//{Бесшумный бег}
const uint8_t PE_LIFEFULL				=32;//{Живучий}
const uint8_t PE_MERCHANT				=33;//{Умелыйпродавец}
const uint8_t PE_FORMED				=34;//{Образованный}
const uint8_t PE_HEALER				=35;//{Лекарь}
const uint8_t PE_TR_DIGGER				=36;//{Кладоискатель}
const uint8_t PE_BEST_HITS				=37;//{Лучшие удары}
const uint8_t PE_COMPASION				=38;//{Сочувствие}
const uint8_t PE_KILLER				=39;//{Убийца}
const uint8_t PE_SNIPER				=40;//{Снайпер}
const uint8_t PE_SILENT_DEATH			=41;//{Безмолвная Смерть}
const uint8_t PE_C_FIGHTER				=42;//{Цикличный Боец}
const uint8_t PE_MIND_BLOCK			=43;//{Блокировка сознания}
const uint8_t PE_PROLONGATION_LIFE		=44;//{Продление жизни}
const uint8_t PE_RECOURCEFULNESS		=45;//{Изворотливость}
const uint8_t PE_SNAKE_EATER			=46;//{Пожиратель змей}
const uint8_t PE_REPEARER				=47;//{Ремонтник}
const uint8_t PE_MEDIC					=48;//{Медик}
const uint8_t PE_SKILLED_THIEF			=49;//{Умелый Вор}
const uint8_t PE_SPEAKER				=50;//{Спикер}
const uint8_t PE_GUTCHER				=51;//{Взяли!}
const uint8_t PE_UNKNOWN_1				=52;//{}  # was -- Friendly Foe
const uint8_t PE_PICK_POCKER			=53;//{Карманник}
const uint8_t PE_GHOST					=54;//{Призрак}
const uint8_t PE_CHAR_CULT				=55;//{Культ личности}
const uint8_t PE_THIFER				=56;//{Воришка}
const uint8_t PE_DISCOVER				=57;//{Исследователь}
const uint8_t PE_THE_PURETY			=58;//{Сама Чистота}
const uint8_t PE_OVERROAD				=59;//{Проходчик}
const uint8_t PE_ANIMAL_FRIENDSHIP		=60;//{Друг Животных}
const uint8_t PE_SCOUT					=61;//{Скаут}
const uint8_t PE_MIST_CHAR				=62;//{Таинственный Незнакомец}
const uint8_t PE_RANGER				=63;//{Рейнджер}
const uint8_t PE_PICK_POCKET_2			=64;//{Карманник}
const uint8_t PE_INTERLOCUTER			=65;//{Собеседник}
const uint8_t PE_NOVICE				=66;//{Способный Ученик}
const uint8_t PE_PRIME_SKILL			=67;//{Основное умение!}
const uint8_t PE_MUTATION				=68;//{Мутация!}
const uint8_t PE_NARC_NUKACOLA			=69;//{Пристрастие к NukaCola}
const uint8_t PE_NARC_BUFFOUT			=70;//{Пристрастие к Буфауту}
const uint8_t PE_NARC_MENTAT			=71;//{Пристрастие к Ментату}
const uint8_t PE_NARC_PSYHO			=72;//{Пристрастие к Психо}
const uint8_t PE_NARC_RADAWAY			=73;//{Пристрастие РэдЭвей}
const uint8_t PE_DISTANT_WEAP			=74;//{Дальнобойное оружие}
const uint8_t PE_ACCURARY_WEAP			=75;//{Точное оружие}
const uint8_t PE_PENETRATION_WEAP		=76;//{Проникающее оружие}
const uint8_t PE_KILLER_WEAP			=77;//{Убойное Оружие}
const uint8_t PE_ENERGY_ARMOR			=78;//{Энергетическая Броня}
const uint8_t PE_BATTLE_ARMOR			=79;//{Боевая Броня}
const uint8_t PE_WEAP_RANGE			=80;//{Дальнобойность}
const uint8_t PE_RAPID_RELOAD			=81;//{Быстрая перезарядка}
const uint8_t PE_NIGHT_SPYGLASS		=82;//{Оружие с ночным видением}
const uint8_t PE_FLAMER				=83;//{Огнемет}
const uint8_t PE_APA_I					=84;//{Улучшенная Броня I}
const uint8_t PE_APA_II				=85;//{Улучшенная Броня II}
const uint8_t PE_FORCEAGE				=86;//{Больше скорости}
const uint8_t PE_DEADLY_NARC			=87;//{Трагическое Пристрастие}
const uint8_t PE_CHARMOLEANCE			=88;//{Харизма }
const uint8_t PE_GEKK_SKINER			=89;//{Выделка Геков}
const uint8_t PE_SKIN_ARMOR			=90;//{Кожа-броня}
const uint8_t PE_A_SKIN_ARMOR			=91;//{Усиленная кожа- броня.}
const uint8_t PE_SUPER_ARMOR			=92;//{Супер броня}
const uint8_t PE_A_SUPER_ARMOR			=93;//{Усиленная Супер броня}
const uint8_t PE_VAULT_INOCUL			=94;//{Прививка Волта}
const uint8_t PE_ADRENALIN_RUSH		=95;//{Прилив адреналина}
const uint8_t PE_CAREFULL				=96;//{Осторожность}
const uint8_t PE_INTELEGENCE			=97;//{Понимание}
const uint8_t PE_PYROKASY				=98;//{Подрывник}
const uint8_t PE_DUDE					=99;//{Игрок}
//==========================
const uint8_t PE_A_STR					=100;//{Больше силы}
const uint8_t PE_A_PER					=101;//{Больше восприятия}
const uint8_t PE_A_END					=102;//{Больше выносливости}
const uint8_t PE_A_CHA					=103;//{Больше обаяния}
const uint8_t PE_A_INT					=104;//{Больше ума}
const uint8_t PE_A_AGL					=105;//{Больше ловкости}
const uint8_t PE_A_LUC					=106;//{Больше удачи}
//==========================
const uint8_t PE_PURERER				=107;//{Сама невинность}
const uint8_t PE_IMAG					=108;//{Немедленно}
const uint8_t PE_EVASION				=109;//{Уклонение}
const uint8_t PE_DROSHKADRAT			=110;//{Специалист по Кама- Сутре}
const uint8_t PE_KARMA_GLOW			=111;//{Свечение Кармы}
const uint8_t PE_SILENT_STEPS			=112;//{Легкие Шаги}
const uint8_t PE_ANATOMY				=113;//{Анатомия}
const uint8_t PE_CHAMER				=114;//{Притягательная личность}
const uint8_t PE_ORATOR				=115;//{Оратор}
const uint8_t PE_PACKER				=116;//{Кладовщик}
const uint8_t PE_EDD_GAYAN_MANIAC		=117;//{Маньяк- убийца}
const uint8_t PE_FAST_REGENERATION		=118;//{Быстрое выздоровление}
const uint8_t PE_VENDOR				=119;//{Продавец}
const uint8_t PE_STONE_WALL			=120;//{Каменная стена}
const uint8_t PE_THIEF_AGAIN			=121;//{Вор}
const uint8_t PE_WEAPON_SKILL			=122;//{Владение оружием}
const uint8_t PE_MAKE_VAULT			=123;//{Подготовка Волта}
const uint8_t PE_ALC_BUFF_1			=124;//{Алкоголь увеличил баллы жизни}
const uint8_t PE_ALC_BUFF_2			=125;//{Алкоголь увеличил баллы жизни}

const uint8_t PE_1				=126;//{Алкоголь уменьшил баллы жизни}
const uint8_t PE_2				=127;//{Алкоголь уменьшил баллы жизни II}
const uint8_t PE_3				=128;//{Автодок повысил баллы жизни}
const uint8_t PE_4				=129;//{Автодок повысил баллы жизни II}
const uint8_t PE_5				=130;//{Автодок понизил баллы жизни}
const uint8_t PE_6				=131;//{Автодок понизил баллы жизни II}
const uint8_t PE_7				=132;//{Добротный Дермогон}
const uint8_t PE_8				=133;//{Оружие усиленной убойной силы}
const uint8_t PE_9				=134;//{Несчастливый}
const uint8_t PE_10			=135;//{Книголюб (Книжный червь)}
const uint8_t PE_11			=136;//{Ментор (Тьютор)}
const uint8_t PE_12			=137;//{Выбит правый глаз}
const uint8_t PE_13			=138;//{Выбит левый глаз}

const uint8_t PE_HIDE_MODE		=139;//{Состояние скрытности (выкл/вкл)}

//Типы повреждений
const uint8_t DAMAGE_TYPE_NORMAL	=0;
const uint8_t DAMAGE_TYPE_LASER	=1;
const uint8_t DAMAGE_TYPE_FIRE		=2;
const uint8_t DAMAGE_TYPE_ELECTR	=3;
const uint8_t DAMAGE_TYPE_EMP		=4;
const uint8_t DAMAGE_TYPE_EXPLODE	=5;

//Предметы
//типы
const uint8_t OBJ_TYPE_ARMOR		=0;
const uint8_t OBJ_TYPE_CONTAINER	=1;
const uint8_t OBJ_TYPE_DRUG		=2;
const uint8_t OBJ_TYPE_WEAPON		=3;
const uint8_t OBJ_TYPE_AMMO		=4;
const uint8_t OBJ_TYPE_MISC		=5;
const uint8_t OBJ_TYPE_KEY			=6;
const uint8_t OBJ_TYPE_DOOR		=7;
const uint8_t OBJ_TYPE_CRAFTING	=8;
const uint8_t OBJ_TYPE_GRID		=9;

const uint8_t SOBJ_MAX_PARAMS		=80;
//=============================================== GENERAL
const uint8_t OBJ_NAME				=0;//{Имя итема}
const uint8_t OBJ_INFO				=1;//{Инфо итема}

const uint8_t OBJ_TIME_SHOW		=2;//{Время доставания объекта}
const uint8_t OBJ_TIME_HIDE		=3;//{Время скрывания объекта}

const uint8_t OBJ_DISTANCE_LIGHT	=4;//{Дистанция света}
const uint8_t OBJ_INTENSITY_LIGHT	=5;//{Интенсивность света}

const uint8_t OBJ_PASSED			=6;//{Проходимость объекта}
const uint8_t OBJ_RAKED			=7;//{Простреливаемость объекта}
const uint8_t OBJ_TRANSPARENT		=8;//{Прозрачность объекта} wall, glass, steam, energy
const uint8_t OBJ_CAN_USE			=9;//{Объект можно использовать}
const uint8_t OBJ_CAN_PICK_UP		=10;//{Объект можно поднять}
const uint8_t OBJ_CAN_USE_ON_SMTH	=11;//{Объект можно применять на чем либо}

const uint8_t OBJ_HIDDEN			=12;//{Скрытый}

const uint8_t OBJ_WEIGHT			=13;//{Вес}
const uint8_t OBJ_SIZE				=14;//{Объем в инвентаре}
const uint8_t OBJ_TWOHANDS			=15;//{Двуручный объект}

const uint8_t OBJ_PIC_MAP			=16;//{Индекс картинки на карте}
const uint8_t OBJ_ANIM_ON_MAP		=17;//{Номер анимации на карте если есть}
const uint8_t OBJ_PIC_INV			=18;//{Индекс картинки в инвентаре}
const uint8_t OBJ_SOUND			=19;//{Индекс звука}

const uint8_t OBJ_LIVETIME			=20;//{Начальное время жизни}

const uint8_t OBJ_COST				=21;//{Базовая цена}
const uint8_t OBJ_MATERIAL			=22;//{Материал предмета}
//----------------------------------------------- type==OBJ_TYPE_ARMOR
const uint8_t OBJ_ARM_ANIM0_MALE	=30;//{Индекс анимации дюда male}
const uint8_t OBJ_ARM_ANIM0_MALE2	=31;//{Индекс анимации дюда male дублер}
const uint8_t OBJ_ARM_ANIM0_FEMALE	=32;//{Индекс анимации дюда female}
const uint8_t OBJ_ARM_ANIM0_FEMALE2=33;//{Индекс анимации дюда female дублер}

const uint8_t OBJ_ARM_AC			=40;//{Армор класс}
const uint8_t OBJ_ARM_PERK			=41;//{Перк на броне}

const uint8_t OBJ_ARM_DR_NORMAL	=50;//{Сопротивление нормальное}
const uint8_t OBJ_ARM_DR_LASER		=51;//{Сопротивление лазеру}
const uint8_t OBJ_ARM_DR_FIRE		=52;//{Сопротивление огню}
const uint8_t OBJ_ARM_DR_PLASMA	=53;//{Сопротивление плазме}
const uint8_t OBJ_ARM_DR_ELECTR	=54;//{Сопротивление электричеству}
const uint8_t OBJ_ARM_DR_EMP		=55;//{Сопротивление емп}
const uint8_t OBJ_ARM_DR_EXPLODE	=56;//{Сопротивление взрыву}

const uint8_t OBJ_ARM_DT_NORMAL	=60;//{Порог повреждения нормальное}
const uint8_t OBJ_ARM_DT_LASER		=61;//{Порог повреждения лазеру}
const uint8_t OBJ_ARM_DT_FIRE		=62;//{Порог повреждения огню}
const uint8_t OBJ_ARM_DT_PLASMA	=63;//{Порог повреждения плазме}
const uint8_t OBJ_ARM_DT_ELECTR	=64;//{Порог повреждения электричеству}
const uint8_t OBJ_ARM_DT_EMP		=65;//{Порог повреждения емп}
const uint8_t OBJ_ARM_DT_EXPLODE	=66;//{Порог повреждения взрыву}
//----------------------------------------------- type==OBJ_TYPE_CONTAINER
const uint8_t OBJ_CONT_SIZE		=30;//{Объем контейнера}
const uint8_t OBJ_CONT_FLAG		=31;//{Флаг контейнера}
//----------------------------------------------- type==OBJ_TYPE_DRUG
const uint8_t OBJ_DRUG_STAT0		=30;//{}
const uint8_t OBJ_DRUG_STAT1		=31;//{}
const uint8_t OBJ_DRUG_STAT2		=32;//{}

const uint8_t OBJ_DRUG_AMOUNT0_S0	=40;//{}
const uint8_t OBJ_DRUG_AMOUNT0_S1	=41;//{}
const uint8_t OBJ_DRUG_AMOUNT0_S2	=42;//{}

const uint8_t OBJ_DRUG_DURATION1	=50;//{}
const uint8_t OBJ_DRUG_AMOUNT1_S0	=51;//{}
const uint8_t OBJ_DRUG_AMOUNT1_S1	=52;//{}
const uint8_t OBJ_DRUG_AMOUNT1_S2	=53;//{}

const uint8_t OBJ_DRUG_DURATION2	=60;//{}
const uint8_t OBJ_DRUG_AMOUNT2_S0	=61;//{}
const uint8_t OBJ_DRUG_AMOUNT2_S1	=62;//{}
const uint8_t OBJ_DRUG_AMOUNT2_S2	=63;//{}

const uint8_t OBJ_DRUG_ADDICTION	=70;//{}
const uint8_t OBJ_DRUG_W_EFFECT	=71;//{}
const uint8_t OBJ_DRUG_W_ONSET		=72;//{}
//----------------------------------------------- type==OBJ_TYPE_WEAPON
const uint8_t OBJ_WEAP_ANIM1		=30;//{Индекс анимации объекта}

const uint8_t OBJ_WEAP_TIME_ACTIV	=31;//{Время подготовки к использованию}
const uint8_t OBJ_WEAP_TIME_UNACTIV=32;//{Время отмены использования}

const uint8_t OBJ_WEAP_VOL_HOLDER	=33;//{Емкость основной обоймы}
const uint8_t OBJ_WEAP_CALIBER		=34;//{Калибр у основной обоймы}
const uint8_t OBJ_WEAP_VOL_HOLDER_E=35;//{Емкость дополнительной обоймы}
const uint8_t OBJ_WEAP_CALIBER_E	=36;//{Калибр у дополнительной обоймы}

const uint8_t OBJ_WEAP_CR_FAILTURE	=37;//{Критическая неудача при использовании}

const uint8_t OBJ_WEAP_TYPE_ATTACK	=38;//{Тип атаки оружия}

const uint8_t OBJ_WEAP_COUNT_ATTACK=39;//{Количество атак} | 0 - нет | 1 - PA | 2 - PA && SA | 3 - PA && SA && TA |

const uint8_t OBJ_WEAP_PA_SKILL	=40;//{Скилл отвечающий за использование}
const uint8_t OBJ_WEAP_PA_HOLDER	=41;//{Используемая обойма}
const uint8_t OBJ_WEAP_PA_PIC		=42;//{Рисунок использования}
const uint8_t OBJ_WEAP_PA_DMG_MIN	=43;//{Минимальное повреждения}
const uint8_t OBJ_WEAP_PA_DMG_MAX	=44;//{Максимальное повреждение}
const uint8_t OBJ_WEAP_PA_MAX_DIST	=45;//{Максимальная дистанция}
const uint8_t OBJ_WEAP_PA_EFF_DIST	=46;//{Эффективная дистанция}
const uint8_t OBJ_WEAP_PA_ANIM2	=47;//{Анимация атаки}
const uint8_t OBJ_WEAP_PA_TIME		=48;//{Базовое время атаки}
const uint8_t OBJ_WEAP_PA_AIM		=49;//{Наличие у атаки прицельного выстрела}
const uint8_t OBJ_WEAP_PA_ROUND	=50;//{Расход патронов за атаку}
const uint8_t OBJ_WEAP_PA_REMOVE	=51;//{Удаление предмета после атаки}

const uint8_t OBJ_WEAP_SA_SKILL	=53;//{Скилл отвечающий за использование}
const uint8_t OBJ_WEAP_SA_HOLDER	=54;//{Используемая обойма}
const uint8_t OBJ_WEAP_SA_PIC		=55;//{Рисунок использования}
const uint8_t OBJ_WEAP_SA_DMG_MIN	=56;//{Минимальное повреждения}
const uint8_t OBJ_WEAP_SA_DMG_MAX	=57;//{Максимальное повреждение}
const uint8_t OBJ_WEAP_SA_MAX_DIST	=58;//{Максимальная дистанция}
const uint8_t OBJ_WEAP_SA_EFF_DIST	=59;//{Эффективная дистанция}
const uint8_t OBJ_WEAP_SA_ANIM2	=60;//{Анимация атаки}
const uint8_t OBJ_WEAP_SA_TIME		=61;//{Базовое время атаки}
const uint8_t OBJ_WEAP_SA_AIM		=62;//{Наличие у атаки прицельного выстрела}
const uint8_t OBJ_WEAP_SA_ROUND	=63;//{Расход патронов за атаку}
const uint8_t OBJ_WEAP_SA_REMOVE	=64;//{Удаление предмета после атаки}

const uint8_t OBJ_WEAP_TA_SKILL	=66;//{Скилл отвечающий за использование}
const uint8_t OBJ_WEAP_TA_HOLDER	=67;//{Используемая обойма}
const uint8_t OBJ_WEAP_TA_PIC		=68;//{Рисунок использования}
const uint8_t OBJ_WEAP_TA_DMG_MIN	=69;//{Минимальное повреждения}
const uint8_t OBJ_WEAP_TA_DMG_MAX	=70;//{Максимальное повреждение}
const uint8_t OBJ_WEAP_TA_MAX_DIST	=71;//{Максимальная дистанция}
const uint8_t OBJ_WEAP_TA_EFF_DIST	=72;//{Эффективная дистанция}
const uint8_t OBJ_WEAP_TA_ANIM2	=73;//{Анимация атаки}
const uint8_t OBJ_WEAP_TA_TIME		=74;//{Базовое время атаки}
const uint8_t OBJ_WEAP_TA_AIM		=75;//{Наличие у атаки прицельного выстрела}
const uint8_t OBJ_WEAP_TA_ROUND	=76;//{Расход патронов за атаку}
const uint8_t OBJ_WEAP_TA_REMOVE	=77;//{Удаление предмета после атаки}
//----------------------------------------------- type==OBJ_TYPE_AMMO
const uint8_t OBJ_AMMO_CALIBER		=30;//{Калибр}
const uint8_t OBJ_AMMO_TYPE_DAMAGE	=31;//{Тип повреждения}
const uint8_t OBJ_AMMO_QUANTITY	=32;//{Количество}
const uint8_t OBJ_AMMO_AC			=33;//{AC}
const uint8_t OBJ_AMMO_DR			=34;//{DR}
const uint8_t OBJ_AMMO_DM			=35;//{Множитель для DR}
const uint8_t OBJ_AMMO_DD			=36;//{Делитель для DR}
//----------------------------------------------- type==OBJ_TYPE_MISC

//----------------------------------------------- type==OBJ_TYPE_KEY
//const uint8_t OBJ_KEY_DOOR			=30;//{От какой двери ключ}
//----------------------------------------------- OBJ_TYPE_DOOR
const uint8_t OBJ_DOOR_ANIMATION		=30;//{Анимация двери}
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

//область применения итемов
const uint8_t OBJ_USE_ON_CRITTER	=0;
const uint8_t OBJ_USE_ON_ITEM		=0;
const uint8_t OBJ_USE_ON_SCENERY	=0;

//!Cvet -------------------------------

//defines
#define MAX_NAME	20
#define MIN_NAME	4 //!Cvet
#define MAX_LOGIN	10
#define MIN_LOGIN	4 //!Cvet
#define MAX_TEXT	1024

#define MSGTYPE		uint8_t
#define CrID		uint32_t
#define CrTYPE		uint8_t
#define HexTYPE		uint16_t //!Cvet
#define DHexTYPE	uint32_t //!Cvet
#define MapTYPE		uint16_t //!Cvet
//!Cvet +++++++++++++++++++++++++++++++
#define TICK		uint32_t

#define MAX_SCENERY 5000

const uint8_t ACT_NULL				=1;
const uint8_t ACT_SHOW_OBJ			=2;
const uint8_t ACT_HIDE_OBJ			=3;
const uint8_t ACT_ACTIVATE_OBJ		=4;
const uint8_t ACT_DACTIVATE_OBJ	=5;
const uint8_t ACT_USE_OBJ			=6;
const uint8_t ACT_CHANGE_ARM		=7;
const uint8_t ACT_DEFEAT			=8;
	const uint8_t ACT_DEFEAT_MISS		=1;
	const uint8_t ACT_DEFEAT_FRONT		=2;
	const uint8_t ACT_DEFEAT_REAR		=3;
	const uint8_t ACT_DEFEAT_KO_FRONT	=4;
	const uint8_t ACT_DEFEAT_KO_REAR	=5;
const uint8_t ACT_REFRESH			=9;
const uint8_t ACT_DEAD				=10;
const uint8_t ACT_CR_DEAD			=11;
const uint8_t ACT_DISCONNECT		=12;
const uint8_t ACT_DROP_OBJ			=13;
const uint8_t ACT_PICK_OBJ_UP		=14;
const uint8_t ACT_PICK_OBJ_DOWN	=15;

//typedef pair<uint8_t,int> param_pair;
//typedef vector<param_pair> param_value;

//typedef vector<uint16_t*> type_of_use_vect;

struct stat_obj
{
	stat_obj():id(0),type(0){for(int i=0;i<SOBJ_MAX_PARAMS;i++)p[i]=0;};

	uint16_t id;
	uint8_t type;

	int p[SOBJ_MAX_PARAMS];
};

///////////////////////////////////////////////////////////////////////////////////////////

//uint8_t accessory
const uint8_t DOBJ_ACCESSORY_CRIT		=1;
const uint8_t DOBJ_ACCESSORY_HEX		=2;
const uint8_t DOBJ_ACCESSORY_CONT		=3;

const uint8_t DOBJ_SLOT_INV			=0;
const uint8_t DOBJ_SLOT_HAND1			=1;
const uint8_t DOBJ_SLOT_HAND2			=2;
const uint8_t DOBJ_SLOT_TWOHAND		=3;
const uint8_t DOBJ_SLOT_ARMOR			=4;

///////////////////////////////////////////////////////////////////////////////////////////

struct dyn_obj
{
//	dyn_obj():id(0),object(NULL),player(0),active(0),map(0),x(0),y(0),tick(0),last_tick(0),
//		holder(0),id_bull(0),holder_ext(0),id_bull_ext(0),broken(0){};

	uint32_t id;
	stat_obj* object;

	uint32_t time_wear;
	uint32_t last_tick;
	uint32_t broken_info;

	uint8_t accessory;
	union
	{
		struct
		{
			uint16_t map;
			HexTYPE x;
			HexTYPE y;
		} ACC_HEX;

		struct
		{
			CrID id;
			uint8_t slot;
//			uint8_t active;
		} ACC_CRITTER;

		struct
		{
			uint32_t id;
		} ACC_CONTAINER;
	};

	uint8_t type;
	union
	{
		struct
		{
			uint32_t reserved;
		} ITEM_ARMOR;

		struct
		{
			uint32_t reserved;
		} ITEM_CONTAINER;

		struct
		{
			uint32_t reserved;
		} ITEM_DRUG;

		struct
		{
			uint16_t AmmoPID;      // Прототип заряженных патронов.
			uint16_t AmmoCount;    // Количество заряженных патронов
			uint16_t AmmoPID_ext;      // Прототип заряженных патронов.
			uint16_t AmmoCount_ext;    // Количество заряженных патронов
		} ITEM_WEAPON;

		struct
		{
			uint16_t  Count;        // кол-во патронов в обойме
		} ITEM_AMMO;

		struct
		{
			uint16_t  Count;        // Текущее количество зарядов (т.е., насколько заряжен).
		} ITEM_MISC;

		struct
		{
			uint32_t   doorID;     // Код двери, которую открывает ключ
		} ITEM_KEY;

		struct
		{
			uint32_t   doorID;     // Код двери
		} ITEM_DOOR;

		struct
		{
			uint32_t   doorID;     // Код двери
		} ITEM_CRAFTING;

		struct
		{
			uint32_t   MapID;      // Номер карты, на которую попадает игрок.
			uint16_t    PosX;       // Позиция игрока на новой карте
			uint16_t    PosY;
			uint8_t    dir;        // Ориентация игрока
		} ITEM_GRID;
	};
};

typedef std::map<uint32_t, dyn_obj*> dyn_map;
typedef std::map<uint16_t, stat_obj*> stat_map;

typedef std::set<uint16_t> list_ind;

//!Cvet -------------------------------

struct crit_info
{
	crit_info():a_obj(&def_obj1),a_obj_arm(&def_obj2),base_type(0),map(0),x(0),y(0),ori(0),look(0),
		start_bt(0),break_time(0),id(0),idchannel(0),cond(COND_NOT_IN_GAME),cond_ext(0),flags(0),
		access(0),world_x(100),world_y(100){};

	CrTYPE base_type; //базовый тип криттера

	MapTYPE map; //номер карты
	HexTYPE x; //позиция в хексах
	HexTYPE y;

	uint16_t world_x;
	uint16_t world_y;

	uint8_t ori; //ориентация
	uint8_t look; //!Cvet дистанция видимости
	char name[MAX_NAME+1]; //имя
	char cases[5][MAX_NAME+1]; //имя склоняемые варианты

	uint8_t access; //!Cvet уровень доступа к игре

	uint32_t start_bt; //!Cvet
	int break_time; //!Cvet

	CrID id; //!Cvet Идентификатор
	CrID idchannel; //!Cvet Идентификатор канала к которому подключен игрок
	char login[MAX_LOGIN+1]; //!Cvet логин
	char pass[MAX_LOGIN+1]; //!Cvet пароль
	uint16_t st[ALL_STATS ]; //!Cvet статы 4-х значный XXXX
	uint16_t sk[ALL_SKILLS]; //!Cvet скилы 3-х значный XXX
	uint8_t pe[ALL_PERKS ]; //!Cvet перки 1-а значный X

	uint8_t cond; //!Cvet состояние криттера
	uint8_t cond_ext; //!Cvet дополнительное состояние криттера
	uint16_t flags; //!Cvet флаги криттера

	list_ind obj_id;

	//инфа по объектам
	dyn_map obj; //!Cvet карта динамических объектов
	//int Slot1, Slot2, ArmorSlot; 
	dyn_obj* a_obj; //активный объект в руках
	dyn_obj* a_obj_arm; //активный объект в слоте армор

	dyn_obj def_obj1;
	dyn_obj def_obj2;
};

#define NETMSG_LOGIN 1
//////////////////////////////////////////////////////////////////////////
// клиент при подключении шлет это сообщение, называя логин и пароль
// params:
// char login[MAX_LOGIN]
// char pass[MAX_LOGIN]
//////////////////////////////////////////////////////////////////////////

#define NETMSG_LOGINOK 2
//////////////////////////////////////////////////////////////////////////
// ответ сервера на NETMSG_NAME - логин и пароль принят
// params:
//////////////////////////////////////////////////////////////////////////

#define NETMSG_CREATE_CLIENT 3 //!Cvet
//////////////////////////////////////////////////////////////////////////
// запрос серверу на создание аккаунта
// params:
// char login[MAX_LOGIN]; //!Cvet логин
// char pass[MAX_LOGIN]; //!Cvet пароль
// char name[MAX_NAME]; // имя
// char cases[5][MAX_NAME]; // имя склоняемые варианты
// CrTYPE base_type
// uint16_t s[MAX_STATS] //статы
//////////////////////////////////////////////////////////////////////////
#define NETMSG_LOGMSG 4
//////////////////////////////////////////////////////////////////////////
// ответ сервера на NETMSG_NAME - имя не принято
// params:
// uint8_t LogMsg
//////////////////////////////////////////////////////////////////////////

#define NETMSG_NAMEERR 5
//////////////////////////////////////////////////////////////////////////
// ответ сервера на NETMSG_NAME - имя не принято
// params:
//////////////////////////////////////////////////////////////////////////

#define NETMSG_ADDCRITTER 6 //!Cvet edit
//////////////////////////////////////////////////////////////////////////
// добавить криттер
// params:
// CrID id
// CrTYPE base_type
// uint16_t id_stat
// uint16_t id_stat_arm
// HexTYPE x
// HexTYPE y
// uint8_t ori
// uint8_t st[ST_GENDER] пол
// uint8_t cond
// uint8_t cond_ext
// uint16_t flags
// char[MAX_NAME] name
// char[5][MAX_NAME] cases
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#define MSG_ADDCRITTER_LEN (sizeof(CrID)+sizeof(CrTYPE)+5+MAX_NAME)
//////////////////////////////////////////////////////////////////////////
// Сообщение длины добавленного криттера

#define NETMSG_REMOVECRITTER 7
//////////////////////////////////////////////////////////////////////////
// убрать криттер
// params:
// CrID id

#define NETMSG_TEXT 8
//////////////////////////////////////////////////////////////////////////
// текстовое сообщение
// params:
// CrID crid
// uint8_t how_say
// uint16_t len
// char[len] str

#define NETMSG_CRITTERTEXT 9
//////////////////////////////////////////////////////////////////////////
// текст который надо нарисовать над криттером
// params:
// CrID id
// uint16_t len
// char[len] str

#define NETMSG_DIR 10 //!Cvet edit
//////////////////////////////////////////////////////////////////////////
// сообщение направления
// params:
// uint8_t dir

#define NETMSG_CRITTER_DIR 11 //!Cvet
//////////////////////////////////////////////////////////////////////////
// направление криттера
// params:
// CrID id
// uint8_t dir

#define NETMSG_SEND_MOVE 12
//////////////////////////////////////////////////////////////////////////
// передача на сервер направления для шага чезена
// params:
// uint8_t dir
// uint8_t move_params - dir,how_move,stop&go
#define NETMSG_CRITTER_MOVE 13
//////////////////////////////////////////////////////////////////////////
// передача направления для других криттеров
// params:
// CrID id
// uint8_t move_params - dir,how_move,stop&go
// HexTYPE x
// HexTYPE y
#define NETMSG_QUIT 14
//////////////////////////////////////////////////////////////////////////
// сигнал о санкционированном выходе клиента
// params:
// CrID id

#define NETMSG_XY 15 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал о неверном положении чезена
// params:
// HexTYPE Chex_x
// HexTYPE Chex_y
// uint8_t dir

#define NETMSG_ALL_PARAMS 16 //!Cvet
//////////////////////////////////////////////////////////////////////////
// передача параметров
// params:
// uint8_t type_param - тип параметра
// uint8_t all_send - всего статов отправлено
// uint8_t num_param - номер параметра
// uint16_t or uint8_t count_param - его кол-во
#define NETMSG_PARAM 17 //!Cvet
//////////////////////////////////////////////////////////////////////////
// передача отдельного параметра
// params:
// uint8_t type_param - тип параметра
// uint16_t num_param - номер параметра
// uint16_t or uint8_t count_param - его кол-во

#define NETMSG_ADD_OBJECT 18 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал о том что ктото спрятал оружие
// params:
// uint32_t id; -dyn
// uint16_t id; -stat
// uint8_t slot -0-inv 1-a_slot
// uint16_t holder;
// uint16_t id_bull;
// uint16_t holder_ext;
// uint16_t id_bull_ext;
// TICK tick;
// uint8_t broken;
#define NETMSG_REMOVE_OBJECT 19 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал о том что ктото спрятал оружие
// params:
// uint32_t id; -dyn
#define NETMSG_WEAR_OBJECT 20 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал о износе предмета
// params:
// uint32_t id; -dyn;
// TICK tick;
// uint8_t broken;

#define NETMSG_ADD_OBJECT_ON_MAP 21 //!Cvet
//////////////////////////////////////////////////////////////////////////
// 
// params:
// HexTYPE x;
// HexTYPE y;
// uint8_t id_stat;
// uint16_t tile_flags;
#define NETMSG_CHANGE_OBJECT_ON_MAP 22 //!Cvet
//////////////////////////////////////////////////////////////////////////
// 
// params:
// HexTYPE x;
// HexTYPE y;
// uint8_t id_stat;
// uint16_t tile_flags;
#define NETMSG_REMOVE_OBJECT_FROM_MAP 23 //!Cvet
//////////////////////////////////////////////////////////////////////////
// 
// params:
// HexTYPE x;
// HexTYPE y;
// uint8_t id_stat;

#define NETMSG_SEND_CHANGE_OBJECT 24 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал чузена о приминении активного предмета
// params:

#define NETMSG_SEND_PICK_OBJECT 25 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал чузена о приминении активного предмета
// params:

#define NETMSG_SEND_USE_OBJECT 26 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал чузена о приминении активного предмета
// params:

#define NETMSG_SEND_USE_SKILL 27 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал о приминении скилла
// params:
// uint8_t skill; -номер скилла
// uint32_t target_id; -игрок или предмет
// uint8_t ori; -напровление применения

#define NETMSG_CRITTER_ACTION 28 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал о том что криттер производит какоето действие
// params:
// CrID crid; -ид криттера
// uint8_t ori; -направление действия
// uint8_t type_action; -номер действия -последняя буква
// uint8_t type; -тип криттера -название криттера
// uint8_t weap; -номер аним оружия в руках -предпоследняя буква
// uint16_t number_of_action; -что за действие передано

#define NETMSG_SEND_TALK_NPC 29 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал игрока о беседе
// CrID id_talk - ид нпц с которым разговариваешь
// uint8_t variant - вариант ответа
#define NETMSG_TALK_NPC 30 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ответ нпц -> игроку
// uint8_t all_answers - всего вариантов ответа, если 0 - диалог закончен
// uint32_t main_text - текст НПЦ
// uint32_t answ_text в кол-ве all_answers - варианты ответа

#define NETMSG_SEND_GET_TIME 31 //!Cvet
//////////////////////////////////////////////////////////////////////////
// запрос на игровое время
#define NETMSG_GAME_TIME 32 //!Cvet
//////////////////////////////////////////////////////////////////////////
// высылается игроку пройденное игровое время с начала игровых суток
// uint16_t minutes - пройденное игровое время
// uint8_t day - игровой день
// uint8_t month - игровой месяц
// uint16_t year - игровой год

#define NETMSG_LOADMAP 33 //!Cvet
//////////////////////////////////////////////////////////////////////////
// высылается игроку команда загрузки карты
// uint16_t num_map - номер карты
// uint16_t num_version - номер версии карты
// на будущее !!!!!!!!
// HexTYPE width - ширина по Х
// HexTYPE height - ширина по У
#define NETMSG_MAP 34 //!Cvet
//////////////////////////////////////////////////////////////////////////
// карта
// uint16_t num_map - номер карты
#define NETMSG_SEND_GIVE_ME_MAP 35 //!Cvet
//////////////////////////////////////////////////////////////////////////
// запрос на высылку карты
// uint16_t num_map - номер карты
#define NETMSG_SEND_LOAD_MAP_OK 36 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сообщение о удачной загрузке карты

//GLOBAL MAP
const uint8_t GM_INFO_CITIES	=BIN8(00000001);
const uint8_t GM_INFO_CRITS	=BIN8(00000010);
const uint8_t GM_INFO_PARAM	=BIN8(00000100);
const uint8_t GM_INFO_ALL		=BIN8(00000111);
//const uint8_t GM_ADDINFO_CITY	=BIN8(00001000);
//const uint8_t GM_DELINFO_CITY	=BIN8(00010000);
#define NETMSG_SEND_GIVE_GLOBAL_INFO 37 //!Cvet
//////////////////////////////////////////////////////////////////////////
// запрос инфы по глобалу
// uint8_t info_flag;
#define NETMSG_GLOBAL_INFO 38 //!Cvet
//////////////////////////////////////////////////////////////////////////
// инфа по глобалу
// uint8_t info_flag;
// info_flag & GM_INFO_CITIES || info_flag & GM_INFO_ALL
//		uint16_t count_cities;
//			for count_cities
//			uint16_t city_num
//			uint16_t city_x
//			uint16_t city_y
//			uint8_t city_radius
// info_flag & GM_ADDINFO_CITY
//			uint16_t city_num
//			uint16_t city_x
//			uint16_t city_y
//			uint8_t city_radius
// info_flag & GM_DELINFO_CITY
//			uint16_t city_num
// info_flag & GM_INFO_CRITS || info_flag & GM_INFO_ALL
//		uint8_t count_group;
//			for count_group
//			CrID id_crit;
//			char[MAX_NAME] name;
//			uint16_t flags_crit;
// info_flag & GM_INFO_PARAM || info_flag & GM_INFO_ALL
//		uint16_t group_x;
//		uint16_t group_y;
//		uint16_t move_x;
//		uint16_t move_y;
//		uint8_t speed;
//		int speed_x;
//		int speed_y;
// uint8_t 0xAA - end_info
const uint8_t GM_RULE_COMMAND_SETMOVE		=1; //+r-a*x,y//двигаться
const uint8_t GM_RULE_COMMAND_STOP			=2; //+r-a//остановиться
const uint8_t GM_RULE_COMMAND_TOLOCAL		=3; //+r-a*num_city,num_map//перейти на локльную карту
const uint8_t GM_RULE_COMMAND_KICKCRIT		=4; //+r-a*cr_id//выкинуть крита из группы
const uint8_t GM_RULE_COMMAND_LOCKCRIT		=5; //+r-a*cr_id//блокировать крита
const uint8_t GM_RULE_COMMAND_UNLOCKCRIT	=6; //+r-a*cr_id//разблокировать крита
const uint8_t GM_RULE_COMMAND_IGNORECRIT	=7; //+r+a*cr_id//игнорировать крита
const uint8_t GM_RULE_COMMAND_LISTENCRIT	=8; //+r+a*cr_id//слушать крита
const uint8_t GM_RULE_COMMAND_READY		=9; //+r+a//готовность к высадке на локальную карту
const uint8_t GM_RULE_COMMAND_TOGEMMATE	=10;//-r+a//отсоединиться от группы
const uint8_t GM_RULE_COMMAND_SETSPEED		=11;//+r-a*speed//задать скорость группы
const uint8_t GM_RULE_COMMAND_ADDPREPCRIT	=12;//+r-a*cr_id//добавит в список крита
const uint8_t GM_RULE_COMMAND_DELPREPCRIT	=13;//+r-a*cr_id//удалить со списка крита
#define NETMSG_SEND_RULE_GLOBAL 39 //!Cvet
//////////////////////////////////////////////////////////////////////////
// управление группой
// uint8_t command;
// uint32_t param1;
// uint32_t param2;

//states

#define STATE_CONN			0
#define STATE_DROPLINK		1
#define STATE_DISCONNECT	2
#define STATE_GAME			3

//послан логин и пароль чузена
#define STATE_LOGINOK		5 //!Cvet

#define STATE_INIT_NET		6


#endif //__NETPROTO_H__