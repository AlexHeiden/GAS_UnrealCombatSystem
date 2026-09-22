#include "GameplayEffects/GE_ExecCalc_Damage.h"

UGE_ExecCalc_Damage::UGE_ExecCalc_Damage()
{
}

void UGE_ExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
}
