#include "AttributeSets/AttackAttributeSet.h"
#include "Net/UnrealNetwork.h"

UAttackAttributeSet::UAttackAttributeSet()
{
	InitAttackPower(20.0f);
}

void UAttackAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttackAttributeSet, AttackPower, OldAttackPower);
}

void UAttackAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAttackAttributeSet, AttackPower, COND_OwnerOnly, REPNOTIFY_Always);
}
