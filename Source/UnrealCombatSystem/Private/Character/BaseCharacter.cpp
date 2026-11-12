#include "Character/BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "AttributeSets/StaminaAttributeSet.h"

ABaseCharacter::ABaseCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	
	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"));
	StaminaAttributeSet = CreateDefaultSubobject<UStaminaAttributeSet>(TEXT("StaminaAttributeSet"));
	AttackAttributeSet = CreateDefaultSubobject<UAttackAttributeSet>(TEXT("AttackAttributeSet"));
	
	RightFistTraceComponent = CreateDefaultSubobject<UMeleeTraceComponent>(TEXT("RightFistTraceComponent"));
	LeftFistTraceComponent = CreateDefaultSubobject<UMeleeTraceComponent>(TEXT("LeftFistTraceComponent"));
	TArray<UMeleeTraceComponent*> TraceComponents;
	GetComponents<UMeleeTraceComponent>(TraceComponents);
	for (UMeleeTraceComponent* MeleeTraceComponent : TraceComponents)
	{
		MeleeTraceComponent->SetTraceMesh(GetMesh());
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GiveDefaultAbilities();
		ApplyDefaultEffects();
	}
}

void ABaseCharacter::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (!InputTag.IsValid() || !AbilitySystemComponent)
	{
		return;
	}

	FGameplayTag* AbilityTag = InputTagToAbilityTag.Find(InputTag);
	if (!AbilityTag || !AbilityTag->IsValid())
	{
		return;
	}
	
	FGameplayTagContainer TagsToActivate;
	TagsToActivate.AddTag(*AbilityTag);
	AbilitySystemComponent->TryActivateAbilitiesByTag(TagsToActivate);
}

void ABaseCharacter::AbilityInputTagReleased(FGameplayTag InputTag)
{
	
}

UMeleeTraceComponent* ABaseCharacter::FindMeleeTraceComponent(const FGameplayTag SourceTag) const
{
	if (!SourceTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Invalid SourceTag provided to FindMeleeTraceComponent"), *GetClass()->GetName());
		return nullptr;	
	}
	
	TArray<UMeleeTraceComponent*> MeleeTraceComponents;
	GetComponents<UMeleeTraceComponent>(MeleeTraceComponents);
	for (UMeleeTraceComponent* MeleeTraceComponent : MeleeTraceComponents)
	{
		if (MeleeTraceComponent && SourceTag.MatchesTagExact(MeleeTraceComponent->SourceTag))
		{
			return MeleeTraceComponent;
		}
	}
	
	return nullptr;
}

void ABaseCharacter::GiveDefaultAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent)
	{
		return;
	}

	for (TSubclassOf<UGameplayAbility> AbilityClass: DefaultAbilities)
	{
		if (AbilityClass)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
		}
	}
}

void ABaseCharacter::ApplyDefaultEffects()
{
	if (!HasAuthority() || !AbilitySystemComponent || bDefaultEffectsApplied)
	{
		return;
	}

	for (TSubclassOf<UGameplayEffect> EffectClass: DefaultEffects)
	{
		if (!EffectClass)
		{
			continue;
		}

		FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
		Context.AddSourceObject(this);
		FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.f, Context);
		if (Spec.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}

	bDefaultEffectsApplied = true;
}

void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (!InputActionConfig)
		{
			UE_LOG(LogTemp, Warning, TEXT("InputActionConfig is NULL in BaseCharacter"));
			return;
		}
		
		for (const FInputActionMapping& ActionMapping : InputActionConfig->InputActions)
		{
			if (!ActionMapping.InputAction)
			{
				UE_LOG(LogTemp, Warning, TEXT("InputAction is NULL in BaseCharacter"));
				continue;
			}
			
			EnhancedInputComponent->BindAction(ActionMapping.InputAction, ETriggerEvent::Started, this,
				&ABaseCharacter::AbilityInputTagPressed, ActionMapping.InputTag);
			EnhancedInputComponent->BindAction(ActionMapping.InputAction, ETriggerEvent::Completed, this,
				&ABaseCharacter::AbilityInputTagReleased, ActionMapping.InputTag);
		}	
	}
}
