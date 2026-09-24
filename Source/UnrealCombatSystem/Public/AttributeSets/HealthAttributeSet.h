#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "UObject/CoreNet.h"
#include "HealthAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class UHealthAttributeSet: public UAttributeSet
{
	GENERATED_BODY()
public:
	UHealthAttributeSet();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_Defense)
	FGameplayAttributeData Defense;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData Damage;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	virtual void OnRep_Defense(const FGameplayAttributeData& OldDefense);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Defense);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Damage);
};
