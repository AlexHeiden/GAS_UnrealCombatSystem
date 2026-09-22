#pragma once

#include "CoreMinimal.h"
#include "UnrealCombatSystemCharacter.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AttributeSets/BaseAttributeSet.h"
#include "AttributeSets/HealthAttributeSet.h"
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
	TMap<FGameplayTag, FGameplayTag> InputTagToAbilityTag;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UBaseAttributeSet> BaseAttributeSet;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UHealthAttributeSet> HealthAttributeSet;

	void GiveDefaultAbilities();

private:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
