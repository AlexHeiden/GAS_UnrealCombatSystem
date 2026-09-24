#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Macros/AttributeSetMacros.h"
#include "AttributeSet.h"
#include "UObject/CoreNet.h"
#include "AttackAttributeSet.generated.h"

UCLASS()
class UNREALCOMBATSYSTEM_API UAttackAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttackAttributeSet();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack",  ReplicatedUsing=OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	
	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(UAttackAttributeSet, AttackPower);
};
