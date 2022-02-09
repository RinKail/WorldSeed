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
	//ChunkSize = 16;

	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	 ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMesh (TEXT("/Game/PlaceholderAssets/PlaceholderSet_Floor")); 
	 ConstructorHelpers::FObjectFinder<UStaticMesh> RaisedMesh (TEXT("/Game/PlaceholderAssets/PlaceholderSet_Raised"));


	/*
	*  static ConstructorHelpers::FObjectFinder<UDataTable> Table(TEXT("DataTable'/Game/TUTORIAL_RESOURCES/DataTables/PlayerAttackMontage.PlayerAttackMontage'"));
	 if (Table.Succeeded())
	 {
		 DataTable_Geometry = Table.Object;
	 }
	*/


	
	


			/*
			FName OName = "Mesh_" + x + y;
			UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(OName);


		
		}
	}
	*/

}

void AWT_WorldChunk::InitialiseMeshComponents()
{
	static const FString ContextString(TEXT("Player Attack Montage"));
	FTile_TableRow* TableRow = DataTable_Geometry->FindRow<FTile_TableRow>(FName(TEXT("Punch1")), ContextString, true);
	if (TableRow)
	{
		InitialiseTileData(EWT_GeomID::ID_Raised, TableRow->Raised);

		InitialiseTileData(EWT_GeomID::ID_Wall, TableRow->Wall);
		
		InitialiseTileData(EWT_GeomID::ID_WallCorner, TableRow->WallCorner);

	

		InitialiseTileData(EWT_GeomID::ID_InnerCorner, TableRow->InnerCorner);
		InitialiseTileData(EWT_GeomID::ID_OuterCorner, TableRow->OuterCorner);



		InitialiseTileData(EWT_GeomID::ID_ThinCorner, TableRow->ThinCorner);
		InitialiseTileData(EWT_GeomID::ID_ThinWall, TableRow->ThinWall);
		InitialiseTileData(EWT_GeomID::ID_ThinWallEnd, TableRow->ThinWallEnd);

		InitialiseTileData(EWT_GeomID::ID_TConnector, TableRow->TConnector);
		InitialiseTileData(EWT_GeomID::ID_XConnector, TableRow->XConnector);
		InitialiseTileData(EWT_GeomID::ID_Block, TableRow->Block);

		



	
	}
}

void AWT_WorldChunk::GenerateChunk(AWT_GeneratorCore* Gen, FVector ChunkScale)
{
	for (int z = 0; z < ChunkScale.Z; ++z)
	{
		for (int x = 0; x < ChunkScale.Y; ++x)
		{
			for (int y = 0; y < ChunkScale.Z; ++y)
			{
				FVector CurrentPos = FVector(x, y, z) + (GetActorLocation() / TileScale);

				EWT_GeomID ID = Gen->GetTileData(CurrentPos).TileID;
				if (ID != EWT_GeomID::ID_Empty && ID != EWT_GeomID::ID_Floor && Gen->IsEmptyAdjacent(CurrentPos))
				{
					UpdateTile(CurrentPos, Gen->GetTileData(CurrentPos));
				}



			}
		}
	}
}

void AWT_WorldChunk::OnConstruction(const FTransform& Transform)
{
	//InitialiseMeshComponents();
}



void AWT_WorldChunk::InitialiseTileData(EWT_GeomID TileID, FTile_AssetTypes Asset)
{
	FInstanceStack TempStack;

	FTile_ComponentData TempData;

	TempData.BottomComponent = NewObject<UInstancedStaticMeshComponent>(this);
	TempData.BottomComponent->RegisterComponent();
	TempData.BottomComponent->SetStaticMesh(Asset.Bottom);

	TempData.MiddleComponent = NewObject<UInstancedStaticMeshComponent>(this);
	TempData.MiddleComponent->RegisterComponent();
	TempData.MiddleComponent->SetStaticMesh(Asset.Middle);
	

	TempData.TopComponent = NewObject<UInstancedStaticMeshComponent>(this);
	TempData.TopComponent->RegisterComponent();
	TempData.TopComponent->SetStaticMesh(Asset.Top);
	


	TempStack.ComponentList.Add(TempData);
	ComponentList.Add(EWT_GeomID::ID_Block, TempStack);
}


void AWT_WorldChunk::UpdateTile(FVector Position, FGridVisual Data)
{
	if (TileKeys.Find(Position))
	{
		TileKeys[Position].Comp->RemoveInstance(TileKeys[Position].Index);

		TileKeys[Position].Index = 0;
		TileKeys[Position].Comp = nullptr;
	}
	else
	{
		TileKeys.Add(Position);
	}
	

	switch (Data.StackID)
	{
	case EWT_StackID::ID_Bottom:
		TileKeys[Position].Index = ComponentList[Data.TileID].ComponentList[Data.Channel].BottomComponent->AddInstance(FTransform(FRotator(0, 0, Data.Rot), Position * TileScale, FVector(1,1,1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].ComponentList[Data.Channel].BottomComponent;
		break;
	case EWT_StackID::ID_Mid:
		TileKeys[Position].Index = ComponentList[Data.TileID].ComponentList[Data.Channel].MiddleComponent->AddInstance(FTransform(FRotator(0, 0, Data.Rot), Position * TileScale, FVector(1, 1, 1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].ComponentList[Data.Channel].MiddleComponent;
		break;
	case EWT_StackID::ID_Top:
		TileKeys[Position].Index = 	ComponentList[Data.TileID].ComponentList[Data.Channel].TopComponent->AddInstance(FTransform(FRotator(0, 0, Data.Rot), Position * TileScale, FVector(1, 1, 1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].ComponentList[Data.Channel].TopComponent;
		break;

	}

	




}


