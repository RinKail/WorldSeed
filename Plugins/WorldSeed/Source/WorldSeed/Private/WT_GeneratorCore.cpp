// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GeneratorCore.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WorldSeed/Public/WT_Landmark_Base.h"
#include "Kismet/GameplayStatics.h"

AWT_GeneratorCore::AWT_GeneratorCore()
{

}

void AWT_GeneratorCore::BuildGrid()
{
	



	Reset();
	int GridX = GridScale.X;
	int GridY = GridScale.Y;
	int GridZ = GridScale.Z;

	int ChunkX = ChunkScale.X;
	int ChunkY = ChunkScale.Y;

	Stored_GridScale = GridScale;
	Stored_ChunkScale = ChunkScale;

	RefreshContainers();

	if ((GridX != 0 && GridY != 0 && GridZ != 0) && (ChunkX != 0 && ChunkY != 0))
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

		for (int i = 0; i < SubLandmarks.Num(); i++)
		{
			SubLandmarks[i]->ApplyLandmark(this);
		}

		for (int i = 0; i < AddLandmarks.Num(); i++)
		{
			AddLandmarks[i]->ApplyLandmark(this);
		}

		GenerateGeometryMap();

		

		for (int i = 0; i < ChunkList.Num(); i++)
		{
			ChunkList[i]->GenerateChunk(this, ChunkScale, GridZ);

		}

	}

}

void AWT_GeneratorCore::RefreshContainers()
{

	ChunkList.Empty();
	AddLandmarks.Empty();
	SubLandmarks.Empty();

	TArray<AActor*> ChunkActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWT_WorldChunk::StaticClass(), ChunkActors);
	if (ChunkActors.Num() > 0 )
	{
		for (int i = 0; i < ChunkActors.Num(); i++)
		{
			ChunkList.Add(Cast<AWT_WorldChunk>(ChunkActors[i]));
			ChunkPositions.Add(ChunkActors[i]->GetActorLocation());
		}
		
	}
	TArray<AActor*> LandmarksActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWT_Landmark_Base::StaticClass(), LandmarksActors);
	if (LandmarksActors.Num() > 0)
	{
		for (int i = 0; i < LandmarksActors.Num(); i++)
		{
			AWT_Landmark_Base* Temp = Cast<AWT_Landmark_Base>(ChunkActors[i]);
			if (Temp->IsLandmarkAdditive())
			{
				AddLandmarks.Add(Temp);
			}
			else
			{
				SubLandmarks.Add(Temp);
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
	ChunkPositions.Empty();
	FActorSpawnParameters SpawnParams;
	for (int i = 0; i < ChunkList.Num(); i++)
	{
		FVector Pos = ChunkList[i]->GetActorLocation();
		
		ChunkList[i]->Destroy();

		AWT_WorldChunk* Temp = GetWorld()->SpawnActor<AWT_WorldChunk>();
		Temp->SetActorLocation(ChunkPositions[i]);

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