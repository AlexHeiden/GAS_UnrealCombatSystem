#include "Abilities/Character/GameplayAbility_Dodge.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AttributeSets/StaminaAttributeSet.h"
#include "GameFramework/Character.h"

UGameplayAbility_Dodge::UGameplayAbility_Dodge()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_Dodge::OnMontageCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGameplayAbility_Dodge::OnMontageCancelled()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

void UGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!DodgeMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Dodge: DodgeMontage not set"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Dodge: Failed to retrieve Character from ActorInfo"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Dodge: Failed to commit ability"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FVector Input = Character->GetLastMovementInputVector();
	if (!Input.IsNearlyZero())
	{
		Character->SetActorRotation(FRotator(0.f, Input.Rotation().Yaw, 0.f));
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		DodgeMontage,
		1.f,
		NAME_None,
		false,
		1.0f);
	if (!MontageTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Dodge: MontageTask failed to be created"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	MontageTask->OnCompleted.AddDynamic(this, &UGameplayAbility_Dodge::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UGameplayAbility_Dodge::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_Dodge::OnMontageCancelled);
	MontageTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_Dodge::OnMontageCompleted);

	MontageTask->ReadyForActivation();
}

bool UGameplayAbility_Dodge::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	const float Cost = StaminaCost.GetValueAtLevel(GetAbilityLevel());

	const UStaminaAttributeSet* StaminaAttributeSet = ActorInfo->AbilitySystemComponent->GetSet<UStaminaAttributeSet>();
	if (!StaminaAttributeSet)
	{
		return false;
	}

	return StaminaAttributeSet->GetStamina() >= Cost;
}
