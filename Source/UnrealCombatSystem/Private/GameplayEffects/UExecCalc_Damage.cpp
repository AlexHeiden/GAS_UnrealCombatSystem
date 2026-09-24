#include "GameplayEffects/UExecCalc_Damage.h"

#include "AttributeSets/AttackAttributeSet.h"
#include "AttributeSets/HealthAttributeSet.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	AttackPowerDef = FGameplayEffectAttributeCaptureDefinition(
		UAttackAttributeSet::GetAttackPowerAttribute(),
		EGameplayEffectAttributeCaptureSource::Source,
		false);
	DefenseDef = FGameplayEffectAttributeCaptureDefinition(
		UHealthAttributeSet::GetDefenseAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	RelevantAttributesToCapture.Add(AttackPowerDef);
	RelevantAttributesToCapture.Add(DefenseDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float AttackPower = 0.f;
	bool bAttackPowerFound = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackPowerDef, EvaluationParameters, AttackPower);
	if (!bAttackPowerFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("ExecCalc_Damage: Failed to capture AttackPower from source"));
	}

	float Defense = 0.f;
	bool bDefenseFound = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefenseDef, EvaluationParameters, Defense);
	if (!bDefenseFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("ExecCalc_Damage: Failed to capture Defense from target"));
	}

	float Damage = FMath::Max(AttackPower - Defense, 0.0f);
	
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			UHealthAttributeSet::GetDamageAttribute(),
			EGameplayModOp::Additive,
			Damage));
}
