#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "UAnimNotifyState_HitWindow.generated.h"


UCLASS()
class UAnimNotifyState_HitWindow: public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
};
