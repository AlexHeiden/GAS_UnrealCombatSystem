#pragma once

#include "CoreMinimal.h"
#include "UnrealCombatSystemCharacter.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AttributeSets/AttackAttributeSet.h"
#include "AttributeSets/HealthAttributeSet.h"
#include "AttributeSets/StaminaAttributeSet.h"
#include "Components/MeleeTraceComponent.h"
#include "Input/InputActionConfig.h"
#include "BaseCharacter.generated.h"


UCLASS()
class UNREALCOMBATSYSTEM_API ABaseCharacter : public AUnrealCombatSystemCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ABaseCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputActionConfig> InputActionConfig;
	
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TMap<FGameplayTag, FGameplayTag> InputTagToAbilityTag;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UMeleeTraceComponent> RightFistTraceComponent;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UMeleeTraceComponent> LeftFistTraceComponent;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	UMeleeTraceComponent* FindMeleeTraceComponent(FGameplayTag SourceTag) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UHealthAttributeSet> HealthAttributeSet;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UStaminaAttributeSet> StaminaAttributeSet;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAttackAttributeSet> AttackAttributeSet;

	void GiveDefaultAbilities();
	void ApplyDefaultEffects();

private:
	bool bDefaultEffectsApplied = false;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
