#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "GE_ExecCalc_Damage.generated.h"


UCLASS()
class UGE_ExecCalc_Damage: public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGE_ExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
