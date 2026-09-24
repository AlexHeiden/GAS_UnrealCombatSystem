#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "UExecCalc_Damage.generated.h"


UCLASS()
class UExecCalc_Damage: public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	FGameplayEffectAttributeCaptureDefinition AttackPowerDef;
	FGameplayEffectAttributeCaptureDefinition DefenseDef;
};
