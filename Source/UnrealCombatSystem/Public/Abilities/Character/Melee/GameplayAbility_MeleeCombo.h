#pragma once
#include "GameplayAbility_MeleeAttack.h"
#include "GameplayAbility_MeleeCombo.generated.h"

UCLASS()
class UGameplayAbility_MeleeCombo: public UGameplayAbility_MeleeAttack
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FGameplayTag OpenCancelWindowTag;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FGameplayTag CloseCancelWindowTag;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FGameplayTag StateOpenCancelWindowTag;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnCancelWindowOpen(FGameplayEventData Payload);
	UFUNCTION()
	void OnCancelWindowClose(FGameplayEventData Payload);

	bool bIsCancelWindowOpen = false;
};
