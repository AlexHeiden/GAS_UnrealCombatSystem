#include "Animation/AnimNotifyState_EventWindow.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"

void UAnimNotifyState_EventWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;	
	}

	if (!OpenEventTag.IsValid() || !CloseEventTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OpenEventTag or CloseEventTag is not set"), *GetClass()->GetName());
		return;
	}
	
	FGameplayEventData EventData;
	EventData.EventTag = OpenEventTag;
	EventData.Instigator = Owner;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, OpenEventTag, EventData);
}

void UAnimNotifyState_EventWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;	
	}

	if (!OpenEventTag.IsValid() || !CloseEventTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OpenEventTag or CloseEventTag is not set"), *GetClass()->GetName());
		return;
	}
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, CloseEventTag, FGameplayEventData());
}