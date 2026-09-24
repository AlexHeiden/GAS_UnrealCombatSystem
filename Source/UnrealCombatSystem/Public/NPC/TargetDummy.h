#pragma once

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AttributeSets/HealthAttributeSet.h"
#include "TargetDummy.generated.h"

UCLASS()
class ATargetDummy: public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ATargetDummy();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UHealthAttributeSet> HealthAttributeSet;
};
