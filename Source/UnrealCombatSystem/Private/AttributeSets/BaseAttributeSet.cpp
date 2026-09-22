#include "AttributeSets//BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"


UBaseAttributeSet::UBaseAttributeSet()
{
	InitStamina(100.0f);
	InitMaxStamina(100.0f);
	InitAttackPower(20.0f);
}

void UBaseAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Stamina, OldStamina);
}

void UBaseAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxStamina, OldMaxStamina);
}

void UBaseAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, AttackPower, OldAttackPower);
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, AttackPower, COND_OwnerOnly, REPNOTIFY_Always);
}
