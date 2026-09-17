#pragma once

#include "CoreMinimal.h"
#include "UnrealCombatSystemCharacter.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "BaseCharacter.generated.h"


UCLASS()
class UNREALCOMBATSYSTEM_API ABaseCharacter : public AUnrealCombatSystemCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ABaseCharacter();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UBaseAttributeSet> BaseAttributeSet;
};
