#pragma once

#include "Abilities/BaseGameplayAbility.h"
#include "GA_AttackLight.generated.h"

UCLASS()
class UGA_AttackLight: public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_AttackLight();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void EndAttackLight();
};
