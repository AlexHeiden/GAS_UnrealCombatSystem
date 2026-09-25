#include "Animation/UAnimNotifyState_HitWindow.h"

#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAnimNotifyState_HitWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;	
	}

	FGameplayTag OpenTag = FGameplayTag::RequestGameplayTag(FName("Event.HitWindow.Open"));
	FGameplayEventData EventData;
	EventData.EventTag = OpenTag;
	EventData.Instigator = Owner;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, OpenTag, EventData);
}

void UAnimNotifyState_HitWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;	
	}

	FGameplayTag CloseTag = FGameplayTag::RequestGameplayTag(FName("Event.HitWindow.Close"));
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, CloseTag, FGameplayEventData());
}
