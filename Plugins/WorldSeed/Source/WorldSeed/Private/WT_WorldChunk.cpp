// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WorldSeed/Public/WT_Resources.h"
#include "WorldSeed/Public/WT_GeneratorCore.h"
#include "UObject/ConstructorHelpers.h"
// Sets default values
AWT_WorldChunk::AWT_WorldChunk()
{


	UE_LOG(LogTemp, Warning, TEXT("IS RUNNING PER FRAME"));

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ChunkSize = 5;

	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	 ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMesh (TEXT("/Game/PlaceholderAssets/PlaceholderSet_Floor")); 
	 ConstructorHelpers::FObjectFinder<UStaticMesh> RaisedMesh (TEXT("/Game/PlaceholderAssets/PlaceholderSet_Raised"));


	 Floor = FloorMesh.Object;
	 Raised = RaisedMesh.Object;

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
				Meshes[Pos]->SetStaticMesh(Floor);


			}

		}
	}

			/*
			FName OName = "Mesh_" + x + y;
			UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(OName);


		
		}
	}
	*/

}

void AWT_WorldChunk::Generate(class AWT_GeneratorCore* Generator, FVector2D GridPosition)
{
	
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			if (Generator->GetGridData(GridPosition + FVector2D(x, y)))
			{
				Meshes[FVector2D(x, y)]->SetStaticMesh(Floor);
			}
			else
			{
				Meshes[FVector2D(x, y)]->SetStaticMesh(Raised);
			}
		}
	}
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
* 
* 
*/

