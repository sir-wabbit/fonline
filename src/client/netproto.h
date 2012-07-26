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
const WORD GM_ZONE_LEN			=50; //должна быть кратна GM_MAXX и GM_MAXY
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
const BYTE USE_OBJ_ON_CRITTER	=1;
const BYTE USE_OBJ_ON_OBJ		=2;

//дефолтное время для действий
const DWORD ACTTIME_DROP_OBJ	=2000;
const DWORD ACTTIME_USE_DOOR	=3000;
const DWORD ACTTIME_USE_CONT	=3000;
const DWORD ACTTIME_PICK_OBJ	=6000;

//движение
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

//время в игре
const BYTE TIME_MULTIPLER	=8;

//общие ответы над головой
const BYTE NPC_SAY_NONE		=0;
const BYTE NPC_SAY_ERROR	=1;
const BYTE NPC_SAY_IMBYSY	=2;
const BYTE NPC_SAY_HELLO	=3;
const BYTE NPC_SAY_IMHAPPY	=4;
const BYTE NPC_SAY_ILOVEYOU	=5;
const BYTE NPC_SAY_LEAVEME	=6;
const BYTE NPC_SAY_FUCKOFF	=7;
const BYTE NPC_SAY_IHATEYOU	=8;

//тип ответа
#define SAY_NORM	0
#define SAY_SHOUT	1
#define SAY_EMOTE	2
#define SAY_WHISP	3 
#define SAY_SOCIAL	4

//состояния криттеров
const BYTE COND_LIFE				=1;//состояние жизни
	const BYTE COND_LIFE_NONE			=1;//состояние
	const BYTE COND_LIFE_ACTWEAP		=2;//состояние
	const BYTE COND_LIFE_USEOBJ			=3;//состояние
	const BYTE COND_LIFE_OFFLINE		=4;//состояние
const BYTE COND_KNOCK_OUT			=2;//состояние накаута
	const BYTE COND_KO_UP				=1;//состояние накаута вверх лицом
	const BYTE COND_KO_DOWN				=2;//состояние накаута вниз лицом
const BYTE COND_DEAD				=3;//состояние смерти
	const BYTE COND_DEAD_NORMAL_UP		=1;//состояние смерти
	const BYTE COND_DEAD_NORMAL_DOWN	=2;//состояние смерти
	const BYTE COND_DEAD_CR_NORMAL_UP	=3;//состояние смерти
	const BYTE COND_DEAD_BRUST			=4;//состояние смерти
	const BYTE COND_DEAD_CR_BRUST		=5;//состояние смерти
	const BYTE COND_DEAD_LASER			=6;//состояние смерти
	const BYTE COND_DEAD_FIRE			=7;//состояние смерти
	const BYTE COND_DEAD_PLASMA			=8;//состояние смерти
	const BYTE COND_DEAD_ELECTR			=9;//состояние смерти
	const BYTE COND_DEAD_EMP			=10;//состояние смерти
	const BYTE COND_DEAD_EXPLODE		=11;//состояние смерти
const BYTE COND_NOT_IN_GAME			=4;//состояние вне игры

//флаги криттеров
#define FCRIT_PLAYER			BIN8(00000001)
#define FCRIT_NPC				BIN8(00000010)
#define FCRIT_MOB				BIN8(00000100)
#define FCRIT_DISCONNECT		BIN8(00001000)
#define FCRIT_CHOSEN			BIN8(00010000)
#define FCRIT_RULEGROUP			BIN8(00100000)

//режимы голоса
const BYTE SAY_SHORT		=1;//говорить шепетом на...
const BYTE SAY_SHORT_RANGE	=2;//...клетку(и)
const BYTE SAY_NORMAL		=2;//говорить нормально, слышат все кто тебя видит
const BYTE SAY_ALL			=3;//говорить громко, слышно на всю карту

//номера групп
const BYTE TYPE_STAT		=1;
const BYTE TYPE_SKILL		=2;
const BYTE TYPE_PERK		=3;

//общее кол-во параметров
const BYTE ALL_STATS		=39;
const BYTE ALL_SKILLS		=18;
const BYTE ALL_PERKS		=140;

//карта параметров
typedef map<string, BYTE, less<string> > params_map;
typedef map<BYTE, string, less<BYTE> > params_str_map;

//Статы. Stats
const BYTE ST_STRENGHT				=0;//{Сила}
const BYTE ST_PERCEPTION			=1;//{Восприятие}
const BYTE ST_ENDURANCE				=2;//{Выносливость}
const BYTE ST_CHARISMA				=3;//{Обаяние}
const BYTE ST_INTELLECT				=4;//{Ум}
const BYTE ST_AGILITY				=5;//{Координация}
const BYTE ST_LUCK					=6;//{Удача}
//===========================
const BYTE ST_MAX_LIFE				=7;//{Макс. Баллы Жизни}
const BYTE ST_MAX_COND				=8;//{Максимальная кондиция}
const BYTE ST_ARMOR_CLASS			=9;//{Класс Брони}
const BYTE ST_MELEE_DAMAGE			=10;//{Вред невооруженный}
const BYTE ST_WEAPON_DAMAGE			=11;//{Вред оружием}
const BYTE ST_CARRY_WEIGHT			=12;//{Макс. груз}
const BYTE ST_SEQUENCE				=13;//{Реакция}
const BYTE ST_HEALING_RATE			=14;//{Лечения}
const BYTE ST_CRITICAL_CHANCE		=15;//{Шанс критический.}
const BYTE ST_MAX_CRITICAL			=16;//{Максимальный критический}
const BYTE ST_INGURE_ABSORB			=17;//{Порог Ранения}
const BYTE ST_LASER_ABSORB			=18;//{Порог Повреждения лазером}
const BYTE ST_FIRE_ABSORB			=19;//{Порог Повреждения Огнем}
const BYTE ST_PLASMA_ABSORB			=20;//{Порог Повреждения Плазмой}
const BYTE ST_ELECTRO_ABSORB		=21;//{Порог Повреждения Электричеством}
const BYTE ST_EMP_ABSORB			=22;//{Порог повреждения EMP}
const BYTE ST_BLAST_ABSORB			=23;//{Порог Повреждения при Взрыве}
const BYTE ST_INGURE_RESIST			=24;//{Сопротивляемость Ранению}
const BYTE ST_LASER_RESIST			=25;//{Сопротивляемость Ранению Лазером}
const BYTE ST_FIRE_RESIST			=26;//{Сопротивляемость Ранению Огнем}
const BYTE ST_PLASMA_RESIST			=27;//{Сопротивляемость Ранению Плазмой}
const BYTE ST_ELECTRO_RESIST		=28;//{Сопротивляемость Ранению Электричеством}
const BYTE ST_EMP_RESIST			=29;//{Сопротивляемость Ранению EMP}
const BYTE ST_BLAST_RESIST			=30;//{Сопротивляемость Ранению при Взрыве}
const BYTE ST_RADIATION_RESISTANCE	=31;//{Сопротивляемость Радиации}
const BYTE ST_POISON_RESISTANCE		=32;//{Сопротивляемость Ядам}
const BYTE ST_AGE					=33;//{Возраст}
const BYTE ST_GENDER				=34;//{Род}
const BYTE ST_CURRENT_HP			=35;//{Текущие Баллы жизни}
const BYTE ST_POISONING_LEVEL		=36;//{Текущий Уровень Ядов}
const BYTE ST_RADIATION_LEVEL		=37;//{Текущий Уровень Радиации}
const BYTE ST_CURRENT_STANDART		=38;//{Текущая кондиция}

//Навыки. Skills
const BYTE SK_SMALL_GUNS			=0;//{Мелкое Оружие}
const BYTE SK_BIG_GUNS				=1;//{Крупное Оружие}
const BYTE SK_ENERGY_WEAPONS		=2;//{Энергетическое Оружие}
const BYTE SK_UNARMED				=3;//{Не вооружен}
const BYTE SK_MELEE_WEAPONS			=4;//{Холодное Оружие}
const BYTE SK_THROWING				=5;//{Бросок}
const BYTE SK_FIRST_AID				=6;//{Помощь}
const BYTE SK_DOCTOR				=7;//{Врач}
const BYTE SK_SNEAK					=8;//{Крадучесть}
const BYTE SK_LOCKPICK				=9;//{Взломщик}
const BYTE SK_STEAL					=10;//{Красть}
const BYTE SK_TRAPS					=11;//{Ловушки}
const BYTE SK_SCIENCE				=12;//{Наука}
const BYTE SK_REPAIR				=13;//{Ремонт}
const BYTE SK_SPEECH				=14;//{Речь}
const BYTE SK_BARTER				=15;//{Обмен} Действует на НПС
const BYTE SK_GAMBLING				=16;//{Играть} Действует на НПС
const BYTE SK_OUTDOORSMAN			=17;//{Знаток Внешнего Мира}

//Перки. Perks
const BYTE PE_FAST_METABOLISM		=0;//{Обмен Веществ}
const BYTE PE_BRUISER				=1;//{Бугай}
const BYTE PE_SMALL_FRAME			=2;//{"Коротышка"}
const BYTE PE_ONE_HANDER			=3;//{Однорукий}
const BYTE PE_FINESSE				=4;//{Искусный}
const BYTE PE_KAMIKAZE				=5;//{Камикадзе}
const BYTE PE_HEAVY_HANDED			=6;//{Тяжелая Рука}
const BYTE PE_FAST_SHOT				=7;//{Быстрый Выстрел}
const BYTE PE_BLOODY_MESS			=8;//{Кровавое Месиво}
const BYTE PE_JINXED				=9;//{Несчастливый}
const BYTE PE_GOOD_NATURED			=10;//{Добродушный}
const BYTE PE_CHEM_RELIANT			=11;//{Привыкание}
const BYTE PE_CHEM_RESISTANT		=12;//{Устойчивый}
const BYTE PE_NIGHT_PERSON			=13;//{Сексапильность}  # was: Night Person
const BYTE PE_SKILLED				=14;//{Умелый}
const BYTE PE_GIFTED				=15;//{Одаренный}
const BYTE PE_AWARENESS				=16;//{Осведомленность}
const BYTE PE_A_MELEE_ATT			=17;//{Доп. рукопашная атака}
const BYTE PE_A_MELEE_DAM			=18;//{Доп. урон в рукопашную}
const BYTE PE_A_MOVE				=19;//{Доп. движение}
const BYTE PE_A_DAM					=20;//{Доп. повреждение}
const BYTE PE_A_SPEED				=21;//{Доп. скорость выстрелов}
//-----------------------------------
const BYTE PE_PASS_FRONT			=22;//{Пропустить вперед}
const BYTE PE_RAPID_HEAL			=23;//{Быстрое лечение}
const BYTE PE_MORE_CRIT_DAM			=24;//{Больше критических выстрелов}
const BYTE PE_NIGHT_SIGHT			=25;//{Ночное видение}
const BYTE PE_PRESENCE				=26;//{Присутствие}
const BYTE PE_RES_NUKLEAR			=27;//{Сопротивление радиации}
const BYTE PE_ENDURENCE				=28;//{Выносливость}
const BYTE PE_STR_BACK				=29;//{Сильная спина}
const BYTE PE_MARKSMAN				=30;//{Меткий стрелок}
const BYTE PE_STEALHING				=31;//{Бесшумный бег}
const BYTE PE_LIFEFULL				=32;//{Живучий}
const BYTE PE_MERCHANT				=33;//{Умелыйпродавец}
const BYTE PE_FORMED				=34;//{Образованный}
const BYTE PE_HEALER				=35;//{Лекарь}
const BYTE PE_TR_DIGGER				=36;//{Кладоискатель}
const BYTE PE_BEST_HITS				=37;//{Лучшие удары}
const BYTE PE_COMPASION				=38;//{Сочувствие}
const BYTE PE_KILLER				=39;//{Убийца}
const BYTE PE_SNIPER				=40;//{Снайпер}
const BYTE PE_SILENT_DEATH			=41;//{Безмолвная Смерть}
const BYTE PE_C_FIGHTER				=42;//{Цикличный Боец}
const BYTE PE_MIND_BLOCK			=43;//{Блокировка сознания}
const BYTE PE_PROLONGATION_LIFE		=44;//{Продление жизни}
const BYTE PE_RECOURCEFULNESS		=45;//{Изворотливость}
const BYTE PE_SNAKE_EATER			=46;//{Пожиратель змей}
const BYTE PE_REPEARER				=47;//{Ремонтник}
const BYTE PE_MEDIC					=48;//{Медик}
const BYTE PE_SKILLED_THIEF			=49;//{Умелый Вор}
const BYTE PE_SPEAKER				=50;//{Спикер}
const BYTE PE_GUTCHER				=51;//{Взяли!}
const BYTE PE_UNKNOWN_1				=52;//{}  # was -- Friendly Foe
const BYTE PE_PICK_POCKER			=53;//{Карманник}
const BYTE PE_GHOST					=54;//{Призрак}
const BYTE PE_CHAR_CULT				=55;//{Культ личности}
const BYTE PE_THIFER				=56;//{Воришка}
const BYTE PE_DISCOVER				=57;//{Исследователь}
const BYTE PE_THE_PURETY			=58;//{Сама Чистота}
const BYTE PE_OVERROAD				=59;//{Проходчик}
const BYTE PE_ANIMAL_FRIENDSHIP		=60;//{Друг Животных}
const BYTE PE_SCOUT					=61;//{Скаут}
const BYTE PE_MIST_CHAR				=62;//{Таинственный Незнакомец}
const BYTE PE_RANGER				=63;//{Рейнджер}
const BYTE PE_PICK_POCKET_2			=64;//{Карманник}
const BYTE PE_INTERLOCUTER			=65;//{Собеседник}
const BYTE PE_NOVICE				=66;//{Способный Ученик}
const BYTE PE_PRIME_SKILL			=67;//{Основное умение!}
const BYTE PE_MUTATION				=68;//{Мутация!}
const BYTE PE_NARC_NUKACOLA			=69;//{Пристрастие к NukaCola}
const BYTE PE_NARC_BUFFOUT			=70;//{Пристрастие к Буфауту}
const BYTE PE_NARC_MENTAT			=71;//{Пристрастие к Ментату}
const BYTE PE_NARC_PSYHO			=72;//{Пристрастие к Психо}
const BYTE PE_NARC_RADAWAY			=73;//{Пристрастие РэдЭвей}
const BYTE PE_DISTANT_WEAP			=74;//{Дальнобойное оружие}
const BYTE PE_ACCURARY_WEAP			=75;//{Точное оружие}
const BYTE PE_PENETRATION_WEAP		=76;//{Проникающее оружие}
const BYTE PE_KILLER_WEAP			=77;//{Убойное Оружие}
const BYTE PE_ENERGY_ARMOR			=78;//{Энергетическая Броня}
const BYTE PE_BATTLE_ARMOR			=79;//{Боевая Броня}
const BYTE PE_WEAP_RANGE			=80;//{Дальнобойность}
const BYTE PE_RAPID_RELOAD			=81;//{Быстрая перезарядка}
const BYTE PE_NIGHT_SPYGLASS		=82;//{Оружие с ночным видением}
const BYTE PE_FLAMER				=83;//{Огнемет}
const BYTE PE_APA_I					=84;//{Улучшенная Броня I}
const BYTE PE_APA_II				=85;//{Улучшенная Броня II}
const BYTE PE_FORCEAGE				=86;//{Больше скорости}
const BYTE PE_DEADLY_NARC			=87;//{Трагическое Пристрастие}
const BYTE PE_CHARMOLEANCE			=88;//{Харизма }
const BYTE PE_GEKK_SKINER			=89;//{Выделка Геков}
const BYTE PE_SKIN_ARMOR			=90;//{Кожа-броня}
const BYTE PE_A_SKIN_ARMOR			=91;//{Усиленная кожа- броня.}
const BYTE PE_SUPER_ARMOR			=92;//{Супер броня}
const BYTE PE_A_SUPER_ARMOR			=93;//{Усиленная Супер броня}
const BYTE PE_VAULT_INOCUL			=94;//{Прививка Волта}
const BYTE PE_ADRENALIN_RUSH		=95;//{Прилив адреналина}
const BYTE PE_CAREFULL				=96;//{Осторожность}
const BYTE PE_INTELEGENCE			=97;//{Понимание}
const BYTE PE_PYROKASY				=98;//{Подрывник}
const BYTE PE_DUDE					=99;//{Игрок}
//==========================
const BYTE PE_A_STR					=100;//{Больше силы}
const BYTE PE_A_PER					=101;//{Больше восприятия}
const BYTE PE_A_END					=102;//{Больше выносливости}
const BYTE PE_A_CHA					=103;//{Больше обаяния}
const BYTE PE_A_INT					=104;//{Больше ума}
const BYTE PE_A_AGL					=105;//{Больше ловкости}
const BYTE PE_A_LUC					=106;//{Больше удачи}
//==========================
const BYTE PE_PURERER				=107;//{Сама невинность}
const BYTE PE_IMAG					=108;//{Немедленно}
const BYTE PE_EVASION				=109;//{Уклонение}
const BYTE PE_DROSHKADRAT			=110;//{Специалист по Кама- Сутре}
const BYTE PE_KARMA_GLOW			=111;//{Свечение Кармы}
const BYTE PE_SILENT_STEPS			=112;//{Легкие Шаги}
const BYTE PE_ANATOMY				=113;//{Анатомия}
const BYTE PE_CHAMER				=114;//{Притягательная личность}
const BYTE PE_ORATOR				=115;//{Оратор}
const BYTE PE_PACKER				=116;//{Кладовщик}
const BYTE PE_EDD_GAYAN_MANIAC		=117;//{Маньяк- убийца}
const BYTE PE_FAST_REGENERATION		=118;//{Быстрое выздоровление}
const BYTE PE_VENDOR				=119;//{Продавец}
const BYTE PE_STONE_WALL			=120;//{Каменная стена}
const BYTE PE_THIEF_AGAIN			=121;//{Вор}
const BYTE PE_WEAPON_SKILL			=122;//{Владение оружием}
const BYTE PE_MAKE_VAULT			=123;//{Подготовка Волта}
const BYTE PE_ALC_BUFF_1			=124;//{Алкоголь увеличил баллы жизни}
const BYTE PE_ALC_BUFF_2			=125;//{Алкоголь увеличил баллы жизни}

const BYTE PE_1				=126;//{Алкоголь уменьшил баллы жизни}
const BYTE PE_2				=127;//{Алкоголь уменьшил баллы жизни II}
const BYTE PE_3				=128;//{Автодок повысил баллы жизни}
const BYTE PE_4				=129;//{Автодок повысил баллы жизни II}
const BYTE PE_5				=130;//{Автодок понизил баллы жизни}
const BYTE PE_6				=131;//{Автодок понизил баллы жизни II}
const BYTE PE_7				=132;//{Добротный Дермогон}
const BYTE PE_8				=133;//{Оружие усиленной убойной силы}
const BYTE PE_9				=134;//{Несчастливый}
const BYTE PE_10			=135;//{Книголюб (Книжный червь)}
const BYTE PE_11			=136;//{Ментор (Тьютор)}
const BYTE PE_12			=137;//{Выбит правый глаз}
const BYTE PE_13			=138;//{Выбит левый глаз}

const BYTE PE_HIDE_MODE		=139;//{Состояние скрытности (выкл/вкл)}

//Типы повреждений
const BYTE DAMAGE_TYPE_NORMAL	=0;
const BYTE DAMAGE_TYPE_LASER	=1;
const BYTE DAMAGE_TYPE_FIRE		=2;
const BYTE DAMAGE_TYPE_ELECTR	=3;
const BYTE DAMAGE_TYPE_EMP		=4;
const BYTE DAMAGE_TYPE_EXPLODE	=5;

//Предметы
//типы
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
const BYTE OBJ_NAME				=0;//{Имя итема}
const BYTE OBJ_INFO				=1;//{Инфо итема}

const BYTE OBJ_TIME_SHOW		=2;//{Время доставания объекта}
const BYTE OBJ_TIME_HIDE		=3;//{Время скрывания объекта}

const BYTE OBJ_DISTANCE_LIGHT	=4;//{Дистанция света}
const BYTE OBJ_INTENSITY_LIGHT	=5;//{Интенсивность света}

const BYTE OBJ_PASSED			=6;//{Проходимость объекта}
const BYTE OBJ_RAKED			=7;//{Простреливаемость объекта}
const BYTE OBJ_TRANSPARENT		=8;//{Прозрачность объекта} wall, glass, steam, energy
const BYTE OBJ_CAN_USE			=9;//{Объект можно использовать}
const BYTE OBJ_CAN_PICK_UP		=10;//{Объект можно поднять}
const BYTE OBJ_CAN_USE_ON_SMTH	=11;//{Объект можно применять на чем либо}

const BYTE OBJ_HIDDEN			=12;//{Скрытый}

const BYTE OBJ_WEIGHT			=13;//{Вес}
const BYTE OBJ_SIZE				=14;//{Объем в инвентаре}
const BYTE OBJ_TWOHANDS			=15;//{Двуручный объект}

const BYTE OBJ_PIC_MAP			=16;//{Индекс картинки на карте}
const BYTE OBJ_ANIM_ON_MAP		=17;//{Номер анимации на карте если есть}
const BYTE OBJ_PIC_INV			=18;//{Индекс картинки в инвентаре}
const BYTE OBJ_SOUND			=19;//{Индекс звука}

const BYTE OBJ_LIVETIME			=20;//{Начальное время жизни}

const BYTE OBJ_COST				=21;//{Базовая цена}
const BYTE OBJ_MATERIAL			=22;//{Материал предмета}
//----------------------------------------------- type==OBJ_TYPE_ARMOR
const BYTE OBJ_ARM_ANIM0_MALE	=30;//{Индекс анимации дюда male}
const BYTE OBJ_ARM_ANIM0_MALE2	=31;//{Индекс анимации дюда male дублер}
const BYTE OBJ_ARM_ANIM0_FEMALE	=32;//{Индекс анимации дюда female}
const BYTE OBJ_ARM_ANIM0_FEMALE2=33;//{Индекс анимации дюда female дублер}

const BYTE OBJ_ARM_AC			=40;//{Армор класс}
const BYTE OBJ_ARM_PERK			=41;//{Перк на броне}

const BYTE OBJ_ARM_DR_NORMAL	=50;//{Сопротивление нормальное}
const BYTE OBJ_ARM_DR_LASER		=51;//{Сопротивление лазеру}
const BYTE OBJ_ARM_DR_FIRE		=52;//{Сопротивление огню}
const BYTE OBJ_ARM_DR_PLASMA	=53;//{Сопротивление плазме}
const BYTE OBJ_ARM_DR_ELECTR	=54;//{Сопротивление электричеству}
const BYTE OBJ_ARM_DR_EMP		=55;//{Сопротивление емп}
const BYTE OBJ_ARM_DR_EXPLODE	=56;//{Сопротивление взрыву}

const BYTE OBJ_ARM_DT_NORMAL	=60;//{Порог повреждения нормальное}
const BYTE OBJ_ARM_DT_LASER		=61;//{Порог повреждения лазеру}
const BYTE OBJ_ARM_DT_FIRE		=62;//{Порог повреждения огню}
const BYTE OBJ_ARM_DT_PLASMA	=63;//{Порог повреждения плазме}
const BYTE OBJ_ARM_DT_ELECTR	=64;//{Порог повреждения электричеству}
const BYTE OBJ_ARM_DT_EMP		=65;//{Порог повреждения емп}
const BYTE OBJ_ARM_DT_EXPLODE	=66;//{Порог повреждения взрыву}
//----------------------------------------------- type==OBJ_TYPE_CONTAINER
const BYTE OBJ_CONT_SIZE		=30;//{Объем контейнера}
const BYTE OBJ_CONT_FLAG		=31;//{Флаг контейнера}
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
const BYTE OBJ_WEAP_ANIM1		=30;//{Индекс анимации объекта}

const BYTE OBJ_WEAP_TIME_ACTIV	=31;//{Время подготовки к использованию}
const BYTE OBJ_WEAP_TIME_UNACTIV=32;//{Время отмены использования}

const BYTE OBJ_WEAP_VOL_HOLDER	=33;//{Емкость основной обоймы}
const BYTE OBJ_WEAP_CALIBER		=34;//{Калибр у основной обоймы}
const BYTE OBJ_WEAP_VOL_HOLDER_E=35;//{Емкость дополнительной обоймы}
const BYTE OBJ_WEAP_CALIBER_E	=36;//{Калибр у дополнительной обоймы}

const BYTE OBJ_WEAP_CR_FAILTURE	=37;//{Критическая неудача при использовании}

const BYTE OBJ_WEAP_TYPE_ATTACK	=38;//{Тип атаки оружия}

const BYTE OBJ_WEAP_COUNT_ATTACK=39;//{Количество атак} | 0 - нет | 1 - PA | 2 - PA && SA | 3 - PA && SA && TA |

const BYTE OBJ_WEAP_PA_SKILL	=40;//{Скилл отвечающий за использование}
const BYTE OBJ_WEAP_PA_HOLDER	=41;//{Используемая обойма}
const BYTE OBJ_WEAP_PA_PIC		=42;//{Рисунок использования}
const BYTE OBJ_WEAP_PA_DMG_MIN	=43;//{Минимальное повреждения}
const BYTE OBJ_WEAP_PA_DMG_MAX	=44;//{Максимальное повреждение}
const BYTE OBJ_WEAP_PA_MAX_DIST	=45;//{Максимальная дистанция}
const BYTE OBJ_WEAP_PA_EFF_DIST	=46;//{Эффективная дистанция}
const BYTE OBJ_WEAP_PA_ANIM2	=47;//{Анимация атаки}
const BYTE OBJ_WEAP_PA_TIME		=48;//{Базовое время атаки}
const BYTE OBJ_WEAP_PA_AIM		=49;//{Наличие у атаки прицельного выстрела}
const BYTE OBJ_WEAP_PA_ROUND	=50;//{Расход патронов за атаку}
const BYTE OBJ_WEAP_PA_REMOVE	=51;//{Удаление предмета после атаки}

const BYTE OBJ_WEAP_SA_SKILL	=53;//{Скилл отвечающий за использование}
const BYTE OBJ_WEAP_SA_HOLDER	=54;//{Используемая обойма}
const BYTE OBJ_WEAP_SA_PIC		=55;//{Рисунок использования}
const BYTE OBJ_WEAP_SA_DMG_MIN	=56;//{Минимальное повреждения}
const BYTE OBJ_WEAP_SA_DMG_MAX	=57;//{Максимальное повреждение}
const BYTE OBJ_WEAP_SA_MAX_DIST	=58;//{Максимальная дистанция}
const BYTE OBJ_WEAP_SA_EFF_DIST	=59;//{Эффективная дистанция}
const BYTE OBJ_WEAP_SA_ANIM2	=60;//{Анимация атаки}
const BYTE OBJ_WEAP_SA_TIME		=61;//{Базовое время атаки}
const BYTE OBJ_WEAP_SA_AIM		=62;//{Наличие у атаки прицельного выстрела}
const BYTE OBJ_WEAP_SA_ROUND	=63;//{Расход патронов за атаку}
const BYTE OBJ_WEAP_SA_REMOVE	=64;//{Удаление предмета после атаки}

const BYTE OBJ_WEAP_TA_SKILL	=66;//{Скилл отвечающий за использование}
const BYTE OBJ_WEAP_TA_HOLDER	=67;//{Используемая обойма}
const BYTE OBJ_WEAP_TA_PIC		=68;//{Рисунок использования}
const BYTE OBJ_WEAP_TA_DMG_MIN	=69;//{Минимальное повреждения}
const BYTE OBJ_WEAP_TA_DMG_MAX	=70;//{Максимальное повреждение}
const BYTE OBJ_WEAP_TA_MAX_DIST	=71;//{Максимальная дистанция}
const BYTE OBJ_WEAP_TA_EFF_DIST	=72;//{Эффективная дистанция}
const BYTE OBJ_WEAP_TA_ANIM2	=73;//{Анимация атаки}
const BYTE OBJ_WEAP_TA_TIME		=74;//{Базовое время атаки}
const BYTE OBJ_WEAP_TA_AIM		=75;//{Наличие у атаки прицельного выстрела}
const BYTE OBJ_WEAP_TA_ROUND	=76;//{Расход патронов за атаку}
const BYTE OBJ_WEAP_TA_REMOVE	=77;//{Удаление предмета после атаки}
//----------------------------------------------- type==OBJ_TYPE_AMMO
const BYTE OBJ_AMMO_CALIBER		=30;//{Калибр}
const BYTE OBJ_AMMO_TYPE_DAMAGE	=31;//{Тип повреждения}
const BYTE OBJ_AMMO_QUANTITY	=32;//{Количество}
const BYTE OBJ_AMMO_AC			=33;//{AC}
const BYTE OBJ_AMMO_DR			=34;//{DR}
const BYTE OBJ_AMMO_DM			=35;//{Множитель для DR}
const BYTE OBJ_AMMO_DD			=36;//{Делитель для DR}
//----------------------------------------------- type==OBJ_TYPE_MISC

//----------------------------------------------- type==OBJ_TYPE_KEY
//const BYTE OBJ_KEY_DOOR			=30;//{От какой двери ключ}
//----------------------------------------------- OBJ_TYPE_DOOR
const BYTE OBJ_DOOR_ANIMATION		=30;//{Анимация двери}
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
			WORD AmmoPID;      // Прототип заряженных патронов.
			WORD AmmoCount;    // Количество заряженных патронов
			WORD AmmoPID_ext;      // Прототип заряженных патронов.
			WORD AmmoCount_ext;    // Количество заряженных патронов
		} ITEM_WEAPON;

		struct
		{
			WORD  Count;        // кол-во патронов в обойме
		} ITEM_AMMO;

		struct
		{
			WORD  Count;        // Текущее количество зарядов (т.е., насколько заряжен).
		} ITEM_MISC;

		struct
		{
			DWORD   doorID;     // Код двери, которую открывает ключ
		} ITEM_KEY;

		struct
		{
			DWORD   doorID;     // Код двери
		} ITEM_DOOR;

		struct
		{
			DWORD   doorID;     // Код двери
		} ITEM_CRAFTING;

		struct
		{
			DWORD   MapID;      // Номер карты, на которую попадает игрок.
			WORD    PosX;       // Позиция игрока на новой карте
			WORD    PosY;
			BYTE    dir;        // Ориентация игрока
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

	CrTYPE base_type; //базовый тип криттера

	MapTYPE map; //номер карты
	HexTYPE x; //позиция в хексах
	HexTYPE y;

	WORD world_x;
	WORD world_y;

	BYTE ori; //ориентация
	BYTE look; //!Cvet дистанция видимости
	char name[MAX_NAME+1]; //имя
	char cases[5][MAX_NAME+1]; //имя склоняемые варианты

	BYTE access; //!Cvet уровень доступа к игре

	DWORD start_bt; //!Cvet
	int break_time; //!Cvet

	CrID id; //!Cvet Идентификатор
	CrID idchannel; //!Cvet Идентификатор канала к которому подключен игрок
	char login[MAX_LOGIN+1]; //!Cvet логин
	char pass[MAX_LOGIN+1]; //!Cvet пароль
	WORD st[ALL_STATS ]; //!Cvet статы 4-х значный XXXX
	WORD sk[ALL_SKILLS]; //!Cvet скилы 3-х значный XXX
	BYTE pe[ALL_PERKS ]; //!Cvet перки 1-а значный X

	BYTE cond; //!Cvet состояние криттера
	BYTE cond_ext; //!Cvet дополнительное состояние криттера
	WORD flags; //!Cvet флаги криттера

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
// WORD s[MAX_STATS] //статы
//////////////////////////////////////////////////////////////////////////
#define NETMSG_LOGMSG 4
//////////////////////////////////////////////////////////////////////////
// ответ сервера на NETMSG_NAME - имя не принято
// params:
// BYTE LogMsg
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
// WORD id_stat
// WORD id_stat_arm
// HexTYPE x
// HexTYPE y
// BYTE ori
// BYTE st[ST_GENDER] пол
// BYTE cond
// BYTE cond_ext
// WORD flags
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
// BYTE how_say
// WORD len
// char[len] str

#define NETMSG_CRITTERTEXT 9
//////////////////////////////////////////////////////////////////////////
// текст который надо нарисовать над криттером
// params:
// CrID id
// WORD len
// char[len] str

#define NETMSG_DIR 10 //!Cvet edit
//////////////////////////////////////////////////////////////////////////
// сообщение направления
// params:
// BYTE dir

#define NETMSG_CRITTER_DIR 11 //!Cvet
//////////////////////////////////////////////////////////////////////////
// направление криттера
// params:
// CrID id
// BYTE dir

#define NETMSG_SEND_MOVE 12
//////////////////////////////////////////////////////////////////////////
// передача на сервер направления для шага чезена
// params:
// BYTE dir
// BYTE move_params - dir,how_move,stop&go
#define NETMSG_CRITTER_MOVE 13
//////////////////////////////////////////////////////////////////////////
// передача направления для других криттеров
// params:
// CrID id
// BYTE move_params - dir,how_move,stop&go
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
// BYTE dir

#define NETMSG_ALL_PARAMS 16 //!Cvet
//////////////////////////////////////////////////////////////////////////
// передача параметров
// params:
// BYTE type_param - тип параметра
// BYTE all_send - всего статов отправлено
// BYTE num_param - номер параметра
// WORD or BYTE count_param - его кол-во
#define NETMSG_PARAM 17 //!Cvet
//////////////////////////////////////////////////////////////////////////
// передача отдельного параметра
// params:
// BYTE type_param - тип параметра
// WORD num_param - номер параметра
// WORD or BYTE count_param - его кол-во

#define NETMSG_ADD_OBJECT 18 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал о том что ктото спрятал оружие
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
// сигнал о том что ктото спрятал оружие
// params:
// DWORD id; -dyn
#define NETMSG_WEAR_OBJECT 20 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал о износе предмета
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
// BYTE skill; -номер скилла
// DWORD target_id; -игрок или предмет
// BYTE ori; -напровление применения

#define NETMSG_CRITTER_ACTION 28 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал о том что криттер производит какоето действие
// params:
// CrID crid; -ид криттера
// BYTE ori; -направление действия
// BYTE type_action; -номер действия -последняя буква
// BYTE type; -тип криттера -название криттера
// BYTE weap; -номер аним оружия в руках -предпоследняя буква
// WORD number_of_action; -что за действие передано

#define NETMSG_SEND_TALK_NPC 29 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сигнал игрока о беседе
// CrID id_talk - ид нпц с которым разговариваешь
// BYTE variant - вариант ответа
#define NETMSG_TALK_NPC 30 //!Cvet
//////////////////////////////////////////////////////////////////////////
// ответ нпц -> игроку
// BYTE all_answers - всего вариантов ответа, если 0 - диалог закончен
// DWORD main_text - текст НПЦ
// DWORD answ_text в кол-ве all_answers - варианты ответа

#define NETMSG_SEND_GET_TIME 31 //!Cvet
//////////////////////////////////////////////////////////////////////////
// запрос на игровое время
#define NETMSG_GAME_TIME 32 //!Cvet
//////////////////////////////////////////////////////////////////////////
// высылается игроку пройденное игровое время с начала игровых суток
// WORD minutes - пройденное игровое время
// BYTE day - игровой день
// BYTE month - игровой месяц
// WORD year - игровой год

#define NETMSG_LOADMAP 33 //!Cvet
//////////////////////////////////////////////////////////////////////////
// высылается игроку команда загрузки карты
// WORD num_map - номер карты
// WORD num_version - номер версии карты
// на будущее !!!!!!!!
// HexTYPE width - ширина по Х
// HexTYPE height - ширина по У
#define NETMSG_MAP 34 //!Cvet
//////////////////////////////////////////////////////////////////////////
// карта
// WORD num_map - номер карты
#define NETMSG_SEND_GIVE_ME_MAP 35 //!Cvet
//////////////////////////////////////////////////////////////////////////
// запрос на высылку карты
// WORD num_map - номер карты
#define NETMSG_SEND_LOAD_MAP_OK 36 //!Cvet
//////////////////////////////////////////////////////////////////////////
// сообщение о удачной загрузке карты

//GLOBAL MAP
const BYTE GM_INFO_CITIES	=BIN8(00000001);
const BYTE GM_INFO_CRITS	=BIN8(00000010);
const BYTE GM_INFO_PARAM	=BIN8(00000100);
const BYTE GM_INFO_ALL		=BIN8(00000111);
//const BYTE GM_ADDINFO_CITY	=BIN8(00001000);
//const BYTE GM_DELINFO_CITY	=BIN8(00010000);
#define NETMSG_SEND_GIVE_GLOBAL_INFO 37 //!Cvet
//////////////////////////////////////////////////////////////////////////
// запрос инфы по глобалу
// BYTE info_flag;
#define NETMSG_GLOBAL_INFO 38 //!Cvet
//////////////////////////////////////////////////////////////////////////
// инфа по глобалу
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
const BYTE GM_RULE_COMMAND_SETMOVE		=1; //+r-a*x,y//двигаться
const BYTE GM_RULE_COMMAND_STOP			=2; //+r-a//остановиться
const BYTE GM_RULE_COMMAND_TOLOCAL		=3; //+r-a*num_city,num_map//перейти на локльную карту
const BYTE GM_RULE_COMMAND_KICKCRIT		=4; //+r-a*cr_id//выкинуть крита из группы
const BYTE GM_RULE_COMMAND_LOCKCRIT		=5; //+r-a*cr_id//блокировать крита
const BYTE GM_RULE_COMMAND_UNLOCKCRIT	=6; //+r-a*cr_id//разблокировать крита
const BYTE GM_RULE_COMMAND_IGNORECRIT	=7; //+r+a*cr_id//игнорировать крита
const BYTE GM_RULE_COMMAND_LISTENCRIT	=8; //+r+a*cr_id//слушать крита
const BYTE GM_RULE_COMMAND_READY		=9; //+r+a//готовность к высадке на локальную карту
const BYTE GM_RULE_COMMAND_TOGEMMATE	=10;//-r+a//отсоединиться от группы
const BYTE GM_RULE_COMMAND_SETSPEED		=11;//+r-a*speed//задать скорость группы
const BYTE GM_RULE_COMMAND_ADDPREPCRIT	=12;//+r-a*cr_id//добавит в список крита
const BYTE GM_RULE_COMMAND_DELPREPCRIT	=13;//+r-a*cr_id//удалить со списка крита
#define NETMSG_SEND_RULE_GLOBAL 39 //!Cvet
//////////////////////////////////////////////////////////////////////////
// управление группой
// BYTE command;
// DWORD param1;
// DWORD param2;

//states

#define STATE_CONN			0
#define STATE_DROPLINK		1
#define STATE_DISCONNECT	2
#define STATE_GAME			3

//послан логин и пароль чузена
#define STATE_LOGINOK		5 //!Cvet

#define STATE_INIT_NET		6


#endif //__NETPROTO_H__