#include "Abilities/Character/Melee/GameplayAbility_MeleeCombo.h"

#include "Data/CombatTags.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"


void UGameplayAbility_MeleeCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsActive())
	{
		return;
	}

	UAbilityTask_WaitGameplayEvent* WaitCancelWindowOpenTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TAG_Event_CancelWindow_Open,
		nullptr,
		false,
		false
	);
	if (!WaitCancelWindowOpenTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: WaitCancelWindowOpenTask failed to be created"), *GetClass()->GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WaitCancelWindowOpenTask->EventReceived.AddDynamic(this, &UGameplayAbility_MeleeCombo::OnCancelWindowOpen);
	WaitCancelWindowOpenTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitCancelWindowCloseTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TAG_Event_CancelWindow_Close,
		nullptr,
		false,
		false
	);
	if (!WaitCancelWindowCloseTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: WaitCancelWindowCloseTask failed to be created"), *GetClass()->GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WaitCancelWindowCloseTask->EventReceived.AddDynamic(this, &UGameplayAbility_MeleeCombo::OnCancelWindowClose);
	WaitCancelWindowCloseTask->ReadyForActivation();
}

void UGameplayAbility_MeleeCombo::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (bIsCancelWindowOpen)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
		if (!SourceASC)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve ASC from source"), *GetClass()->GetName());
		}
		else
		{
			SourceASC->RemoveLooseGameplayTag(TAG_State_CancelWindow_Open);
			bIsCancelWindowOpen = false;
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_MeleeCombo::OnCancelWindowOpen(FGameplayEventData Payload)
{
	if (bIsCancelWindowOpen)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve ASC from source"), *GetClass()->GetName());
		return;
	}

	SourceASC->AddLooseGameplayTag(TAG_State_CancelWindow_Open);
	bIsCancelWindowOpen = true;
}

void UGameplayAbility_MeleeCombo::OnCancelWindowClose(FGameplayEventData Payload)
{
	if (!bIsCancelWindowOpen)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve ASC from source"), *GetClass()->GetName());
		return;
	}

	SourceASC->RemoveLooseGameplayTag(TAG_State_CancelWindow_Open);
	bIsCancelWindowOpen = false;
}
