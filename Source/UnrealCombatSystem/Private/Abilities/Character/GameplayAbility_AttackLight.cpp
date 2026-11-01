#include "Abilities/Character/GameplayAbility_AttackLight.h"

#include "GameplayEffects/UExecCalc_Damage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AttributeSets/AttackAttributeSet.h"
#include "AbilitySystemGlobals.h"
#include "Character/BaseCharacter.h"

UGameplayAbility_AttackLight::UGameplayAbility_AttackLight()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_AttackLight::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
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

	UAbilityTask_WaitGameplayEvent* WaitHitWindowOpenTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Event.HitWindow.Open")),
		nullptr,
		false,
		false
	);
	if (!WaitHitWindowOpenTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: WaitHitWindowOpenTask failed to be created"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WaitHitWindowOpenTask->EventReceived.AddDynamic(this, &UGameplayAbility_AttackLight::OnHitWindowOpen);
	WaitHitWindowOpenTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitHitWindowCloseTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Event.HitWindow.Close")),
		nullptr,
		false,
		false
		);
	if (!WaitHitWindowCloseTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: WaitHitWindowCloseTask failed to be created"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WaitHitWindowCloseTask->EventReceived.AddDynamic(this, &UGameplayAbility_AttackLight::OnHitWindowClose);
	WaitHitWindowCloseTask->ReadyForActivation();
	
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
	MontageTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_AttackLight::OnMontageCompleted);

	MontageTask->ReadyForActivation();
}

void UGameplayAbility_AttackLight::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	StopHitTrace();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_AttackLight::OnMontageCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGameplayAbility_AttackLight::OnMontageCancelled()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

void UGameplayAbility_AttackLight::OnHitWindowOpen(FGameplayEventData Payload)
{
	StartHitTrace();
}

void UGameplayAbility_AttackLight::OnHitWindowClose(FGameplayEventData Payload)
{
	StopHitTrace();
}

void UGameplayAbility_AttackLight::StartHitTrace()
{
	StopHitTrace();

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: Failed to retrieve BaseCharacter"));
		return;
	}
	
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: Failed to retrieve ASC from source"));
		return;
	}

	UMeleeTraceComponent* TraceComponent = BaseCharacter->GetRightFistTraceComponent();
	if (!TraceComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: Failed to retrieve RightFistTrace MeleeComponent from source"));
		return;
	}
	
	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	DamageSpecHandle = SourceASC->MakeOutgoingSpec(
		DamageEffectClass, GetAbilityLevel(), ContextHandle);
	if (!DamageSpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: Failed to create GameplayEffectSpec"));
		return;
	}

	ActiveTrace = TraceComponent;
	TraceComponent->OnTraceHit.AddDynamic(this, &UGameplayAbility_AttackLight::OnTraceHit);

	if (!TraceComponent->StartHitTrace())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: hit trace failed to start"));
		StopHitTrace();
	}
}

void UGameplayAbility_AttackLight::StopHitTrace()
{
	if (UMeleeTraceComponent* TraceComponent = ActiveTrace.Get())
	{
		TraceComponent->StopHitTrace();
		TraceComponent->OnTraceHit.RemoveDynamic(this, &UGameplayAbility_AttackLight::OnTraceHit);
	}
	ActiveTrace.Reset();
	DamageSpecHandle.Clear();
}

void UGameplayAbility_AttackLight::OnTraceHit(const AActor* HitActor, const FHitResult& HitResult)
{
	if (!HasAuthority(&CurrentActivationInfo))
	{
		return;
	}
	
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC  || !DamageSpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: missing source ASC or damage spec"));
		return;
	}
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitActor);
	if (!TargetASC)
	{
		return;
	}

	SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
}
