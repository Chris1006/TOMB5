#pragma once


void inject_gameflow();

void DoGameflow();
int GetCampaignCheatValue();//somewhere in specific 
int TitleOptions();

#define DoLevel	( (void(__cdecl*)(unsigned char, unsigned char)) 0x00435F60 )
#define DoTitle	( (void(__cdecl*)(unsigned char, unsigned char)) 0x00435C70 )
#define DoOptions	( (void(__cdecl*)()) 0x004ADF90 )
#define do_dels_cutseq_selector	( (void(__cdecl*)()) 0x004364B0 )
#define DoCredits	( (int(__cdecl*)()) 0x004927C0 )//somewhere in specific
#define SuperShowLogo	( (void(__cdecl*)()) 0x004C9190 )//somewhere in specific 
#define SetFade	( (void(__cdecl*)(int, int)) 0x004CA720 )//somewhere in specific 

enum gf_languages
{
	LNG_ENGLISH,
	LNG_FRENCH,
	LNG_GERMAN,
	LNG_ITALIAN,
	LNG_SPANISH,
	LNG_US,
	LNG_JAPAN,
	LNG_DUTCH,
	NUM_GF_LANGUAGES
};

enum gf_commands
{
	GF_FMV = 0x80,
	GF_LEVEL = 0x81,
	GF_TITLE_LEVEL = 0x82,
	GF_LEVEL_DATA_END = 0x83,
	GF_CUT = 0x84,

	GF_RESIDENTCUT1 = 0x85,
	GF_RESIDENTCUT2 = 0x86,
	GF_RESIDENTCUT3 = 0x87,
	GF_RESIDENTCUT4 = 0x88,

	GF_LAYER1 = 0x89,
	GF_LAYER2 = 0x8A,

	GF_UV_ROTATE = 0x8B,
	GF_LEGEND = 0x8C,
	GF_LENS_FLARE = 0x8D,
	GF_MIRROR = 0x8E,
	GF_FOG = 0x8F,
	GF_ANIMATING_MIP = 0x90,
	GF_RESET_HUB = 0x91,

	GF_KEY_ITEM1 = 0x92,
	GF_KEY_ITEM2 = 0x93,
	GF_KEY_ITEM3 = 0x94,
	GF_KEY_ITEM4 = 0x95,
	GF_KEY_ITEM5 = 0x96,
	GF_KEY_ITEM6 = 0x97,
	GF_KEY_ITEM7 = 0x98,
	GF_KEY_ITEM8 = 0x99,
	GF_KEY_ITEM9 = 0x9A,
	GF_KEY_ITEM10 = 0x9B,
	GF_KEY_ITEM11 = 0x9C,
	GF_KEY_ITEM12 = 0x9D,

	GF_PUZZLE_ITEM1 = 0x9E,
	GF_PUZZLE_ITEM2 = 0x9F,
	GF_PUZZLE_ITEM3 = 0xA0,
	GF_PUZZLE_ITEM4 = 0xA1,
	GF_PUZZLE_ITEM5 = 0xA2,
	GF_PUZZLE_ITEM6 = 0xA3,
	GF_PUZZLE_ITEM7 = 0xA4,
	GF_PUZZLE_ITEM8 = 0xA5,
	GF_PUZZLE_ITEM9 = 0xA6,
	GF_PUZZLE_ITEM10 = 0xA7,
	GF_PUZZLE_ITEM11 = 0xA8,
	GF_PUZZLE_ITEM12 = 0xA9,

	GF_PICKUP_ITEM1 = 0xAA,
	GF_PICKUP_ITEM2 = 0xAB,
	GF_PICKUP_ITEM3 = 0xAC,
	GF_PICKUP_ITEM4 = 0xAD,

	GF_EXAMINE1 = 0xAE,
	GF_EXAMINE2 = 0xAF,
	GF_EXAMINE3 = 0xB0,

	GF_KEY_ITEM1_COMBO1 = 0xB1,
	GF_KEY_ITEM1_COMBO2 = 0xB2,
	GF_KEY_ITEM2_COMBO1 = 0xB3,
	GF_KEY_ITEM2_COMBO2 = 0xB4,
	GF_KEY_ITEM3_COMBO1 = 0xB5,
	GF_KEY_ITEM3_COMBO2 = 0xB6,
	GF_KEY_ITEM4_COMBO1 = 0xB7,
	GF_KEY_ITEM4_COMBO2 = 0xB8,
	GF_KEY_ITEM5_COMBO1 = 0xB9,
	GF_KEY_ITEM5_COMBO2 = 0xBA,
	GF_KEY_ITEM6_COMBO1 = 0xBB,
	GF_KEY_ITEM6_COMBO2 = 0xBC,
	GF_KEY_ITEM7_COMBO1 = 0xBD,
	GF_KEY_ITEM7_COMBO2 = 0xBE,
	GF_KEY_ITEM8_COMBO1 = 0xBF,
	GF_KEY_ITEM8_COMBO2 = 0xC0,

	GF_PUZZLE_ITEM1_COMBO1 = 0xC1,
	GF_PUZZLE_ITEM1_COMBO2 = 0xC2,
	GF_PUZZLE_ITEM2_COMBO1 = 0xC3,
	GF_PUZZLE_ITEM2_COMBO2 = 0xC4,
	GF_PUZZLE_ITEM3_COMBO1 = 0xC5,
	GF_PUZZLE_ITEM3_COMBO2 = 0xC6,
	GF_PUZZLE_ITEM4_COMBO1 = 0xC7,
	GF_PUZZLE_ITEM4_COMBO2 = 0xC8,
	GF_PUZZLE_ITEM5_COMBO1 = 0xC9,
	GF_PUZZLE_ITEM5_COMBO2 = 0xCA,
	GF_PUZZLE_ITEM6_COMBO1 = 0xCB,
	GF_PUZZLE_ITEM6_COMBO2 = 0xCC,
	GF_PUZZLE_ITEM7_COMBO1 = 0xCD,
	GF_PUZZLE_ITEM7_COMBO2 = 0xCE,
	GF_PUZZLE_ITEM8_COMBO1 = 0xCF,
	GF_PUZZLE_ITEM8_COMBO2 = 0xD0,

	GF_PICKUP_ITEM1_COMBO1 = 0xD1,
	GF_PICKUP_ITEM1_COMBO2 = 0xD2,
	GF_PICKUP_ITEM2_COMBO1 = 0xD3,
	GF_PICKUP_ITEM2_COMBO2 = 0xD4,
	GF_PICKUP_ITEM3_COMBO1 = 0xD5,
	GF_PICKUP_ITEM3_COMBO2 = 0xD6,
	GF_PICKUP_ITEM4_COMBO1 = 0xD7,
	GF_PICKUP_ITEM4_COMBO2 = 0xD8,

	GF_GIVE_ITEM_AT_STARTUP = 0xD9,
	GF_LOSE_ITEM_AT_STARTUP = 0xDA
};

enum gf_level_options
{
	GF_LVOP_YOUNG_LARA = (1 << 0),				// 0x0001
	GF_LVOP_WEATHER = (1 << 1),					// 0x0002
	GF_LVOP_HORIZON = (1 << 2),					// 0x0004
	GF_LVOP_LAYER1_USED = (1 << 3),				// 0x0008
	GF_LVOP_LAYER2_USED = (1 << 4),				// 0x0010
	GF_LVOP_STARFIELD = (1 << 5),				// 0x0020
	GF_LVOP_LIGHTNING = (1 << 6),				// 0x0040
	GF_LVOP_TRAIN = (1 << 7),					// 0x0080
	GF_LVOP_PULSE = (1 << 8),					// 0x0100
	GF_LVOP_COL_ADD_HORIZON = (1 << 9),			// 0x0200
	GF_LVOP_RESETHUB_USED = (1 << 10),			// 0x0400
	GF_LVOP_LENSFLARE_USED = (1 << 11),			// 0x0800
	GF_LVOP_TIMER = (1 << 12),					// 0x1000
	GF_LVOP_MIRROR_USED = (1 << 13),			// 0x2000
	GF_LVOP_REMOVE_AMULET = (1 << 14),			// 0x4000
	GF_LVOP_NO_LEVEL = (1 << 15)				// 0x8000
};

enum gf_tr5_levels
{
	LVL5_TITLE,
	LVL5_STREETS_OF_ROME,
	LVL5_TRAJAN_MARKETS,
	LVL5_COLOSSEUM,
	LVL5_BASE,
	LVL5_SUBMARINE,
	LVL5_DEEPSEA_DIVE,
	LVL5_SINKING_SUBMARINE,
	LVL5_GALLOWS_TREE,
	LVL5_LABYRINTH,
	LVL5_OLD_MILL,
	LVL5_THIRTEENTH_FLOOR,
	LVL5_ESCAPE_WITH_THE_IRIS,
	LVL5_SECURITY_BREACH,
	LVL5_RED_ALERT,
	LVL5_GIBBY_LEVEL,
	LVL5_DEL_LEVEL,
	LVL5_TOM_LEVEL,

	NUM_TR5_LEVELS
};

enum gf_tr5_strings
{
	STR_SEVERAL_HOURS_LATER,
	STR_GIBBYS_LEVEL,
	STR_DELS_LEVEL,
	STR_TOMS_LEVEL,
	STR_THE_13TH_FLOOR,
	STR_ESCAPE_WITH_THE_IRIS,
	STR_SECURITY_BREACH,
	STR_RED_ALERT,
	STR_THE_BASE,
	STR_THE_SUBMARINE,
	STR_DEEPSEA_DIVE,
	STR_SINKING_SUBMARINE,
	STR_STREETS_OF_ROME,
	STR_TRAJANS_MARKETS,
	STR_THE_COLOSSEUM,
	STR_GALLOWS_TREE,
	STR_LABYRINTH,
	STR_OLD_MILL,
	STR_SELECT_CUTSCENE,
	STR_JOBY_CRANE_CUT,
	STR_RICH_CUT_2,
	STR_JOBY_CUT_2,
	STR_RICH_CUT_1,
	STR_RICH_CUT_3,
	STR_JOBY_CUT_3,
	STR_ANDY1,
	STR_RICH1,
	STR_ANDY2,
	STR_JOBY4,
	STR_ANDREA1,
	STR_ANDREA2,
	STR_JOBY5,
	STR_ANDY3,
	STR_JOBY9,
	STR_JOBY10,
	STR_RICHCUT4,
	STR_ANDY4,
	STR_ANDY4B,
	STR_ANDREA3,
	STR_ANDREA3B,
	STR_ANDY5,
	STR_JOBY6,
	STR_JOBY8,
	STR_ANDY6,
	STR_ANDYPEW,
	STR_ANDY7,
	STR_COSSACK,
	STR_ANDY9,
	STR_ANDY8,
	STR_ANDY10,
	STR_JOBY7,
	STR_ANDREA4,
	STR_MONK2,
	STR_SWAMPY,
	STR_ANDY11,
	STR_ENTER_COMBINATION,
	STR_UNLIMITED,
	STR_USE,
	STR_CHOOSE_AMMO,
	STR_CHOOSE_WEAPON_MODE,
	STR_COMBINE,
	STR_SEPARATE,
	STR_EQUIP,
	STR_COMBINE_WITH,
	STR_LOAD_GAME,
	STR_SAVE_GAME,
	STR_EXAMINE,
	STR_MORE,
	STR_SELECT_AMMO,
	STR_SELECT_OPTION,
	STR_COMBINE_BIS,
	STR_PUSH_KEYPAD,
	STR_MOVEMENT,
	STR_PREVIOUS_NEXT_BACK,
	STR_GRAPPLING_GUN,
	STR_GRAPPLING_GUN_AMMO,
	STR_CHLOROFORM_SOAKED_CLOTH,
	STR_CLOTH,
	STR_BOTTLE_OF_CHLOROFORM,
	STR_UZI,
	STR_PISTOLS,
	STR_SHOTGUN,
	STR_REVOLVER,
	STR_REVOLVER_LASERSIGHT,
	STR_DESERTEAGLE,
	STR_DESERTEAGLE_LASERSIGHT,
	STR_DESERTEAGLE_AMMO,
	STR_HK,
	STR_HK_SILENCED,
	STR_SHOTGUN_NORMAL_AMMO,
	STR_SHOTGUN_WIDESHOT_AMMO,
	STR_HK_SNIPER_MODE,
	STR_HK_BURST_MODE,
	STR_HK_RAPID_MODE,
	STR_HK_AMMO,
	STR_REVOLVER_AMMO,
	STR_UZI_AMMO,
	STR_PISTOL_AMMO,
	STR_LASERSIGHT,
	STR_SILENCER,
	STR_LARGE_MEDIPACK,
	STR_SMALL_MEDIPACK,
	STR_BINOCULARS,
	STR_HEADSET,
	STR_FLARES,
	STR_TIMEX_TMX,
	STR_LOAD,
	STR_SAVE,
	STR_CROWBAR,
	STR_GARDEN_KEY,
	STR_SATURN_SYMBOL,
	STR_GOLDEN_KEY_1,
	STR_GOLDEN_KEY_2,
	STR_MERCURY_STONE,
	STR_TELEPORTER_DISC,
	STR_FUSE,
	STR_SILVER_KEY,
	STR_BRONZE_KEY,
	STR_SWIPE_CARD,
	STR_HAMMER,
	STR_ACCESS_CODE_DISC,
	STR_IRIS_LAB_ACCESS,
	STR_HIGH_LEVEL_ACCESS_CARD,
	STR_ID_ACCESS_CARD,
	STR_IRIS_ARTEFACT,
	STR_LOW_LEVEL_ACCESS_CARD,
	STR_RESTROOM_ACCESS_CARD,
	STR_HELIPAD_ACCESS_KEY,
	STR_KEY_BIT_LEFT,
	STR_KEY_BIT_RIGHT,
	STR_VALVE_WHEEL,
	STR_GOLDEN_COIN,
	STR_MARS_SYMBOL,
	STR_VENUS_SYMBOL,
	STR_GEMSTONE,
	STR_PHILOSOPHERS_STONE,
	STR_COLOSSEUM_KEY_1,
	STR_COLOSSEUM_KEY_2,
	STR_GEMSTONE_PIECE,
	STR_SPEAR_OF_DESTINY,
	STR_HEART,
	STR_CATAPULT,
	STR_IRON_CLAPPER,
	STR_RUBBER_TUBE,
	STR_PITCHFORK,
	STR_BONE_DUST,
	STR_BESTIARY,
	STR_CHALK,
	STR_SILVER_COIN,
	STR_SUIT_CONSOLE,
	STR_SUIT_BATTERY,
	STR_AQUALUNG,
	STR_SUIT_CONSOLE_BIS,
	STR_BATTERY_PLUS,
	STR_BATTERY_MINUS,
	STR_CHAFF_FLARES,
	STR_NITROGEN_CANISTER,
	STR_OXYGEN_CANISTER,
	STR_YES,
	STR_NO,
	STR_LOAD_GAME_BIS,
	STR_SAVE_GAME_BIS,
	STR_PAUSED,
	STR_SELECT_GAME_TO_LOAD,
	STR_SELECT_GAME_TO_SAVE,
	STR_SPECIAL_FEATURES,
	STR_MOVIE_TRAILER,
	STR_STORYBOARDS_PART_1,
	STR_NEXT_GENERATION_CONCEPT_ART,
	STR_STORYBOARDS_PART_2,
	STR_NEXT_GENERATION_PREVIEW,
	STR_GAME_OVER,
	STR_SAVE_GAME_BIS_BIS,
	STR_EXIT_TO_TITLE,
	STR_DEMO_MODE,
	STR_CURRENT_LOCATION,
	STR_STATISTICS,
	STR_DISTANCE_TRAVELLED,
	STR_AMMO_USED,
	STR_SECRETS_FOUND,
	STR_LOCATION,
	STR_HEALTH_PACKS_USED,
	STR_TIME_TAKEN,
	STR_DAYS,
	STR_OF,
	STR_SECRETS_NUM,
	STR_ACTION,
	STR_DRAW_WEAPON,
	STR_JUMP,
	STR_ROLL,
	STR_WALK,
	STR_LOOK,
	STR_DUCK,
	STR_DASH,
	STR_INVENTORY,
	STR_SELECT,
	STR_CANCEL,
	STR_BACK,
	STR_OK,
	STR_CANCEL_BIS,
	STR_PSX_GAME_ID, // BESLES-03331 (IT, UK), BASLUS-01311 (US), BESLES-03334 (DE), BESLES-03333 (FR), BESLES-03336 (ES)
	STR_INSERT_MEMCARD_INTO_SLOT_1,
	STR_CHECKING_MEMORY_CARD,
	STR_MEMCARD_UNFORMATTED_FORMAT_IT,
	STR_LOADING_DATA_DO_NOT_REMOVE,
	STR_SAVING_DATA_DO_NOT_REMOVE,
	STR_FORMATTING_MEMCARD_DO_NOT_REMOVE,
	STR_OVERWRITE_ON_MEMCARD,
	STR_MEMCARD_IS_UNFORMATTED_INSERT_FORMATTED,
	STR_MEMCARD_INSUFFICIENT_FREE_BLOCKS, // + INSERT MEMCARD WITH AT LEAST 2 FREE BLOCKS
	STR_THERE_ARE_NO_SAVEGAMES,
	STR_LOADING,
	STR_LOAD_OK,
	STR_SAVED_OK,
	STR_FORMAT_OK,
	STR_LOAD_FAILED,
	STR_SAVE_FAILED,
	STR_FORMAT_FAILED,
	STR_EMPTY,
	STR_CONTROLLER_REMOVED,
	STR_RESUME,
	STR_QUIT,
	STR_GAME_SETTINGS,
	STR_CONTROL_OPTIONS,
	STR_CONFIRM_QUIT,
	STR_SCREEN_ADJUST,
	STR_SFX_VOLUME,
	STR_MUSIC_VOLUME,
	STR_USE_DIRECTIONAL_BUTTONS,
	STR_CONFIGURE_CONTROLS,
	STR_CONFIGURATION,
	STR_VIBRATION_ON,
	STR_VIBRATION_OFF,
	STR_MANUAL_TARGETING,
	STR_AUTOMATIC_TARGETING,
	STR_ELLIPSIS,
	STR_ELLIPSIS_BIS,
	STR_PROGRAMMERS,
	STR_ADDITIONAL_PROGRAMMERS,
	STR_AI_PROGRAMMERS,
	STR_ANIMATORS,
	STR_LEVEL_DESIGNERS,
	STR_FMV_SEQUENCES,
	STR_MUSIC_AND_SOUND_FX,
	STR_ORIGINAL_STORY,
	STR_SCRIPT,
	STR_PRODUCER,
	STR_QA,
	STR_EXECUTIVE_PRODUCERS,
	STR_ADDITIONAL_SOUND_FX,
	STR_RUBBISH,
	STR_PC_PROGRAMMER,
	STR_PROGRAMMERS_BIS,
	STR_ADDITIONAL_PROGRAMMING,
	STR_RESUME_BIS,
	STR_OPTIONS,
	STR_EMPTY_SLOT,
	STR_MUSIC_VOLUME_BIS,
	STR_SFX_VOLUME_BIS,
	STR_SOUND_QUALITY,
	STR_LOW,
	STR_MEDIUM,
	STR_HIGH,
	STR_TARGETING,
	STR_MANUAL,
	STR_AUTOMATIC,
	STR_GRAPHICS_ADAPTER,
	STR_OUTPUT_SETTINGS,
	STR_OUTPUT_RESOLUTION,
	STR_TEXTURE_BIT_DEPTH,
	STR_OK_BIS,
	STR_CANCEL_BIS_BIS,
	STR_HARDWARE_ACCELERATION,
	STR_SOFTWARE_MODE,
	STR_VOLUMETRIC_FX,
	STR_BILINEAR_FILTERING,
	STR_BUMP_MAPPING,
	STR_LOW_RESOLUTION_TEXTURES,
	STR_LOW_RESOLUTION_BUMP_MAPS,
	STR_SOUND_DEVICE,
	STR_DISABLE,
	STR_BIT,
	STR_FULL_SCREEN,
	STR_WINDOWED,
	STR_RENDER_OPTIONS,
	STR_NO_SOUND_CARD_INSTALLED,
	STR_FAILED_TO_SETUP_DIRECTX,
	STR_EXIT,
	STR_CONTROL_CONFIGURATION,
	STR_USE_FLARE,
	STR_CONTROL_METHOD,
	STR_MOUSE,
	STR_KEYBOARD,
	STR_JOYSTICK,
	STR_WAITING,
	STR_RESET,
	STR_SAVE_SETTINGS,
	STR_SAVED_OK_BIS,
	STR_STEP_LEFT,
	STR_STEP_RIGHT,
	STR_PLAY,
	STR_SETUP,
	STR_INSTALL,
	STR_UNINSTALL,
	STR_README,
	STR_DEMO_MODE_ESC_TO_EXIT,
	STR_SELECT_LEVEL,
	STR_NO_FMV,
	STR_LAST_STRING_ENTRY,

	NUM_STRING_ENTRIES
};

