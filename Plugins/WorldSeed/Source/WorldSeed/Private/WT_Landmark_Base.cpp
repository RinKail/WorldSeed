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

	LandmarkScale = FVector2D(5, 5);


}

// Called when the game starts or when spawned
void AWT_Landmark_Base::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(50, 50, 0) * TileScale);
	
}

// Called every frame
void AWT_Landmark_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWT_Landmark_Base::ApplyLandmark(class AWT_GeneratorCore* Generator)
{
	FVector2D ConvertedPosition = FVector2D(GetActorLocation().X, GetActorLocation().Y);

	ConvertedPosition.X = (int)(ConvertedPosition.X / TileScale);
	ConvertedPosition.Y = (int)(ConvertedPosition.Y / TileScale);

	for (int x = 0; x < LandmarkScale.X; x++)
	{
		for (int y = 0; y < LandmarkScale.Y; y++)
		{
			Generator->ApplyToGrid(ConvertedPosition + FVector2D(x, y), true);
		}
	}
}