// (c) Simon Borghese 2023 All Rights Reserved. See License.txt for details.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DynamicTexture.h"
#include "DepthCamera.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVPROJECTIONRENDER_API UDepthCamera : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDepthCamera();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UDynamicTexture *Texture;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
