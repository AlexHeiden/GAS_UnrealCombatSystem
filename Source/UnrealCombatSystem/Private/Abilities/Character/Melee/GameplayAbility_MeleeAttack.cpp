#include "Abilities/Character/Melee/GameplayAbility_MeleeAttack.h"

#include "GameplayEffects/ExecCalc_Damage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AttributeSets/AttackAttributeSet.h"
#include "AbilitySystemGlobals.h"
#include "Character/BaseCharacter.h"

UGameplayAbility_MeleeAttack::UGameplayAbility_MeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	if (!AttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: AttackLightMontage not set"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!OpenHitWindowTag.IsValid() || !CloseHitWindowTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: OpenHitWindowTag or CloseHitWindowTag is not set"));
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
		OpenHitWindowTag,
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

	WaitHitWindowOpenTask->EventReceived.AddDynamic(this, &UGameplayAbility_MeleeAttack::OnHitWindowOpen);
	WaitHitWindowOpenTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitHitWindowCloseTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		CloseHitWindowTag,
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

	WaitHitWindowCloseTask->EventReceived.AddDynamic(this, &UGameplayAbility_MeleeAttack::OnHitWindowClose);
	WaitHitWindowCloseTask->ReadyForActivation();
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackMontage,
		1.0f,
		NAME_None,
		false,
		1.0f);
	if (!MontageTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: MontageTask failed to be created"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	MontageTask->OnCompleted.AddDynamic(this, &UGameplayAbility_MeleeAttack::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UGameplayAbility_MeleeAttack::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_MeleeAttack::OnMontageCancelled);
	MontageTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_MeleeAttack::OnMontageCompleted);

	MontageTask->ReadyForActivation();
}

void UGameplayAbility_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	StopHitTrace();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_MeleeAttack::OnMontageCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGameplayAbility_MeleeAttack::OnMontageCancelled()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

void UGameplayAbility_MeleeAttack::OnHitWindowOpen(FGameplayEventData Payload)
{
	StartHitTrace();
}

void UGameplayAbility_MeleeAttack::OnHitWindowClose(FGameplayEventData Payload)
{
	StopHitTrace();
}

void UGameplayAbility_MeleeAttack::StartHitTrace()
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
	TraceComponent->OnTraceHit.AddDynamic(this, &UGameplayAbility_MeleeAttack::OnTraceHit);

	if (!TraceComponent->StartHitTrace())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayAbility_AttackLight: hit trace failed to start"));
		StopHitTrace();
	}
}

void UGameplayAbility_MeleeAttack::StopHitTrace()
{
	if (UMeleeTraceComponent* TraceComponent = ActiveTrace.Get())
	{
		TraceComponent->StopHitTrace();
		TraceComponent->OnTraceHit.RemoveDynamic(this, &UGameplayAbility_MeleeAttack::OnTraceHit);
	}
	ActiveTrace.Reset();
	DamageSpecHandle.Clear();
}

void UGameplayAbility_MeleeAttack::OnTraceHit(const AActor* HitActor, const FHitResult& HitResult)
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
