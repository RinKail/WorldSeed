// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WorldSeed/Public/WT_Resources.h"
#include "WorldSeed/Public/WT_GeneratorCore.h"
#include "UObject/ConstructorHelpers.h"
// Sets default values
AWT_WorldChunk::AWT_WorldChunk()
{


	

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ChunkSize = 16;

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

void AWT_WorldChunk::OnConstruction(const FTransform& Transform)
{
	GenerateMeshes();
}

void AWT_WorldChunk::BeginPlay()
{
}

void AWT_WorldChunk::ClearTile(int ID, int Channel, EWT_TileID MeshID)
{
	MeshInstances[MeshID]->ComponentList[Channel]->RemoveInstance(ID);
	
}

void AWT_WorldChunk::Generate(AWT_GeneratorCore* Generator, FVector2D GridPosition)
{

	ChunkPosition = GridPosition;
	FVector2D WorldPosition = ChunkPosition *= ChunkSize;
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
		
			FVector2D CurCoordinate = FVector2D(x, y);
			bool bIsFloor = Generator->GetGridData(CurCoordinate);
			FWT_TileData Data;

			//Collect the current data or add new data
			if (ConstructionData.Find(CurCoordinate))
			{
				Data = ConstructionData[CurCoordinate];
				ClearTile(Data.InstanceID,Data.Channel, Data.MeshID);
				Data.Channel = 0;
				Data.InstanceID = 0;
			}
			else
				Data = ConstructionData.Add(CurCoordinate, Data);

			//Assign Tile types

			if (bIsFloor)
			{
				Data.MeshID = EWT_TileID::TI_Floor;
			}
			else
			{
				if (IsFloorAdjacent(CurCoordinate, Generator))
					Data.MeshID = EWT_TileID::TI_Edge;
				else
					Data.MeshID = EWT_TileID::TI_Raised;
			}



			ConstructionData[CurCoordinate] = Data;
		}
	}

	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{

			SetCorner(FVector2D(x,y));
		}
	}

	
}

bool AWT_WorldChunk::IsFloorAdjacent(FVector2D Pos, AWT_GeneratorCore* Generator)
{
	TArray<FVector2D> Positions;
	Positions.Add(FVector2D(1, 0));
	Positions.Add(FVector2D(-1, 0));
	Positions.Add(FVector2D(0, 1));
	Positions.Add(FVector2D(0, -1));

	Positions.Add(FVector2D(1, 1));
	Positions.Add(FVector2D(-1, -1));
	Positions.Add(FVector2D(-1, 1));
	Positions.Add(FVector2D(1, -1));

	for (int i = 0; i < Positions.Num(); i++)
	{
		if (Generator->GetGridData(Pos + Positions[i]) == true)
		{
			return true;
		}
	}

	return false;
}

void AWT_WorldChunk::SetCorner(FVector2D Pos)
{
	if (IsTile(Pos, EWT_TileID::TI_Edge))
	{



	

		if (IsEdge(Pos + FVector2D(1, 0)) && IsEdge(Pos + FVector2D(0, -1)))
		{

			if (IsTile(Pos + FVector2D(1, -1), EWT_TileID::TI_Floor))
			{
				
				ConstructionData[Pos].MeshID = EWT_TileID::TI_OuterCorner;
			}
			else if (IsTile(Pos + FVector2D(-1, 1), EWT_TileID::TI_Floor))
			{
				ConstructionData[Pos].MeshID = EWT_TileID::TI_InnerCorner;
			}
		}
		if (IsEdge(Pos + FVector2D(1, 0)) && IsEdge(Pos + FVector2D(0, 1)))
		{

			if (IsTile(Pos + FVector2D(1, 1), EWT_TileID::TI_Floor))
			{
				ConstructionData[Pos].MeshID = EWT_TileID::TI_OuterCorner;
			}
			else if (IsTile(Pos + FVector2D(-1, -1), EWT_TileID::TI_Floor))
			{
				ConstructionData[Pos].MeshID = EWT_TileID::TI_InnerCorner;
			}
		}
		if (IsEdge(Pos + FVector2D(-1, 0)) && IsEdge(Pos + FVector2D(0, -1)))
		{

			if (IsTile(Pos + FVector2D(-1, -1), EWT_TileID::TI_Floor))
			{
				ConstructionData[Pos].MeshID = EWT_TileID::TI_OuterCorner;
			}
			else if (IsTile(Pos + FVector2D(1, 1), EWT_TileID::TI_Floor))
			{
				ConstructionData[Pos].MeshID = EWT_TileID::TI_InnerCorner;
			}
		}
		if (IsEdge(Pos + FVector2D(-1, 0)) && IsEdge(Pos + FVector2D(0, 1)))
		{

			if (IsTile(Pos + FVector2D(-1, 1), EWT_TileID::TI_Floor))
			{
				ConstructionData[Pos].MeshID = EWT_TileID::TI_OuterCorner;
			}
			else if (IsTile(Pos + FVector2D(1, -1), EWT_TileID::TI_Floor))
			{
				ConstructionData[Pos].MeshID = EWT_TileID::TI_InnerCorner;
			}
		}




	}


}

void AWT_WorldChunk::GenerateMeshes()
{
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			switch (ConstructionData[FVector2D(x, y)].MeshID)
			{
			case EWT_TileID::TI_Raised:
				SetTile(FVector2D (x,y), EWT_TileID::TI_Raised, 0, 0);
				break;
			case EWT_TileID::TI_Edge:
				SetTile(FVector2D(x, y), EWT_TileID::TI_Edge, 0, 0);
				break;
			case EWT_TileID::TI_Floor:
				SetTile(FVector2D(x, y), EWT_TileID::TI_Floor, 0, 0);
				break;
			case EWT_TileID::TI_InnerCorner:
				SetTile(FVector2D(x, y), EWT_TileID::TI_InnerCorner, 0, 0);
				break;
			case EWT_TileID::TI_OuterCorner:
				SetTile(FVector2D(x, y), EWT_TileID::TI_OuterCorner, 0, 0);
				break;
			
			}

		}
	}
}




 int AWT_WorldChunk::SetTile(FVector Position, EWT_TileDirection Dir, EWT_TileID ID, int Channel)
{
	if (!MeshInstances.Find(ID))
	{

		MeshInstances.Add(ID, new FInstanceStack());

	}
	if (!MeshInstances[ID]->ComponentList.IsValidIndex(Channel))
	{
		UInstancedStaticMeshComponent* TempComponent = NewObject<UInstancedStaticMeshComponent>(this);
		TempComponent->SetupAttachment(RootComponent);
		TempComponent->RegisterComponent();
	

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


	UE_LOG(LogTemp, Warning, TEXT("Generating Tile at: %d, %d"), (int)Position.X, (int)Position.Y);
	return MeshInstances[ID]->ComponentList[Channel]->AddInstance(FTransform(FRotator(0,0,0), Position, FVector(1,1,1)));

}

 void AWT_WorldChunk::SetTile(FVector2D Position, EWT_TileID ID, int Channel, int AdjacentData)
 {
	 FVector WorldPos = FVector(Position.X, Position.Y, 0);
	 WorldPos *= ChunkSize;
	 if (!MeshInstances.Find(ID))
	 {

		 MeshInstances.Add(ID, new FInstanceStack());

	 }
	 if (!MeshInstances[ID]->ComponentList.IsValidIndex(Channel))
	 {
		 UInstancedStaticMeshComponent* TempComponent = NewObject<UInstancedStaticMeshComponent>(this);
		 TempComponent->SetupAttachment(RootComponent);
		 TempComponent->RegisterComponent();


		 switch (ID)
		 {
		 case EWT_TileID::TI_Raised:
			 TempComponent->SetStaticMesh(Raised);
			 break;
		 case EWT_TileID::TI_Floor:
			 TempComponent->SetStaticMesh(Floor);
			 break;

		 }
	 }
	 
	int Index = MeshInstances[ID]->ComponentList[Channel]->AddInstance(FTransform(FRotator(0, 0, 0), WorldPos, FVector(1, 1, 1)));
	ConstructionData[Position].InstanceID = Index;

	  
 }

/*
void AWT_WorldChunk::Generate(class AWT_GeneratorCore* Generator, FVector2D GridPosition)
{
	UE_LOG(LogTemp, Warning, TEXT("Generating Chunk: %d, %d"), (int)GridPosition.X, (int)GridPosition.Y);

	ChunkPosition = GridPosition;
	GridPosition *= ChunkSize;
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			int Instance = 0;
			if (Generator->GetGridData(GridPosition + FVector2D(x, y)))
			{
				
				Instance = SetTile(FVector(x * TileScale, y * TileScale, 0), EWT_TileDirection::TD_All, EWT_TileID::TI_Floor, 0);
				ConstructionData.Add(FVector2D(x, y), FWT_TileData(Instance, 0, true, EWT_TileID::TI_Floor));
			}
			else
			{
				
				Instance = SetTile(FVector(x * TileScale, y * TileScale, 0), EWT_TileDirection::TD_All, EWT_TileID::TI_Raised, 0);
				ConstructionData.Add(FVector2D(x, y), FWT_TileData(Instance, 0, false, EWT_TileID::TI_Raised));
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


