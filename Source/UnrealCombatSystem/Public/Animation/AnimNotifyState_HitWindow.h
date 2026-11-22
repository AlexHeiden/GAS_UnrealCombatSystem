#pragma once
#include "AnimNotifyState_EventWindow.h"
#include "Data/CombatTags.h"
#include "AnimNotifyState_HitWindow.generated.h"

UCLASS(meta = (DisplayName = "Hit Window"))
class UAnimNotifyState_HitWindow: public UAnimNotifyState_EventWindow
{
	GENERATED_BODY()
public:
	UAnimNotifyState_HitWindow()
	{
		OpenEventTag = TAG_Event_HitWindow_Open;
		CloseEventTag = TAG_Event_HitWindow_Close;
	}

	virtual FString GetNotifyName_Implementation() const override { return TEXT("Hit Window"); }
};
