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

	UPROPERTY(EditAnywhere, Category = "Animation")
	FGameplayTag OpenIFramesTag = FGameplayTag::RequestGameplayTag(FName("Event.IFrames.Open"));

	UPROPERTY(EditAnywhere, Category = "Animation")
	FGameplayTag CloseIFramesTag = FGameplayTag::RequestGameplayTag(FName("Event.IFrames.Close"));; 
	
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

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};
