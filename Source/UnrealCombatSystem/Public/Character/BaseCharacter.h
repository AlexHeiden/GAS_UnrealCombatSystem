#pragma once

#include "CoreMinimal.h"
#include "UnrealCombatSystemCharacter.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "BaseCharacter.generated.h"


UCLASS()
class UNREALCOMBATSYSTEM_API ABaseCharacter : public AUnrealCombatSystemCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ABaseCharacter();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
};
