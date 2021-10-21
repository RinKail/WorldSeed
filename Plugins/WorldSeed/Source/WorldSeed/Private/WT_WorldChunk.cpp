// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSeed/Public/WT_WorldChunk.h"

// Sets default values
AWT_WorldChunk::AWT_WorldChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWT_WorldChunk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWT_WorldChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
* // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);



	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{

			FVector2D Pos = FVector2D((int)x, (int)y);
			
			Meshes.Add(Pos, nullptr);
		
		

			FName Name = *FString::Printf(TEXT("Mesh %i %i"), x, y);
			Meshes[Pos] = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		

			if (Meshes[Pos])
			{
				
				Meshes[Pos]->SetupAttachment(SceneRoot);
				Meshes[Pos]->SetRelativeLocation(FVector(TileScale * x, TileScale * y, 0));
				
				
			}



			/*
			FName OName = "Mesh_" + x + y;
			UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(OName);

			

		}
	}
* 
*/

