#pragma once
#include "AnimNotifyState_EventWindow.h"
#include "Data/CombatTags.h"
#include "AnimNotifyState_IFrames.generated.h"

UCLASS(meta = (DisplayName = "IFrames"))
class UAnimNotifyState_IFrames: public UAnimNotifyState_EventWindow
{
	GENERATED_BODY()	
public:
	UAnimNotifyState_IFrames()
	{
		OpenEventTag = TAG_Event_IFrames_Open;
		CloseEventTag = TAG_Event_IFrames_Close;
	}

	virtual FString GetNotifyName_Implementation() const override { return TEXT("IFrames"); }
};
