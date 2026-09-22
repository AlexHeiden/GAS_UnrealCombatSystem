#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "UObject/CoreNet.h"
#include "AttackAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


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
