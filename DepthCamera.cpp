// (c) Simon Borghese 2023 All Rights Reserved. See License.txt for details.


#include "DepthCamera.h"

// Sets default values for this component's properties
UDepthCamera::UDepthCamera()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	Texture = CreateDefaultSubobject<UDynamicTexture>(TEXT("Texture"));
}


// Called when the game starts
void UDepthCamera::BeginPlay()
{
	Super::BeginPlay();
	Texture->Create(1200, 800);

	// ...

	/*
	cv::Mat frame;
	cv::VideoCapture cap;
	int deviceID = 0;             // 0 = open default camera
	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
	cap.open(deviceID, apiID);
	if (!cap.isOpened()) {
		UE_LOG(LogActor, Warning, TEXT("ERROR! Unable to open camera"));
	} else
	{
		cap.read(frame);
		if (frame.empty()) {
			UE_LOG(LogActor, Warning, TEXT("ERROR! blank frame grabbed"));
		} else
		{

			TArray<FColor> TexData;
			for (int y = 0; y < 800; y++)
			{
				for (int x = 0; x < 1200; x++)
				{
					TexData.Push(FColor(frame.data[(y*1200) + x]));
				}
			}
			Texture->DrawImage(TexData);
			Texture->UpdateTexture();
		}
	}
	*/
		
}


// Called every frame
void UDepthCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

