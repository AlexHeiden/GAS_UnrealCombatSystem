#include "Character/BaseCharacter.h"

#include "EnhancedInputComponent.h"

ABaseCharacter::ABaseCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	BaseAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("BaseAttributeSet"));
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
