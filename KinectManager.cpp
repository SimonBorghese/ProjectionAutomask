// (c) Simon Borghese 2023 All Rights Reserved. See License.txt for details.


#include "KinectManager.h"

#include <dmerror.h>
#define SEVERITY_ERROR 1
#define VOID void
#include <NuiApi.h>

#undef SetPortW

#include <ImageUtils.h>

// Sets default values for this component's properties
UKinectManager::UKinectManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AutoMaskMesh"));
	MeshMaterial = CreateDefaultSubobject<UMaterialInstanceDynamic>(TEXT("Mesh Mat"));

	DynamicTexture = CreateDefaultSubobject<UDynamicTexture>(TEXT("Dynamic Texture"));
	

	// ...
}


// Called when the game starts
void UKinectManager::BeginPlay()
{
	Super::BeginPlay();

	DynamicTexture->Create(800, 800);
	NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
	int NumSensors = -1;
	HRESULT herr = NuiGetSensorCount(&NumSensors);
	
	if (herr == S_OK)
	{
		UE_LOG(LogActor, Warning, TEXT("Found: %d Sensors"), NumSensors);

		if (NumSensors > 0)
		{
			herr = NuiCreateSensorByIndex(0, &Sensor);

			if (herr != S_OK)
			{
				UE_LOG(LogActor, Warning, TEXT("Failed to Create Sensor!"));
				Sensor->Release();
				Sensor = nullptr;
			} else
			{
				UE_LOG(LogActor, Warning, TEXT("Successfully created sensor!"))
				Sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON);

				DepthImageEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
				herr = Sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH,
				NUI_IMAGE_RESOLUTION_640x480,
				0,
				2,
				DepthImageEvent,
				&ImageStream);
			}
		}
	} else
	{
		UE_LOG(LogActor, Warning, TEXT("Failed to iterate sensors!"));
	}

	
	if (MaterialBase)
	{
		MeshMaterial = UMaterialInstanceDynamic::Create(MaterialBase, NULL);

		if (MeshMaterial)
		{
			Mesh->SetMaterial(0, MeshMaterial);
		}
	}
	
	//MeshMaterial = CreateDefaultSubobject<UMaterialInstanceDynamic>(TEXT("MeshMat"));
}


// Called every frame
void UKinectManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (Sensor && WAIT_OBJECT_0 == WaitForSingleObject(DepthImageEvent, 0))
	{
		HRESULT hr;
		NUI_IMAGE_FRAME imageFrame;

		// Attempt to get the depth frame
		hr = Sensor->NuiImageStreamGetNextFrame(ImageStream, 0, &imageFrame);
		if (FAILED(hr))
		{
			return;
		}

		BOOL nearMode;
		INuiFrameTexture* pTexture;

		// Get the depth image pixel texture
		hr = Sensor->NuiImageFrameGetDepthImagePixelFrameTexture(
			ImageStream, &imageFrame, &nearMode, &pTexture);
		if (FAILED(hr))
		{
			goto ReleaseFrame;
		}

		NUI_LOCKED_RECT LockedRect;

		// Lock the frame data so the Kinect knows not to modify it while we're reading it
		pTexture->LockRect(0, &LockedRect, NULL, 0);

		// Make sure we've received valid data
		if (LockedRect.Pitch != 0)
		{
			// Get the min and max reliable depth for the current frame
			int minDepth = (nearMode ? NUI_IMAGE_DEPTH_MINIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MINIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;
			int maxDepth = (nearMode ? NUI_IMAGE_DEPTH_MAXIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MAXIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;

			const NUI_DEPTH_IMAGE_PIXEL * pBufferRun = reinterpret_cast<const NUI_DEPTH_IMAGE_PIXEL *>(LockedRect.pBits);

			// end pixel is start + width*height - 1
			const NUI_DEPTH_IMAGE_PIXEL * pBufferEnd = pBufferRun + (640 * 480);

			TArray<FColor> ColorData;

			while ( pBufferRun < pBufferEnd )
			{
				// discard the portion of the depth that contains only the player index
				USHORT depth = pBufferRun->depth;

				// To convert to a byte, we're discarding the most-significant
				// rather than least-significant bits.
				// We're preserving detail, although the intensity will "wrap."
				// Values outside the reliable depth range are mapped to 0 (black).

				// Note: Using conditionals in this loop could degrade performance.
				// Consider using a lookup table instead when writing production code.
				BYTE intensity = static_cast<BYTE>(depth >= minDepth && depth <= maxDepth ? depth % 256 : 0);

				if (intensity < MaxDist && intensity > MinDist)
				{
					ColorData.Add(FColor(intensity, intensity, intensity));
				} else
				{
					ColorData.Add(FColor(0, 0,0, 0));
				}

				// We're outputting BGR, the last byte in the 32 bits is unused so skip it
				// If we were outputting BGRA, we would write alpha here.

				// Increment our index into the Kinect's depth buffer
				++pBufferRun;
			}
			Texture = FImageUtils::CreateTexture2D(640, 480, ColorData, this, TEXT("Test"), EObjectFlags::RF_Public, FCreateTexture2DParameters());

			MeshMaterial->SetTextureParameterValue(TEXT("Tex"), Texture);
			Mesh->SetMaterial(0, MeshMaterial);
			

		}

		ReleaseFrame:
		Sensor->NuiImageStreamReleaseFrame(ImageStream, &imageFrame);

	}

	CurrentTime += DeltaTime;
	DynamicTexture->Fill(FLinearColor(0, 0, 0, 0));
	DynamicTexture->DrawRect(abs(599 * sin(CurrentTime)), abs(399 * cos(CurrentTime)), 200, 200, FLinearColor::Red);
	DynamicTexture->UpdateTexture();
	MeshMaterial->SetTextureParameterValue(TEXT("Tex"), DynamicTexture->GetTexture());
	Mesh->SetMaterial(0, MeshMaterial);
}

