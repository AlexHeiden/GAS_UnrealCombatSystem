#include "Abilities/Character/GameplayAbility_Dodge.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
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

	if (!OpenIFramesTag.IsValid() || !CloseIFramesTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Dodge: OpenIFramesTag or OpenIFramesTag is not set"));
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

	UAbilityTask_WaitGameplayEvent* WaitIFramesOpenWindowTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		OpenIFramesTag,
		nullptr,
		false,
		false);
	if (!WaitIFramesOpenWindowTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Dodge: WaitIFramesOpenWindowTask failed to be created"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WaitIFramesOpenWindowTask->EventReceived.AddDynamic(this, &UGameplayAbility_Dodge::OnIFramesOpen);
	WaitIFramesOpenWindowTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitIframesCloseWindowTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		CloseIFramesTag,
		nullptr,
		false,
		false);
	if (!WaitIframesCloseWindowTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Dodge: WaitIFramesCloseWindowTask failed to be created"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WaitIframesCloseWindowTask->EventReceived.AddDynamic(this, &UGameplayAbility_Dodge::OnIFramesClose);
	WaitIframesCloseWindowTask->ReadyForActivation();
		
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

void UGameplayAbility_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (bIsInvulnerable)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
		if (!SourceASC)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Dodge: Failed to retrieve ASC from source"));
		}
		else
		{
			SourceASC->RemoveLooseGameplayTag(InvulnerabilityTag);
			bIsInvulnerable = false;
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

void UGameplayAbility_Dodge::OnIFramesOpen(FGameplayEventData Payload)
{
	if (bIsInvulnerable)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Dodge: Failed to retrieve ASC from source"));
		return;
	}

	SourceASC->AddLooseGameplayTag(InvulnerabilityTag);
	bIsInvulnerable = true;
}

void UGameplayAbility_Dodge::OnIFramesClose(FGameplayEventData Payload)
{
	if (!bIsInvulnerable)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_Dodge: Failed to retrieve ASC from source"));
		return;
	}

	SourceASC->RemoveLooseGameplayTag(InvulnerabilityTag);
	bIsInvulnerable = false;
}
