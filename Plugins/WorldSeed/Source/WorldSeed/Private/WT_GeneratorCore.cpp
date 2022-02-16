// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GeneratorCore.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WorldSeed/Public/WT_Landmark_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"

AWT_GeneratorCore::AWT_GeneratorCore()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Table(TEXT("DataTable'/WorldSeed/DT_WorldSeed_TileData.DT_WorldSeed_TileData'"));


	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	if (Table.Succeeded())
	{
		DataTable_Geometry = Table.Object;
	}


	InitialiseMeshComponents();
}

void AWT_GeneratorCore::BuildGrid()
{
	



	Reset();
	int GridX = GridScale.X;
	int GridY = GridScale.Y;
	int GridZ = GridScale.Z;

	

	Stored_GridScale = GridScale;
	Stored_ChunkScale = ChunkScale;


	if ((GridX != 0 && GridY != 0 && GridZ != 0))
	{


		



		for (int z = 0; z < GridZ; ++z)
		{
			for (int x = 0; x < GridX; ++x)
			{
				for (int y = 0; y < GridY; ++y)
				{

				
					Grid_Data.Add(FVector(x, y, z), false);
					Grid_Visual.Add(FVector(x, y, z));
					Grid_Structure.Add(FVector(x, y, z), EWT_SpaceID::ID_Empty);

				}
			}

		}
		for (int x = 0; x < GridX; ++x)
		{
			for (int y = 0; y < GridY; ++y)
			{


				for (int i = 0; i < FillHeight; i++)
				{
					Grid_Data.Add(FVector(x, y, i), true);
				}
				

			}
		}


		//Seperates the landmarks by type, this is required to make sure that additive landmarks are no completly overwritten by subtractive ones.
		//To-DO:- Look into other methods of seperating, perhaps using layers.

		for (int i = 0; i < SubLandmarks.Num(); i++)
		{
			SubLandmarks[i]->ApplyLandmark(this);
		}

		for (int i = 0; i < AddLandmarks.Num(); i++)
		{
			AddLandmarks[i]->ApplyLandmark(this);
		}

		GenerateGeometryMap();

		for (int z = 0; z < GridZ; ++z)
		{
			for (int x = 0; x < GridY; ++x)
			{
				for (int y = 0; y < GridX; ++y)
				{
					FVector CurrentPos = FVector(x, y, z) + (GetActorLocation() / TileScale);

					EWT_GeomID ID = GetTileData(FVector(x, y, z)).TileID;
					if (ID != EWT_GeomID::ID_Empty && ID != EWT_GeomID::ID_Floor && IsEmptyAdjacent(FVector(x,y,z)))
					{



						UpdateTile(FVector(x, y, z), GetTileData(FVector(x, y, z)));

						//Gen->GetTileData(CurrentPos)

					}



				}
			}
		}

		

	}

}

void AWT_GeneratorCore::BeginPlay()
{
	BuildGrid();
}

void AWT_GeneratorCore::OnConstruction(const FTransform& Transform)
{
	


	BuildGrid();
	
	
}
void AWT_GeneratorCore::AddChunk(FVector Position)
{
	
}



void AWT_GeneratorCore::Reset()
{
	if (TileKeys.Num() > 0)
	{

		for (int z = 0; z < GridScale.Z; ++z)
		{
			for (int x = 0; x < GridScale.Y; ++x)
			{
				for (int y = 0; y < GridScale.X; ++y)
				{
					FVector Position = FVector(x, y, z);


					if (TileKeys.Find(Position) != nullptr)
					{
						UE_LOG(LogTemp, Warning, TEXT("Key Found, removing instance: [%f | %f | %f]"), Position.X, Position.Y, Position.Z);
						if (TileKeys[Position].Comp != nullptr)
						{
							TileKeys[Position].Comp->RemoveInstance(TileKeys[Position].Index);
						}

						TileKeys[Position].Index = 0;
						TileKeys[Position].Comp = nullptr;
					}

				}
			}
		}


	}

	if (Grid_Data.Num() > 0)
	{
		Grid_Data.Empty();
	}

	if (Grid_Visual.Num() > 0)
	{
		Grid_Visual.Empty();
	}
	if (Grid_Structure.Num() > 0)
	{
		Grid_Structure.Empty();
	 }

	
	

}

void AWT_GeneratorCore::GenerateGeometryMap()
{
	int GridX = GridScale.X;
	int GridY = GridScale.Y;
	int GridZ = GridScale.Z;

	int ChunkX = ChunkScale.X;
	int ChunkY = ChunkScale.Y;

	
	for (int z = 0; z < GridZ; ++z)
	{
		for (int x = 0; x < GridX; ++x)
		{
			for (int y = 0; y < GridY; ++y)
			{
				if (GetCellState(FVector(x, y, z)))
				{
					if (IsEdge(FVector(x, y, z)))
					{
						Grid_Structure[FVector(x, y, z)] = EWT_SpaceID::ID_Edge;
					}
					else
					{
						Grid_Structure[FVector(x, y, z)] = EWT_SpaceID::ID_Raised;
					}
				}
				else
				{
					if (IsFloor(FVector(x, y, z)))
					{
						Grid_Structure[FVector(x, y, z)] = EWT_SpaceID::ID_Floor;
					}
					else
					{
						Grid_Structure[FVector(x, y, z)] = EWT_SpaceID::ID_Empty;
					}
				}

			}
		}
	}

	
	for (int z = 0; z < GridZ; ++z)
	{
		for (int x = 0; x < GridX; ++x)
		{
			for (int y = 0; y < GridY; ++y)
			{
				Grid_Visual[FVector(x, y, z)].Rot = 0;
				Grid_Visual[FVector(x, y, z)].Channel = 0;
				Grid_Visual[FVector(x, y, z)].StackID = EWT_StackID::ID_Mid;
				switch (Grid_Structure[FVector(x,y,z)])
				{
				case EWT_SpaceID::ID_Empty:
					Grid_Visual[FVector(x, y, z)].TileID = EWT_GeomID::ID_Empty;
				
					break;
				case EWT_SpaceID::ID_Raised:
					Grid_Visual[FVector(x, y, z)].TileID = EWT_GeomID::ID_Raised;
					break;
				case EWT_SpaceID::ID_Floor:
					Grid_Visual[FVector(x, y, z)].TileID = EWT_GeomID::ID_Floor;
					break;
				case EWT_SpaceID::ID_Edge:
					Grid_Visual[FVector(x, y, z)].TileID = EWT_GeomID::ID_Wall;
					break;
				}

			}
		}
	}




}

bool AWT_GeneratorCore::IsEdge(FVector Pos)
{
	TArray<FVector> PosList;
	PosList.Add(FVector(1, -1, 0));
	PosList.Add(FVector(-1, 1, 0));
	PosList.Add(FVector(-1, -1, 0));
	PosList.Add(FVector(1, 1, 0));

	PosList.Add(FVector(0, -1, 0));
	PosList.Add(FVector(-1, 0, 0));
	PosList.Add(FVector(0, 1, 0));
	PosList.Add(FVector(1, 0, 0));

	for (int i = 0; i < PosList.Num(); i++)
	{
		if (!GetCellState(Pos + PosList[i])) return true;
	}
	return false;
}

bool AWT_GeneratorCore::IsFloor(FVector Pos)
{
	if (!GetCellState(Pos) && GetCellState(Pos + FVector(0, 0, -1)) && !GetCellState(Pos + FVector(0, 0, 1))) return true;
	else return false;
}

void AWT_GeneratorCore::StoreLandmark(AWT_Landmark_Base* In)
{
	if (In->IsLandmarkAdditive())
	{
		AddLandmarks.Add(In);
	}
	else
	{
		SubLandmarks.Add(In);
	}

}

bool AWT_GeneratorCore::IsEmptyAdjacent(FVector Pos)
{
	TArray<FVector> PosList;
	PosList.Add(FVector(1, -1, 0));
	PosList.Add(FVector(-1, 1, 0));
	PosList.Add(FVector(-1, -1, 0));
	PosList.Add(FVector(1, 1, 0));

	PosList.Add(FVector(0, -1, 0));
	PosList.Add(FVector(-1, 0, 0));
	PosList.Add(FVector(0, 1, 0));
	PosList.Add(FVector(1, 0, 0));

	PosList.Add(FVector(0,0, 1));
	PosList.Add(FVector(0, 0, -1));

	for (int i = 0; i < PosList.Num(); i++)
	{
		if (!GetCellState(Pos + PosList[i])) return true;
	}
	return false;
}






//
// TILE SETUP
//


void AWT_GeneratorCore::InitialiseTileData(EWT_GeomID TileID, FTile_AssetTypes Asset, FString CompName)
{
	FInstanceStack TempStack;

	FTile_ComponentData TempData;

	FName Top = FName(CompName + ".Top");
	FName Mid = FName(CompName + ".Mid");
	FName Bot = FName(CompName + ".Bot");

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

	if (TempData.MiddleComponent != nullptr && TempData.TopComponent != nullptr && TempData.BottomComponent != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Components created successfully"));
	}



	ComponentList.Add(TileID, TempData);
}





void AWT_GeneratorCore::UpdateTile(FVector Position, FGridVisual Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Updating Tile"));

	if (TileKeys.Find(Position) != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Key Found, removing instance: [%f | %f | %f]"), Position.X, Position.Y, Position.Z);
		if (TileKeys[Position].Comp != nullptr)TileKeys[Position].Comp->RemoveInstance(TileKeys[Position].Index);

		TileKeys[Position].Index = 0;
		TileKeys[Position].Comp = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No key found, Creating key: [%f | %f | %f]"), Position.X, Position.Y, Position.Z);
		TileKeys.Add(Position);
		TileKeys[Position].Comp = nullptr;
		TileKeys[Position].Index = 0;
	}

	if (ComponentList.Find(Data.TileID))
	{
		UE_LOG(LogTemp, Warning, TEXT("Component List located: "));

	}

	switch (Data.StackID)
	{
	case EWT_StackID::ID_Bottom:
		TileKeys[Position].Index = ComponentList[Data.TileID].BottomComponent->AddInstance(FTransform(FRotator(0, 0, Data.Rot), Position * TileScale, FVector(1, 1, 1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].BottomComponent;
		break;
	case EWT_StackID::ID_Mid:
		TileKeys[Position].Index = ComponentList[Data.TileID].MiddleComponent->AddInstance(FTransform(FRotator(0, 0, Data.Rot), Position * TileScale, FVector(1, 1, 1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].MiddleComponent;
		break;
	case EWT_StackID::ID_Top:
		TileKeys[Position].Index = ComponentList[Data.TileID].TopComponent->AddInstance(FTransform(FRotator(0, 0, Data.Rot), Position * TileScale, FVector(1, 1, 1)));
		TileKeys[Position].Comp = ComponentList[Data.TileID].TopComponent;
		break;

	}






}



void AWT_GeneratorCore::InitialiseMeshComponents()
{
	static const FString ContextString(TEXT(""));
	FTile_TableRow* TableRow = DataTable_Geometry->FindRow<FTile_TableRow>(FName(TEXT("Placeholder")), ContextString, true);
	if (TableRow)
	{
		InitialiseTileData(EWT_GeomID::ID_Raised, TableRow->Raised, "RaisedComponents");

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