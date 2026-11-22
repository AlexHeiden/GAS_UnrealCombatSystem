#pragma once
#include "AbilityDataAsset.h"
#include "GameplayTagContainer.h"
#include "AttackDataAsset.generated.h"

UCLASS(BlueprintType)
class UAttackDataAsset: public UAbilityDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FGameplayTag HitSourceTag;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (ClampMin = "0.0"))
	float DamageMultiplier = 1.f;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
