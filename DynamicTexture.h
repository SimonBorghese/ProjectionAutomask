// (c) Simon Borghese 2023 All Rights Reserved. See License.txt for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RHITypes.h"
#include "DynamicTexture.generated.h"

/**
 * 
 */
UCLASS()
class ADVPROJECTIONRENDER_API UDynamicTexture : public UObject
{
	GENERATED_BODY()
public:
	UDynamicTexture();

protected:
	UPROPERTY(VisibleAnywhere)
	UTexture2D *BaseTexture;

	FUpdateTextureRegion2D TextureRegion;

	UPROPERTY()
	TArray<uint8> TextureCache;

	int32 TexWidth, TexHeight;
public:
	UFUNCTION(BlueprintCallable)
	void Create(int32 Width, int32 Height);
	
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetTexture();

	UFUNCTION(BlueprintCallable)
	void UpdateTexture();

	UFUNCTION(BlueprintCallable)
	void DrawRect(int32 x0, int32 y0, int32 w, int32 h, FLinearColor Color);

	UFUNCTION(BlueprintCallable)
	void DrawImage(TArray<FColor> Pixels);

	UFUNCTION(BlueprintCallable)
	void DrawImageSmooth(TArray<FColor> Pixels);

	UFUNCTION(BlueprintCallable)
	void DrawPixel(int32 x, int32 y, FLinearColor Color);

	UFUNCTION(BlueprintCallable)
	FColor GetPixel(int32 x, int32 y);

	UFUNCTION(BlueprintCallable)
	void Fill(FLinearColor Color);
};
