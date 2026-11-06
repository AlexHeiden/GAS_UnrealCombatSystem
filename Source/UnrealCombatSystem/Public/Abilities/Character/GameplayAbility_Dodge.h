#pragma once
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Dodge.generated.h"

UCLASS()
class UGameplayAbility_Dodge: public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGameplayAbility_Dodge();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FGameplayTag OpenIFramesTag = FGameplayTag::RequestGameplayTag(FName("Event.IFrames.Open"));

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FGameplayTag CloseIFramesTag = FGameplayTag::RequestGameplayTag(FName("Event.IFrames.Close")); 

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FGameplayTag InvulnerabilityTag = FGameplayTag::RequestGameplayTag(FName("State.Invulnerable"));
	
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
	UFUNCTION()
	void OnIFramesOpen(FGameplayEventData Payload);
	UFUNCTION()
	void OnIFramesClose(FGameplayEventData Payload);

	bool bIsInvulnerable = false;
};
