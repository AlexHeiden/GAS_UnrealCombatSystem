#include "Abilities/Character/GameplayAbility_AttackLight.h"

#include "GameplayEffects/UExecCalc_Damage.h"
#include "Kismet/GameplayStatics.h"
#include "NPC/TargetDummy.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGameplayAbility_AttackLight::UGameplayAbility_AttackLight()
{

}

void UGameplayAbility_AttackLight::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* SourceASC = ActorInfo->AbilitySystemComponent.Get();
	if (!SourceASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: Source doesn't have ASC"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!DamageEffectClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: DamageEffectClass not set"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!AttackLightMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: AttackLightMontage not set"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: Failed to commit ability"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackLightMontage,
		1.0f,
		NAME_None,
		false,
		1.0f
	);

	if (!MontageTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: MontageTask failed to be created"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	MontageTask->OnCompleted.AddDynamic(this, &UGameplayAbility_AttackLight::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UGameplayAbility_AttackLight::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_AttackLight::OnMontageCancelled);
	MontageTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_AttackLight::OnMontageCancelled);

	MontageTask->ReadyForActivation();

	/*
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
	*/
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

void UGameplayAbility_AttackLight::OnMontageCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGameplayAbility_AttackLight::OnMontageCancelled()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}
