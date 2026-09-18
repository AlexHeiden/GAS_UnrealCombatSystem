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
	}

	SetOwner(NewController);
}

void ABaseCharacter::AbilityInputTagPressed(FGameplayTag InputTag)
{
	
}

void ABaseCharacter::AbilityInputTagReleased(FGameplayTag InputTag)
{
	
}

void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		for (const FInputActionMapping& ActionMapping : InputActionConfig->InputActions)
		{
			EnhancedInputComponent->BindAction(ActionMapping.InputAction, ETriggerEvent::Started, this,
				&ABaseCharacter::AbilityInputTagPressed, ActionMapping.InputTag);
			EnhancedInputComponent->BindAction(ActionMapping.InputAction, ETriggerEvent::Completed, this,
				&ABaseCharacter::AbilityInputTagReleased, ActionMapping.InputTag);
		}	
	}
	
}
