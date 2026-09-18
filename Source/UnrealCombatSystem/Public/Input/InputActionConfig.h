#pragma once

#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "InputActionConfig.generated.h"

USTRUCT(BlueprintType)
struct FInputActionMapping
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};

UCLASS()
class UInputActionConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInputActionMapping> InputActions;
};
