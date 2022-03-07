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


	if ((GridX != 0 && GridY != 0 && GridZ != 0) && (ChunkX != 0 && ChunkY != 0))
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

		

		for (int x = 0; x < GridX / ChunkX; ++x)
		{
			for (int y = 0; y < GridY / ChunkY; ++y)
			{
				ChunkList[FVector(x, y, 0)]->GenerateChunk(this, ChunkScale, GridZ);
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
	if (!ChunkList.Find(Position))
	{
		AWT_WorldChunk* Chunk = GetWorld()->SpawnActor<AWT_WorldChunk>(FVector(((Position.X * ChunkScale.X) * TileScale), ((Position.Y * ChunkScale.Y) * TileScale), 0.0f), FRotator(0, 0, 0));
		ChunkList.Add(Position, Chunk);
	}
}



void AWT_GeneratorCore::Reset()
{
	if (ChunkList.Num() > 0)
	{
		for (int x = 0; x < Stored_GridScale.X / Stored_ChunkScale.X; ++x)
		{
			for (int y = 0; y < Stored_GridScale.Y / Stored_ChunkScale.Y; ++y)
			{
				if (ChunkList.Find(FVector(x,y,0)))ChunkList[FVector(x, y, 0)]->Destroy();
			}

		}
		ChunkList.Empty();
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

				FVector EmptyAdjacent = GetAdjacentEmpty_Directional(FVector(x,y,z));

				
				
				

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
				
					AssignVisual_Edge(FVector(x,y,z));



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

FVector AWT_GeneratorCore::GetAdjacentEmpty_Directional(FVector Position)
{
	FVector ReturnValue = FVector(0, 0, 0);
	if (IsValidCoordinate(Position))
	{
		if (IsValidCoordinate(Position + FVector(0, 0, 1)) && Grid_Structure[Position + FVector(0,0,1)] == EWT_SpaceID::ID_Empty)
		{
			ReturnValue.Z = 1;
			if (IsValidCoordinate(Position + FVector(0, 0, -1)) && Grid_Structure[Position + FVector(0, 0, -1)] == EWT_SpaceID::ID_Empty)
			{
				ReturnValue.Z = 2;
			}
		}
		else if (IsValidCoordinate(Position + FVector(0, 0, -1)) && Grid_Structure[Position + FVector(0, 0, -1)] == EWT_SpaceID::ID_Empty)
		{
			ReturnValue.Z = -1;
			if (IsValidCoordinate(Position + FVector(0, 0, 1)) && Grid_Structure[Position + FVector(0, 0, 1)] == EWT_SpaceID::ID_Empty)
			{
				ReturnValue.Z = 2;
			}
		}


	}
	if (IsValidCoordinate(Position))
	{
		if (IsValidCoordinate(Position + FVector(0, 1, 0)) && Grid_Structure[Position + FVector(0, 1, 0)] == EWT_SpaceID::ID_Empty)
		{
			ReturnValue.Y = 1;
			if (IsValidCoordinate(Position + FVector(0, -1, 0)) && Grid_Structure[Position + FVector(0, -1, 0)] == EWT_SpaceID::ID_Empty)
			{
				ReturnValue.Y = 2;
			}
		}
		else if (IsValidCoordinate(Position + FVector(0, -1, 0)) && Grid_Structure[Position + FVector(0, -1, 0)] == EWT_SpaceID::ID_Empty)
		{
			ReturnValue.Y = -1;
			if (IsValidCoordinate(Position + FVector(0, 1, 0)) && Grid_Structure[Position + FVector(0, 1, 0)] == EWT_SpaceID::ID_Empty)
			{
				ReturnValue.Y = 2;
			}
		}


	}
	if (IsValidCoordinate(Position))
	{
		if (IsValidCoordinate(Position + FVector(1, 0, 0)) && Grid_Structure[Position + FVector(1, 0, 0)] == EWT_SpaceID::ID_Empty)
		{
			ReturnValue.X = 1;
			if (IsValidCoordinate(Position + FVector(-1, 0, 0)) && Grid_Structure[Position + FVector(-1, 0, 0)] == EWT_SpaceID::ID_Empty)
			{
				ReturnValue.X = 2;
			}
		}
		else if (IsValidCoordinate(Position + FVector(-1, 0, 0)) && Grid_Structure[Position + FVector(-1, 0, 0)] == EWT_SpaceID::ID_Empty)
		{
			ReturnValue.X = -1;
			if (IsValidCoordinate(Position + FVector(1, 0, 0)) && Grid_Structure[Position + FVector(1, 0, 0)] == EWT_SpaceID::ID_Empty)
			{
				ReturnValue.X = 2;
			}
		}


	}





	return ReturnValue;
}

FVector AWT_GeneratorCore::GetAdjacentAir_Directional(FVector Position)
{
	FVector ReturnValue = FVector(0, 0, 0);
	/*
	* if (IsTileType(Position + FVector(0, 0, 1), EWT_SpaceID::ID_Empty) || IsTileType(Position + FVector(0, 0, 1), EWT_SpaceID::ID_Floor))
	{
		ReturnValue.Z = 1;
		if (IsTileType(Position + FVector(0, 0, -1), EWT_SpaceID::ID_Empty) || IsTileType(Position + FVector(0, 0, 1), EWT_SpaceID::ID_Floor))
		{
			ReturnValue.Z = 2;
		}
	}
	else if (IsTileType(Position + FVector(0, 0, -1), EWT_SpaceID::ID_Empty) || IsTileType(Position + FVector(0, 0, -1), EWT_SpaceID::ID_Floor))
	{
		ReturnValue.Z = -1;
		if (IsTileType(Position + FVector(0, 0, 1), EWT_SpaceID::ID_Empty) || IsTileType(Position + FVector(0, 0, 1), EWT_SpaceID::ID_Floor))
		{
			ReturnValue.Z = 2;
		}
	}
	if (IsTileType(Position + FVector(0, 1, 0), EWT_SpaceID::ID_Empty) || IsTileType(Position + FVector(0, 1, 0), EWT_SpaceID::ID_Floor))
	{
		ReturnValue.Y = 1;
		if (IsTileType(Position + FVector(0, -1, 0), EWT_SpaceID::ID_Empty) || IsTileType(Position + FVector(0, -1, 0), EWT_SpaceID::ID_Floor))
		{
			ReturnValue.Y = 2;
		}
	}
	else if (IsTileType(Position + FVector(0, -1, 0), EWT_SpaceID::ID_Empty) || IsTileType(FVector(0, -1, 0), EWT_SpaceID::ID_Floor))
	{
		ReturnValue.Y = -1;
		if (IsTileType(Position + FVector(0, 1, 0), EWT_SpaceID::ID_Empty) || IsTileType(Position + FVector(0, 1, 0), EWT_SpaceID::ID_Floor))
		{
			ReturnValue.Y = 2;
		}
	}
	if (IsTileType(Position + FVector(1, 0, 0), EWT_SpaceID::ID_Empty) || IsTileType(Position + FVector(1, 0, 0), EWT_SpaceID::ID_Floor))
	{
		ReturnValue.X = 1;
		if (IsTileType(Position + FVector(-1, 0, 0), EWT_SpaceID::ID_Empty) || IsTileType(Position + FVector(-1, 0, 0), EWT_SpaceID::ID_Floor))
		{
			ReturnValue.X = 2;
		}
	}
	else if (IsTileType(Position + FVector(-1, 0, 0), EWT_SpaceID::ID_Empty) || IsTileType(Position + FVector(-1, 0, 0), EWT_SpaceID::ID_Floor))
	{
		ReturnValue.X = -1;
		if (IsTileType(Position + FVector(1, 0, 0), EWT_SpaceID::ID_Empty) || IsTileType(Position + FVector(1, 0, 0), EWT_SpaceID::ID_Floor))
		{
			ReturnValue.X = 2;
		}
	}
	*/


	if (IsTileType(Position + FVector(0, 0, 1), EWT_SpaceID::ID_Raised))
	{
		ReturnValue.Z = 1;
		if (IsTileType(Position + FVector(0, 0, -1), EWT_SpaceID::ID_Raised))
		{
			ReturnValue.Z = 2;
		}
	}
	else if (IsTileType(Position + FVector(0, 0, -1), EWT_SpaceID::ID_Raised) )
	{
		ReturnValue.Z = -1;
		if (IsTileType(Position + FVector(0, 0, 1), EWT_SpaceID::ID_Raised) )
		{
			ReturnValue.Z = 2;
		}
	}
	if (IsTileType(Position + FVector(0, 1, 0), EWT_SpaceID::ID_Raised))
	{
		ReturnValue.Y = 1;
		if (IsTileType(Position + FVector(0, -1, 0), EWT_SpaceID::ID_Raised))
		{
			ReturnValue.Y = 2;
		}
	}
	else if (IsTileType(Position + FVector(0, -1, 0), EWT_SpaceID::ID_Raised))
	{
		ReturnValue.Y = -1;
		if (IsTileType(Position + FVector(0, 1, 0), EWT_SpaceID::ID_Raised))
		{
			ReturnValue.Y = 2;
		}
	}
	if (IsTileType(Position + FVector(1, 0, 0), EWT_SpaceID::ID_Raised))
	{
		ReturnValue.X = 1;
		if (IsTileType(Position + FVector(-1, 0, 0), EWT_SpaceID::ID_Raised))
		{
			ReturnValue.X = 2;
		}
	}
	else if (IsTileType(Position + FVector(-1, 0, 0), EWT_SpaceID::ID_Raised) )
	{
		ReturnValue.X = -1;
		if (IsTileType(Position + FVector(1, 0, 0), EWT_SpaceID::ID_Raised) )
		{
			ReturnValue.X = 2;
		}
	}


	return ReturnValue;
}

FVector AWT_GeneratorCore::GetEdgeAdjacent_Directional(FVector Position)
{
	FVector ReturnValue = FVector(0, 0, 0);
	if (IsValidCoordinate(Position))
	{
		if (IsValidCoordinate(Position + FVector(0, 0, 1)) && Grid_Structure[Position + FVector(0, 0, 1)] == EWT_SpaceID::ID_Edge)
		{
			ReturnValue.Z = 1;
			if (IsValidCoordinate(Position + FVector(0, 0, -1)) && Grid_Structure[Position + FVector(0, 0, -1)] == EWT_SpaceID::ID_Edge)
			{
				ReturnValue.Z = 2;
			}
		}
		else if (IsValidCoordinate(Position + FVector(0, 0, -1)) && Grid_Structure[Position + FVector(0, 0, -1)] == EWT_SpaceID::ID_Edge)
		{
			ReturnValue.Z = -1;
			if (IsValidCoordinate(Position + FVector(0, 0, 1)) && Grid_Structure[Position + FVector(0, 0, 1)] == EWT_SpaceID::ID_Edge)
			{
				ReturnValue.Z = 2;
			}
		}


	}
	if (IsValidCoordinate(Position))
	{
		if (IsValidCoordinate(Position + FVector(0, 1, 0)) && Grid_Structure[Position + FVector(0, 1, 0)] == EWT_SpaceID::ID_Edge)
		{
			ReturnValue.Y = 1;
			if (IsValidCoordinate(Position + FVector(0, -1, 0)) && Grid_Structure[Position + FVector(0, -1, 0)] == EWT_SpaceID::ID_Edge)
			{
				ReturnValue.Y = 2;
			}
		}
		else if (IsValidCoordinate(Position + FVector(0, -1, 0)) && Grid_Structure[Position + FVector(0, -1, 0)] == EWT_SpaceID::ID_Edge)
		{
			ReturnValue.Y = -1;
			if (IsValidCoordinate(Position + FVector(0, 1, 0)) && Grid_Structure[Position + FVector(0, 1, 0)] == EWT_SpaceID::ID_Edge)
			{
				ReturnValue.Y = 2;
			}
		}


	}
	if (IsValidCoordinate(Position))
	{
		if (IsValidCoordinate(Position + FVector(1, 0, 0)) && Grid_Structure[Position + FVector(1, 0, 0)] == EWT_SpaceID::ID_Edge)
		{
			ReturnValue.X = 1;
			if (IsValidCoordinate(Position + FVector(-1, 0, 0)) && Grid_Structure[Position + FVector(-1, 0, 0)] == EWT_SpaceID::ID_Edge)
			{
				ReturnValue.X = 2;
			}
		}
		else if (IsValidCoordinate(Position + FVector(-1, 0, 0)) && Grid_Structure[Position + FVector(-1, 0, 0)] == EWT_SpaceID::ID_Edge)
		{
			ReturnValue.X = -1;
			if (IsValidCoordinate(Position + FVector(1, 0, 0)) && Grid_Structure[Position + FVector(1, 0, 0)] == EWT_SpaceID::ID_Edge)
			{
				ReturnValue.X = 2;
			}
		}


	}

	return ReturnValue;
}

void AWT_GeneratorCore::AssignVisual_Edge(FVector Position)
{

	FVector EmptyAdjacent = GetAdjacentAir_Directional(Position);

	if (TileSolver_EdgeTable.Find(EmptyAdjacent))
	{
		Grid_Visual[Position].Rot = TileSolver_EdgeTable[EmptyAdjacent].Rot;
		
	}


}
