#include "Abilities/Character/GameplayAbility_AttackLight.h"

#include "GameplayEffects/UExecCalc_Damage.h"
#include "Kismet/GameplayStatics.h"
#include "NPC/TargetDummy.h"

UGameplayAbility_AttackLight::UGameplayAbility_AttackLight()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Light")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Light")));
}

void UGameplayAbility_AttackLight::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: Failed to commit ability"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!DamageEffectClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: DamageEffectClass not set"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	ATargetDummy* TargetDummy = Cast<ATargetDummy>(
		UGameplayStatics::GetActorOfClass(
			GetWorld(),
			ATargetDummy::StaticClass()));
	if (!TargetDummy)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: Failed to find target"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	UAbilitySystemComponent* TargetASC = TargetDummy->GetAbilitySystemComponent();
	if (!TargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: Target doesn't have ASC"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* SourceASC = ActorInfo->AbilitySystemComponent.Get();
	if (!SourceASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: Source doesn't have ASC"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
		DamageEffectClass, GetAbilityLevel(), ContextHandle);
	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: Failed to create GameplayEffectSpec"));
	}

	SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UGameplayAbility_AttackLight::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	};

	return true;
}
