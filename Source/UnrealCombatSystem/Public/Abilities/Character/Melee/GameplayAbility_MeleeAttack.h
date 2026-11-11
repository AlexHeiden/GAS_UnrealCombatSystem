#pragma once

#include "Abilities/BaseGameplayAbility.h"
#include "Character/Components/MeleeTraceComponent.h"
#include "GameplayAbility_MeleeAttack.generated.h"

UCLASS()
class UGameplayAbility_MeleeAttack: public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_MeleeAttack();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FGameplayTag OpenHitWindowTag;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FGameplayTag CloseHitWindowTag; 
	
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
