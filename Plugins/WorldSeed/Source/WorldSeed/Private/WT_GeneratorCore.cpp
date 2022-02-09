// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GeneratorCore.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WorldSeed/Public/WT_Landmark_Base.h"

AWT_GeneratorCore::AWT_GeneratorCore()
{

}

void AWT_GeneratorCore::BuildGrid()
{
	int GridX = GridScale.X;
	int GridY = GridScale.Y;
	int GridZ = GridScale.Z;

	int ChunkX = ChunkScale.X;
	int ChunkY = ChunkScale.Y;
	int ChunkZ = ChunkScale.Z;

	if ((GridX != 0 && GridY != 0 && GridZ != 0) && (ChunkX != 0 && ChunkY != 0 && ChunkZ != 0))
	{


		for (int x = 0; x < GridX / ChunkX; ++x)
		{
			for (int y = 0; y < GridY / ChunkY; ++y)
			{
				AddChunk(FVector(x,y,0));
			}

		}



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


				Grid_Data.Add(FVector(x, y, 0), true);
				

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

		for (int z = 0; z < ChunkZ; ++z)
		{
			for (int x = 0; x < ChunkX; ++x)
			{
				for (int y = 0; y < ChunkY; ++y)
				{
					ChunkList[FVector(x, y, 0)]->GenerateChunk(this, ChunkScale);
				}
			}
		}
	}

}
void AWT_GeneratorCore::AddChunk(FVector Position)
{
	ChunkList.Add(Position, GetWorld()->SpawnActor<AWT_WorldChunk>(FVector(((Position.X * (GridScale.X / ChunkScale.X)) * TileScale), ((Position.Y * (GridScale.Y / ChunkScale.Y)) * TileScale), 0.0f), FRotator(0, 0, 0)));
}



void AWT_GeneratorCore::GenerateGeometryMap()
{
	int GridX = GridScale.X;
	int GridY = GridScale.Y;
	int GridZ = GridScale.Z;

	int ChunkX = ChunkScale.X;
	int ChunkY = ChunkScale.Y;
	int ChunkZ = ChunkScale.Z;
	
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