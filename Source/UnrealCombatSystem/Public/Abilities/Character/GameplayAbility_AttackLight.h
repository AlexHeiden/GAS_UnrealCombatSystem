#pragma once

#include "Abilities/BaseGameplayAbility.h"
#include "Character/Components/MeleeTraceComponent.h"
#include "GameplayAbility_AttackLight.generated.h"

UCLASS()
class UGameplayAbility_AttackLight: public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_AttackLight();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackLightMontage;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	FGameplayTag OpenHitWindowTag = FGameplayTag::RequestGameplayTag(FName("Event.HitWindow.Open"));

	UPROPERTY(EditAnywhere, Category = "Animation")
	FGameplayTag CloseHitWindowTag = FGameplayTag::RequestGameplayTag(FName("Event.HitWindow.Close"));; 

	UPROPERTY(EditDefaultsOnly, Category = "Costs")
	FScalableFloat StaminaCost;
	
	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageCancelled();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
private:
	FGameplayEffectSpecHandle DamageSpecHandle;
	TWeakObjectPtr<UMeleeTraceComponent> ActiveTrace;

	UFUNCTION()
	void OnHitWindowOpen(FGameplayEventData Payload);
	UFUNCTION()
	void OnHitWindowClose(FGameplayEventData Payload);
	UFUNCTION()
	void OnTraceHit(const AActor* HitActor, const FHitResult& HitResult);
	
	void StartHitTrace();
	void StopHitTrace();
};
