#include "Data/AttackDataAsset.h"

#include "Misc/DataValidation.h"

#if WITH_EDITOR
EDataValidationResult UAttackDataAsset::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (!Montage)
	{
		Context.AddError(FText::FromString(TEXT("Montage is not set")));
		Result = EDataValidationResult::Invalid;
	}

	if (!HitSourceTag.IsValid())
	{
		Context.AddError(FText::FromString(TEXT("HitSourceTag is not set")));
		Result = EDataValidationResult::Invalid;
	}
	
	return Result;
}
#endif
