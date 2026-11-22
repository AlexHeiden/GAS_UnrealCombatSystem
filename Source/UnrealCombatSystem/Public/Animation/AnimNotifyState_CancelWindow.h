#pragma once
#include "AnimNotifyState_EventWindow.h"
#include "Data/CombatTags.h"
#include "AnimNotifyState_CancelWindow.generated.h"


UCLASS(meta = (DisplayName = "Cancel Window"))
class UAnimNotifyState_CancelWindow: public UAnimNotifyState_EventWindow
{
	GENERATED_BODY()
public:
	UAnimNotifyState_CancelWindow()
	{
		OpenEventTag = TAG_Event_CancelWindow_Open;
		CloseEventTag = TAG_Event_CancelWindow_Close;
	}

	virtual FString GetNotifyName_Implementation() const override { return TEXT("Cancel Window"); }
};
