#pragma once

#include "AttributeSet.h"
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Macros/AttributeSetMacros.h"
#include "UObject/CoreNet.h"
#include "StaminaAttributeSet.generated.h"

UCLASS()
class UStaminaAttributeSet: public UAttributeSet
{
	GENERATED_BODY()
public:
	UStaminaAttributeSet();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina", ReplicatedUsing=OnRep_Stamina)
	FGameplayAttributeData Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina", ReplicatedUsing=OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, MaxStamina);
};
