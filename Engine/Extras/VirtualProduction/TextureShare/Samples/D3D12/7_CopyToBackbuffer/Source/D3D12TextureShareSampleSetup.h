// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ITextureShareObject.h"
#include "D3D12AppSetup.h"

namespace TextureShareSample
{
	static FTextureShareObjectDesc ObjectDesc(ETextureShareDeviceType::D3D12);

	namespace Receive
	{
		namespace Texture1
		{
			// Request to read a resource #1 to a remote process
			static FTextureShareResourceDesc Desc(UE::TextureShareStrings::SceneTextures::FinalColor, ETextureShareTextureOp::Read);
		}

		namespace Texture2
		{
			// Request to read a resource #2 to a remote process
			static FTextureShareResourceDesc Desc(UE::TextureShareStrings::SceneTextures::Backbuffer, ETextureShareTextureOp::Read);
		}
	}
};
