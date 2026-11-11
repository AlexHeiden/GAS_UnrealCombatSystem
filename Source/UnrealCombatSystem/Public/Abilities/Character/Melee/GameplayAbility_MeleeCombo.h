#pragma once
#include "GameplayAbility_MeleeAttack.h"
#include "GameplayAbility_MeleeCombo.generated.h"

UCLASS()
class UGameplayAbility_MeleeCombo: public UGameplayAbility_MeleeAttack
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FGameplayTag StateCancelWindowOpenTag;
};
