// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PropertyValue.h"

#include "PropertyValueColor.generated.h"

// Keeps an FLinearColor interface by using the property setter/getter functions,
// even though the property itself is of FColor type
UCLASS(BlueprintType)
class VARIANTMANAGERCONTENT_API UPropertyValueColor : public UPropertyValue
{
	GENERATED_UCLASS_BODY()

public:

	// UPropertyValue interface
	TArray<uint8> GetDataFromResolvedObject() const override;
	virtual UScriptStruct* GetStructPropertyStruct() const override;
	virtual int32 GetValueSizeInBytes() const override;
	//~ UPropertyValue interface
};

#if UE_ENABLE_INCLUDE_ORDER_DEPRECATED_IN_5_2
#include "CoreMinimal.h"
#endif
