// (c) Simon Borghese 2023 All Rights Reserved. See License.txt for details.

#pragma once
#undef SetPort
#undef SetPortW
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VT/VirtualTexture.h"
#include "DynamicTexture.h"
#include "KinectManager.generated.h"
#undef SetPort
#undef SetPortW



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVPROJECTIONRENDER_API UKinectManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKinectManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class INuiSensor *Sensor;

	Windows::HANDLE ImageStream;
	Windows::HANDLE DepthImageEvent;

	UPROPERTY(VisibleAnywhere)
	UTexture2D *Texture;

	UPROPERTY(VisibleAnywhere)
	UDynamicTexture *DynamicTexture;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic *MeshMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface *MaterialBase;
	

	UPROPERTY(EditAnywhere)
	int MinDist = 0;

	UPROPERTY(EditAnywhere)
	int MaxDist = 255;

	float CurrentTime = 0.0;

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
