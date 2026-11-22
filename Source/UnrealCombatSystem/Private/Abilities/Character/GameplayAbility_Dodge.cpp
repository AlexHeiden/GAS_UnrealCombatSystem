#include "Abilities/Character/GameplayAbility_Dodge.h"

#include "Data/CombatTags.h"
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

	if (!DodgeData || !DodgeData->Montage)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: DodgeData is invalid"), *GetClass()->GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve Character from ActorInfo"), *GetClass()->GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to commit ability"), *GetClass()->GetName());
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
		TAG_Event_IFrames_Open,
		nullptr,
		false,
		false);
	if (!WaitIFramesOpenWindowTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: WaitIFramesOpenWindowTask failed to be created"), *GetClass()->GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WaitIFramesOpenWindowTask->EventReceived.AddDynamic(this, &UGameplayAbility_Dodge::OnIFramesOpen);
	WaitIFramesOpenWindowTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitIframesCloseWindowTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TAG_Event_IFrames_Close,
		nullptr,
		false,
		false);
	if (!WaitIframesCloseWindowTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: WaitIFramesCloseWindowTask failed to be created"), *GetClass()->GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WaitIframesCloseWindowTask->EventReceived.AddDynamic(this, &UGameplayAbility_Dodge::OnIFramesClose);
	WaitIframesCloseWindowTask->ReadyForActivation();
		
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		DodgeData->Montage,
		1.f,
		NAME_None,
		false,
		1.0f);
	if (!MontageTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: MontageTask failed to be created"), *GetClass()->GetName());
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
			UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve ASC from source"), *GetClass()->GetName());
		}
		else
		{
			SourceASC->RemoveLooseGameplayTag(TAG_State_Invulnerability);
			bIsInvulnerable = false;
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGameplayAbility_Dodge::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!DodgeData || !ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}

	const UStaminaAttributeSet* Stamina = ActorInfo->AbilitySystemComponent->GetSet<UStaminaAttributeSet>();
	return Stamina && Stamina->GetStamina() >= DodgeData->StaminaCost;
}

void UGameplayAbility_Dodge::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (!CostGameplayEffectClass || !DodgeData)
	{
		return;
	}

	FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(
		Handle, ActorInfo, ActivationInfo, CostGameplayEffectClass, GetAbilityLevel(Handle, ActorInfo));
	if (!Spec.IsValid())
	{
		return;
	}
	
	Spec.Data->SetSetByCallerMagnitude(TAG_Data_Cost_Stamina, -DodgeData->StaminaCost);
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);
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
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve ASC from source"), *GetClass()->GetName());
		return;
	}

	SourceASC->AddLooseGameplayTag(TAG_State_Invulnerability);
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
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve ASC from source"), *GetClass()->GetName());
		return;
	}

	SourceASC->RemoveLooseGameplayTag(TAG_State_Invulnerability);
	bIsInvulnerable = false;
}
