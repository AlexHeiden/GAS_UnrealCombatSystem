#include "Data/AbilityDataAsset.h"
#include "Misc/DataValidation.h"

#if WITH_EDITOR
EDataValidationResult UAbilityDataAsset::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!Montage)
	{
		Context.AddError(FText::FromString(TEXT("Montage is not set")));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}
#endif
