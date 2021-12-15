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



			/*
			FName OName = "Mesh_" + x + y;
			UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(OName);


		
		}
	}
	*/

}

void AWT_WorldChunk::SetTile(FVector Position, EWT_TileDirection Dir, EWT_TileID ID, int Channel)
{
	if (!MeshInstances.Find(ID))
	{

		MeshInstances.Add(ID, new FInstanceStack());

	}
	if (MeshInstances[ID]->ComponentList[Channel] == nullptr)
	{
		UInstancedStaticMeshComponent* TempComponent = NewObject<UInstancedStaticMeshComponent>(this);
		TempComponent->RegisterComponent();
		TempComponent->SetupAttachment(RootComponent);

		switch (ID)
		{
		case EWT_TileID::TI_Raised:
			TempComponent->SetStaticMesh(Raised);
			break;
		case EWT_TileID::TI_Floor:
			TempComponent->SetStaticMesh(Floor);
			break;

		}

		MeshInstances[ID]->ComponentList.Add(TempComponent);

	}
	


	MeshInstances[ID]->ComponentList[Channel]->AddInstance(FTransform(Position));

}


void AWT_WorldChunk::Generate(class AWT_GeneratorCore* Generator, FVector2D GridPosition)
{
	
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			if (Generator->GetGridData(GridPosition + FVector2D(x, y)))
			{
				SetTile(FVector(x,y,0), EWT_TileDirection::TD_All, EWT_TileID::TI_Floor, 0);
			}
			else
			{
				SetTile(FVector(x, y, 0), EWT_TileDirection::TD_All, EWT_TileID::TI_Raised, 0);
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

