// Copyright Epic Games, Inc. All Rights Reserved.

#include "D3D11TextureShareSample.h"
#include "D3D11TextureShareSampleSetup.h"
#include "Misc/TextureShareLog.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// FD3D11TextureShareSample
//////////////////////////////////////////////////////////////////////////////////////////////
FD3D11TextureShareSample::FD3D11TextureShareSample(ID3D11Texture2D* InBackBufferTexture)
{
	// Create texture share object
	if(TextureShareObject = ITextureShareObject::CreateInstance(TextureShareSample::ObjectDesc))
	{
		// Request resources for receive
		TextureShareObject->GetData().ResourceRequests.Add(FTextureShareResourceRequest(TextureShareSample::Receive::Texture1::Desc, TextureShareSample::Receive::Texture1::Resource));
		TextureShareObject->GetData().ResourceRequests.Add(FTextureShareResourceRequest(TextureShareSample::Receive::Texture2::Desc, TextureShareSample::Receive::Texture2::Resource));

		// Request custom size backbuffer resource for sending
		if (InBackBufferTexture)
		{
			TextureShareObject->GetData().ResourceRequests.Add(ITextureShareObject::GetResourceRequest(TextureShareSample::Send::Backbuffer::Desc, FTextureShareImageD3D11(InBackBufferTexture)));
		}
	}
}

FD3D11TextureShareSample::~FD3D11TextureShareSample()
{
	if (TextureShareObject)
	{
		delete TextureShareObject;
		TextureShareObject = nullptr;
	}
}

ID3D11ShaderResourceView* FD3D11TextureShareSample::GetReceiveTextureSRV(int32 InReceiveTextureIndex) const
{
	switch (InReceiveTextureIndex)
	{
	case 0: return TextureShareSample::Receive::Texture1::Resource.GetTextureSRV();
	case 1: return TextureShareSample::Receive::Texture2::Resource.GetTextureSRV();
	default:
		break;
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////
void FD3D11TextureShareSample::BeginFrame(const FTextureShareDeviceContextD3D11& InDeviceContext, ID3D11Texture2D* InBackBufferTexture)
{
	if (TextureShareObject)
	{
		// Set custom data for SDK object
		TextureShareObject->GetData().CustomData.EmplaceValue(FTextureShareCoreCustomData(TextureShareSample::CustomData::WriteParam1Key, TextureShareSample::CustomData::WriteParam1Value));
		TextureShareObject->GetData().CustomData.EmplaceValue(FTextureShareCoreCustomData(TextureShareSample::CustomData::WriteParam2Key, TextureShareSample::CustomData::WriteParam2Value));

		if (TextureShareObject->BeginFrame())
		{
			//Receive custom data from remote objects
			for (const FTextureShareCoreObjectData& RemoteObjectIt : TextureShareObject->GetReceivedData())
			{
				if (const FTextureShareCoreCustomData* CustomData = RemoteObjectIt.Data.CustomData.FindByEqualsFunc(TextureShareSample::CustomData::ReadParam1Key))
				{
					DEBUG_LOG(TEXT("Read custom data %s=%s from process %s"), *CustomData->Key, *CustomData->Value, *RemoteObjectIt.Desc.ProcessDesc.ProcessId);
				}

				if (const FTextureShareCoreCustomData* CustomData = RemoteObjectIt.Data.CustomData.FindByEqualsFunc(TextureShareSample::CustomData::ReadParam2Key))
				{
					DEBUG_LOG(TEXT("Read custom data %s=%s from process %s"), *CustomData->Key, *CustomData->Value, *RemoteObjectIt.Desc.ProcessDesc.ProcessId);
				}
			}

			// Receive remote textures
			TextureShareObject->ReceiveResource(InDeviceContext, TextureShareSample::Receive::Texture1::Desc, TextureShareSample::Receive::Texture1::Resource);
			TextureShareObject->ReceiveResource(InDeviceContext, TextureShareSample::Receive::Texture2::Desc, TextureShareSample::Receive::Texture2::Resource);
		}
	}
}

void FD3D11TextureShareSample::EndFrame(const FTextureShareDeviceContextD3D11& InDeviceContext, ID3D11Texture2D* InBackBufferTexture)
{
	if (TextureShareObject)
	{
		// Send backbuffer texture back
		TextureShareObject->SendTexture(InDeviceContext, TextureShareSample::Send::Backbuffer::Desc, FTextureShareImageD3D11(InBackBufferTexture));

		TextureShareObject->EndFrame();
	}
}
