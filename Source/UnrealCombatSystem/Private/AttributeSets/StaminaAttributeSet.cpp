#include "AttributeSets/StaminaAttributeSet.h"
#include "Net/UnrealNetwork.h"

UStaminaAttributeSet::UStaminaAttributeSet()
{
	InitStamina(100.0f);
	InitMaxStamina(100.0f);
}

void UStaminaAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaminaAttributeSet, Stamina, OldStamina);
}

void UStaminaAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaminaAttributeSet, MaxStamina, OldMaxStamina);
}

void UStaminaAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UStaminaAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UStaminaAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}
