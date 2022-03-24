// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WorldSeed/Public/WT_Resources.h"
#include "WorldSeed/Public/WT_GeneratorCore.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AWT_WorldChunk::AWT_WorldChunk()
{


	

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//ChunkSize = 16;

	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	 ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMesh (TEXT("/Game/PlaceholderAssets/NewAssets/WorldSeedPlaceholder_Floor.WorldSeedPlaceholder_Floor")); 
	 ConstructorHelpers::FObjectFinder<UStaticMesh> RaisedMesh (TEXT("/Game/PlaceholderAssets/NewAssets/WorldSeedPlaceholder_Raised.WorldSeedPlaceholder_Raised"));
	
	

	
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

		InitialiseTileData(EWT_GeomID::ID_Floor, TableRow->Raised, "FloorComponents");

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

void AWT_WorldChunk::GenerateChunk(AWT_GeneratorCore* Gen, FVector2D ChunkScale, int WorldHeight)
{
	GridSize = FVector(ChunkScale.X, ChunkScale.Y, WorldHeight);


	for (int z = 0; z < WorldHeight; ++z)
	{
		for (int x = 0; x < ChunkScale.X; ++x)
		{
			for (int y = 0; y < ChunkScale.Y; ++y)
			{
				FVector CurrentPos = FVector(x, y, z) + (GetActorLocation() / TileScale);
			


				FGridVisual Visual;

				EWT_GeomID ID = Gen->GetTileData(CurrentPos).TileID;
				EWT_StackID Layer = Gen->GetTileData(CurrentPos).StackID;
				if (ID != EWT_GeomID::ID_Empty && ID != EWT_GeomID::ID_Floor && Gen->IsEmptyAdjacent(CurrentPos))
				{
					Visual = Gen->GetTileData(CurrentPos);
					

					UpdateTile(CurrentPos, Gen->GetTileData(CurrentPos));

					//Gen->GetTileData(CurrentPos)

				}
				else
				{
					Visual.TileID = EWT_GeomID::ID_Empty;
				}
				if (ID == EWT_GeomID::ID_Floor && Layer == EWT_StackID::ID_Bottom)
				{

					Visual.StackID = EWT_StackID::ID_Bottom;
					Visual.TileID = EWT_GeomID::ID_Floor;


					UpdateTile(CurrentPos, Visual);



					UE_LOG(LogTemp, Warning, TEXT("Floor Found"));
				}
				

				StoredData.Add(FVector(x, y, z), Visual);

			}
		}
	}
}

void AWT_WorldChunk::BeginPlay()
{
	Super::BeginPlay();


	
	

	//UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY"));
}

void AWT_WorldChunk::OnConstruction(const FTransform& Transform)
{
	//InitialiseChunk();

	//Generator->BuildGrid();

	UE_LOG(LogTemp, Warning, TEXT("RUNNING CONSTRUCTOR"));
}



void AWT_WorldChunk::InitialiseTileData(EWT_GeomID TileID, FTile_AssetTypes Asset, FString CompName)
{
	FInstanceStack TempStack;

	FTile_ComponentData TempData;

	FName Top = FName(CompName + ".Top");
	FName Mid = FName(CompName + ".Mid");
	FName Bot = FName(CompName + ".Bot");
	FName TopUnwalk = FName(CompName + ".TopUnwalkable");
	FName Single = FName(CompName + ".Single");

	TempData.BottomComponent = NewObject<UInstancedStaticMeshComponent>(this, Bot);
	TempData.BottomComponent->SetupAttachment(SceneRoot);
	TempData.BottomComponent->RegisterComponent();
	TempData.BottomComponent->SetStaticMesh(Asset.Bottom);

	TempData.MiddleComponent = NewObject<UInstancedStaticMeshComponent>(this, Mid);
	TempData.MiddleComponent->SetupAttachment(SceneRoot);
	TempData.MiddleComponent->RegisterComponent();
	TempData.MiddleComponent->SetStaticMesh(Asset.Middle);
	

	TempData.TopComponent = NewObject<UInstancedStaticMeshComponent>(this, Top);
	TempData.TopComponent->SetupAttachment(SceneRoot);
	TempData.TopComponent->RegisterComponent();
	TempData.TopComponent->SetStaticMesh(Asset.Top);

	TempData.SingleComponent = NewObject<UInstancedStaticMeshComponent>(this, Single);
	TempData.SingleComponent->SetupAttachment(SceneRoot);
	TempData.SingleComponent->RegisterComponent();
	TempData.SingleComponent->SetStaticMesh(Asset.Single);

	TempData.TopUnwalkableComponent = NewObject<UInstancedStaticMeshComponent>(this, TopUnwalk);
	TempData.TopUnwalkableComponent->SetupAttachment(SceneRoot);
	TempData.TopUnwalkableComponent->RegisterComponent();
	TempData.TopUnwalkableComponent->SetStaticMesh(Asset.UnwalkableTop);
	
	


	
	ComponentList.Add(TileID, TempData);
}

void AWT_WorldChunk::InitialiseChunk()
{
	if (StoredData.Num() > 0)
	{
		for (int z = 0; z < GridSize.Z; ++z)
		{
			for (int x = 0; x < GridSize.X; ++x)
			{
				for (int y = 0; y < GridSize.Y; ++y)
				{
					FVector CurrentPos = FVector(x, y, z) + (GetActorLocation() / TileScale);

					EWT_GeomID ID = StoredData[FVector(x, y, z)].TileID;
					if (ID != EWT_GeomID::ID_Empty)
					{



						UpdateTile(FVector(x, y, z), StoredData[FVector(x, y, z)]);

						//Gen->GetTileData(CurrentPos)

					}



				}
			}
		}
	}
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
		TileKeys[Position].Comp = nullptr;
		TileKeys[Position].Index = 0;
	}
	
	if (ComponentList.Find(Data.TileID))
	{
		
		
	}

	switch (Data.StackID)
	{
	case EWT_StackID::ID_Bottom:
		TileKeys[Position].Index = ComponentList[Data.TileID].BottomComponent->AddInstance(FTransform(FRotator(0, Data.Rot, 0), Position * TileScale, FVector(1,1,1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].BottomComponent;
		break;
	case EWT_StackID::ID_Mid:
		TileKeys[Position].Index = ComponentList[Data.TileID].MiddleComponent->AddInstance(FTransform(FRotator(0, Data.Rot, 0), Position * TileScale, FVector(1, 1, 1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].MiddleComponent;
		break;
	case EWT_StackID::ID_Top:
		TileKeys[Position].Index = 	ComponentList[Data.TileID].TopUnwalkableComponent->AddInstance(FTransform(FRotator(0, Data.Rot, 0), Position * TileScale, FVector(1, 1, 1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].TopUnwalkableComponent;
		break;
	case EWT_StackID::ID_TopWalkable:
		TileKeys[Position].Index = ComponentList[Data.TileID].TopComponent->AddInstance(FTransform(FRotator(0, Data.Rot, 0), Position * TileScale, FVector(1, 1, 1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].TopComponent;
		break;
	case EWT_StackID::ID_Single:
		TileKeys[Position].Index = ComponentList[Data.TileID].SingleComponent->AddInstance(FTransform(FRotator(0, Data.Rot, 0), Position * TileScale, FVector(1, 1, 1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].SingleComponent;
		break;

	}

	




}


