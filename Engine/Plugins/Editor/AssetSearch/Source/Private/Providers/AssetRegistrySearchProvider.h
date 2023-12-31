// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ISearchProvider.h"

class FAssetRegistrySearchProvider : public ISearchProvider
{
public:
	virtual ~FAssetRegistrySearchProvider() { }
	virtual void Search(FSearchQueryPtr SearchQuery) override;
};
