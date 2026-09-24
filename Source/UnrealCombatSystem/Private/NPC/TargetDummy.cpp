#include "NPC/TargetDummy.h"


ATargetDummy::ATargetDummy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"));

	if (HealthAttributeSet)
	{
		UE_LOG(LogTemp, Display, TEXT("%s Health: %f / MaxHealth: %f"),
		*GetName(), HealthAttributeSet->GetHealth(), HealthAttributeSet->GetMaxHealth());
	}
}

UAbilitySystemComponent* ATargetDummy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
