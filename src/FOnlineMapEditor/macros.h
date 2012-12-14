#ifndef MACROS_INCLUDED
#define MACROS_INCLUDED

#define VER_FALLOUT1 0x00000013
#define VER_FALLOUT2 0x10000014

#define ZLIB_BUFF_SIZE 0x10000
#define GZIP_MODE1 0x0178
#define GZIP_MODE2 0xDA78

#define TILE_SIZE_X 80
#define TILE_SIZE_Y 36
#define TILE_LEFT_CORNER_X 0
#define TILE_LEFT_CORNER_Y 11
#define TILE_UP_CORNER_X 47
#define TILE_UP_CORNER_Y 0
#define TILE_RIGHT_CORNER_X 79
#define TILE_RIGHT_CORNER_Y 23
#define TILE_DOWN_CORNER_X 35
#define TILE_DOWN_CORNER_Y 35

#define BRIGHTNESS_FACTOR 5
#define BRIGHTNESS_FINETUNE 0
#define R_FINETUNE 0
#define G_FINETUNE 0
#define B_FINETUNE 0

#define SHOW_LOCAL               0x00
#define SHOW_WORLD               0x01

#define FRM_HEX			 0x00
#define FRM_SCR_BLOCK		 0x01
#define FRM_HEX_BLOCK		 0x02

#define item_ID			 0x00
#define critter_ID		 0x01
#define scenery_ID		 0x02
#define wall_ID			 0x03
#define tile_ID			 0x04
#define misc_ID			 0x05
#define inven_ID 		 0x06
#define intrface_ID		 0x07

#define EG_blockID               0x07
#define SAI_blockID              0x08
#define wall_blockID             0x09
#define obj_blockID              0x0a
#define light_blockID            0x0b
#define scroll_blockID           0x0c
#define obj_self_blockID         0x0d

#define floor_ID		 0x04
#define roof_ID			 0x06

#define ALL_LEVELS     		 0x00
#define LEVEL1     		 0x00
#define LEVEL2     		 0x01
#define LEVEL3     		 0x02

#define SELECT_NONE              0x00
#define SELECT_ROOF              0x01
#define SELECT_OBJ               0x02
#define SELECT_FLOOR             0x03
#define DRAW_ROOF                0x04
#define DRAW_OBJ                 0x05
#define DRAW_FLOOR               0x06
#define MOVE_OBJ                 0x07

//----Full types
#define OArmor                   0x00
#define OContainer               0x01
#define ODrug                    0x02
#define OWeapon                  0x03
#define OAmmo                    0x04
#define OMiscItem                0x05
#define OKey                     0x06
#define OCritter                 0x07
#define OPortal                  0x08
#define OStairs                  0x09
#define OElevator                0x0a
#define OLadderBottom            0x0b
#define OLadderUp                0x0c
#define OGenericScenery          0x0d
#define OWalls                   0x0e
#define OExitGrid                0x0f
#define OGenericMisc             0x10

//----Subtypes
#define OSArmor          0
#define OSContainer      1
#define OSDrug           2
#define OSWeapon         3
#define OSAmmo           4
#define OSMiscItem       5
#define OSKey            6

#define OSPortal         0
#define OSStairs         1
#define OSElevator       2
#define OSLadderBottom   3
#define OSLadderTop      4
#define OSGenericScenery 5

#define NONE_SELECTED    0xFFFFFFFF

#define OBJ_DESELECTED   0x00
#define OBJ_SELECTED     0x01

#define NAV_SIZE_X 80
#define NAV_SIZE_Y 62

#define ROTATE_CW        0x00
#define ROTATE_CCW       0x01

#endif
