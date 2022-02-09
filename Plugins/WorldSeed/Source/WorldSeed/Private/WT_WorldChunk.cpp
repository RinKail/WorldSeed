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


	
	  static ConstructorHelpers::FObjectFinder<UDataTable> Table(TEXT("DataTable'/WorldSeed/DT_WorldSeed_TileData.DT_WorldSeed_TileData'"));

	 if (Table.Succeeded())
	 {
		 DataTable_Geometry = Table.Object;
	 }
	


	
	


			
			/*
			* FName OName = "Mesh_" + x + y;
			UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(OName);

			*/

		
	
	 InitialiseMeshComponents();

}

void AWT_WorldChunk::InitialiseMeshComponents()
{
	static const FString ContextString(TEXT(""));
	FTile_TableRow* TableRow = DataTable_Geometry->FindRow<FTile_TableRow>(FName(TEXT("Placeholder")), ContextString, true);
	if (TableRow)
	{
		InitialiseTileData(EWT_GeomID::ID_Raised, TableRow->Raised,"RaisedComponents");

		InitialiseTileData(EWT_GeomID::ID_Wall, TableRow->Wall, "WallComponents");
		
	
		InitialiseTileData(EWT_GeomID::ID_WallCorner, TableRow->WallCorner, "WallCornerComponents");

	

		InitialiseTileData(EWT_GeomID::ID_InnerCorner, TableRow->InnerCorner, "InnerCornerComponents");
		InitialiseTileData(EWT_GeomID::ID_OuterCorner, TableRow->OuterCorner, "OuterCornerComponents");



		InitialiseTileData(EWT_GeomID::ID_ThinCorner, TableRow->ThinCorner, "ThinCornerComponents");
		InitialiseTileData(EWT_GeomID::ID_ThinWall, TableRow->ThinWall, "ThinWallComponents");
		InitialiseTileData(EWT_GeomID::ID_ThinWallEnd, TableRow->ThinWallEnd, "ThinWallEndComponents");

		InitialiseTileData(EWT_GeomID::ID_TConnector, TableRow->TConnector, "TConnectorComponents");
		InitialiseTileData(EWT_GeomID::ID_XConnector, TableRow->XConnector, "XConnectorComponents");
		InitialiseTileData(EWT_GeomID::ID_Block, TableRow->Block, "BlockComponents");

		



	
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
					UE_LOG(LogTemp, Warning, TEXT("Updating Tile: (%d | %d | %d): %s"), x, y, z, *UEnum::GetValueAsString<EWT_GeomID>(ID));
					FGridVisual Temp;
					Temp.TileID = EWT_GeomID::ID_Wall;
					Temp.Channel = 0;
					Temp.StackID = EWT_StackID::ID_Mid;
					Temp.Rot = 0;

					UpdateTile(CurrentPos, Temp);

					//Gen->GetTileData(CurrentPos)

				}
				


			}
		}
	}
}

void AWT_WorldChunk::OnConstruction(const FTransform& Transform)
{
	
}



void AWT_WorldChunk::InitialiseTileData(EWT_GeomID TileID, FTile_AssetTypes Asset, FName CompName)
{
	FInstanceStack TempStack;

	FTile_ComponentData TempData;

	TempData.BottomComponent = NewObject<UInstancedStaticMeshComponent>(this, CompName);
	TempData.BottomComponent->RegisterComponent();
	TempData.BottomComponent->SetStaticMesh(Asset.Bottom);

	TempData.MiddleComponent = NewObject<UInstancedStaticMeshComponent>(this, CompName);
	TempData.MiddleComponent->RegisterComponent();
	TempData.MiddleComponent->SetStaticMesh(Asset.Middle);
	

	TempData.TopComponent = NewObject<UInstancedStaticMeshComponent>(this, CompName);
	TempData.TopComponent->RegisterComponent();
	TempData.TopComponent->SetStaticMesh(Asset.Top);
	


	TempStack.ComponentList.Add(TempData);
	ComponentList.Add(EWT_GeomID::ID_Block, TempStack);
}


void AWT_WorldChunk::UpdateTile(FVector Position, FGridVisual Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Updating Tile"));

	if (TileKeys.Find(Position))
	{
		UE_LOG(LogTemp, Warning, TEXT("Key Found, removing instance"));
		TileKeys[Position].Comp->RemoveInstance(TileKeys[Position].Index);

		TileKeys[Position].Index = 0;
		TileKeys[Position].Comp = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No key found, Creating key"));
		TileKeys.Add(Position);
		TileKeys[Position].Comp = nullptr;
		TileKeys[Position].Index = 0;
	}
	

	switch (Data.StackID)
	{
	case EWT_StackID::ID_Bottom:
		TileKeys[Position].Index = ComponentList[Data.TileID].ComponentList[Data.Channel].BottomComponent->AddInstance(FTransform(FRotator(0, 0, Data.Rot), Position * TileScale, FVector(1,1,1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].ComponentList[Data.Channel].BottomComponent;
		break;
	case EWT_StackID::ID_Mid:
		TileKeys[Position].Index = ComponentList[Data.TileID].ComponentList[0].MiddleComponent->AddInstance(FTransform(FRotator(0, 0, Data.Rot), Position * TileScale, FVector(1, 1, 1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].ComponentList[0].MiddleComponent;
		break;
	case EWT_StackID::ID_Top:
		TileKeys[Position].Index = 	ComponentList[Data.TileID].ComponentList[Data.Channel].TopComponent->AddInstance(FTransform(FRotator(0, 0, Data.Rot), Position * TileScale, FVector(1, 1, 1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].ComponentList[Data.Channel].TopComponent;
		break;

	}

	




}


