// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSeed/Public/WT_Landmark_Base.h"
#include "WorldSeed/Public/WT_Resources.h"
#include "WorldSeed/Public/WT_GeneratorCore.h"




// Sets default values
AWT_Landmark_Base::AWT_Landmark_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	LandmarkScale = FVector(5, 5, 1);



	

}

// Called when the game starts or when spawned
void AWT_Landmark_Base::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(50, 50, 0) * TileScale);
	//RootComponent->TransformUpdated.AddUObject(this, &AWT_Landmark_Base::OnMovementDetected);
	
}


// Called every frame
void AWT_Landmark_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AWT_Landmark_Base::OnMovementDetected()
{
	

}

void AWT_Landmark_Base::ApplyLandmark(class AWT_GeneratorCore* Generator)
{
	FVector ConvertedPosition = FVector(0, 0, 0);

	ConvertedPosition.X = (int)(GetActorLocation().X / TileScale);
	ConvertedPosition.Y = (int)(GetActorLocation().Y / TileScale);
	ConvertedPosition.Z = (int)(GetActorLocation().Z / TileScale);


	for (int z = 0; z < LandmarkScale.Z; z++)
	{
		for (int x = 0; x < LandmarkScale.X; x++)
		{
			for (int y = 0; y < LandmarkScale.Y; y++)
			{
				Generator->SetCellState(ConvertedPosition + FVector(x, y, z), bAdditive);
				if (bIsWalkable)
				{
					if (bAdditive)
					{
						if (z >= LandmarkScale.Z - 1)
						{
							Generator->AddFloorCell(ConvertedPosition + FVector(x, y, z + 1));
						}
					}
					else
					{
						if (z == 0)
						{
							Generator->AddFloorCell(ConvertedPosition + FVector(x, y, z));
						}
					}
				}
				
			}
		}
	}

}
