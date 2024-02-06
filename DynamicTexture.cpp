// (c) Simon Borghese 2023 All Rights Reserved. See License.txt for details.


#include "DynamicTexture.h"

#include "ImageUtils.h"

UDynamicTexture::UDynamicTexture() : TexWidth(0), TexHeight(0)
{
	BaseTexture = nullptr;
}

void UDynamicTexture::Create(int32 Width, int32 Height)
{
	// Create the virtual texture
	BaseTexture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
	BaseTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	BaseTexture->CompressionSettings = TC_Default;
	BaseTexture->SRGB = true;
	BaseTexture->Filter = TF_Nearest;
	BaseTexture->UpdateResource();

	TextureRegion = FUpdateTextureRegion2D(0, 0, 0, 0, Width, Height);

	// Width * Height * RGBA
	TextureCache.Init(0, Width * Height * 4);

	TexWidth = Width;
	TexHeight = Height;
}

UTexture2D* UDynamicTexture::GetTexture()
{
	return BaseTexture;
}

void UDynamicTexture::UpdateTexture()
{
	if (BaseTexture)
	{
		BaseTexture->UpdateTextureRegions(
			0,
			1,
			&TextureRegion,
			TexWidth * 4,
			4,
			TextureCache.GetData());

		//BaseTexture->UpdateResource();
	}
}

void UDynamicTexture::DrawRect(int32 x0, int32 y0, int32 w, int32 h, FLinearColor Color)
{
	for (int32 y = y0; y < y0 + h; y++)
	{
		for (int32 x = x0; x < x0 + w; x++)
		{
			DrawPixel(x, y, Color);
		}
	}
}

void UDynamicTexture::DrawPixel(int32 x, int32 y, FLinearColor Color)
{
	uint8 *Pixel = &TextureCache[(y*TexWidth * 4) + (x* 4)];
	Pixel[0] = Color.R * 255;
	Pixel[1] = Color.G * 255;
	Pixel[2] = Color.B * 255;
	Pixel[3] = Color.A * 255;
}

void UDynamicTexture::Fill(FLinearColor Color)
{
	DrawRect(0, 0, TexWidth, TexHeight, Color);
}
