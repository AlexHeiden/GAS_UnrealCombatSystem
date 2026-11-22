#include "Data/CombatTags.h"

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Invulnerability, "State.Invulnerability",
	"Target ignores damage while this tag is present");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_CancelWindow_Open, "State.CancelWindow.Open");

UE_DEFINE_GAMEPLAY_TAG(TAG_Event_HitWindow_Open, "Event.HitWindow.Open");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_HitWindow_Close, "Event.HitWindow.Close");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_CancelWindow_Open, "Event.HitWindow.Open");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_CancelWindow_Close, "Event.HitWindow.Close");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_IFrames_Open, "Event.HitWindow.Open");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_IFrames_Close, "Event.HitWindow.Close");

UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Damage_Multiplier, "Data.Damage.Multiplier");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Cost_Stamina, "Data.Cost.Stamina");
