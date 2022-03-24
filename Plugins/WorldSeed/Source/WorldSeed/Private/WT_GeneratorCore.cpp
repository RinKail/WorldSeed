// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GeneratorCore.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WorldSeed/Public/WT_Landmark_Base.h"
#include "WorldSeed/Public/WT_GenerationStyle.h"
#include "Kismet/GameplayStatics.h"

AWT_GeneratorCore::AWT_GeneratorCore()
{
	LoadTileSolverTable();
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

		OrganiseLandmarks();

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

		for (int i = 0; i < LandmarkChannels.Num(); i++)
		{
			

			for (int y = 0; y < LandmarkChannels[i].SubLandmarks.Num(); y++)
			{
				if (IsValid(LandmarkChannels[i].SubLandmarks[y])) LandmarkChannels[i].SubLandmarks[y]->ApplyLandmark(this);
			}

			for (int x = 0; x < LandmarkChannels[i].AddLandmarks.Num(); x++)
			{
				if (IsValid(LandmarkChannels[i].AddLandmarks[x]))LandmarkChannels[i].AddLandmarks[x]->ApplyLandmark(this);
			}
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

void AWT_GeneratorCore::GenerateLevel()
{
	if (!IsValid(GenerationStyle_Object))
	{
		GenerationStyle_Object = NewObject<UWT_GenerationStyle>(this, GenerationStyle);
	}
	if (IsValid(GenerationStyle_Object))
	{
		GenerationStyle_Object->ClearLandmarks();
		GenerationStyle_Object->GenerateStyle(this);
	}

	BuildGrid();



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
	if (Grid_Data.Num() > 0)
	{
		Grid_FloorData.Empty();
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
					if (Grid_FloorData.Find(FVector(x, y, z)))
					{
						Grid_Structure[FVector(x, y, z)] = EWT_SpaceID::ID_Floor;
					}
					else
					{
						Grid_Structure[FVector(x, y, z)] = EWT_SpaceID::ID_Empty;
					}

					
					/*
					* if (IsFloor(FVector(x, y, z)))
					{
						Grid_Structure[FVector(x, y, z)] = EWT_SpaceID::ID_Floor;
					}
					else
					{
						Grid_Structure[FVector(x, y, z)] = EWT_SpaceID::ID_Empty;
					}
					*/
					
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
					Grid_Visual[FVector(x, y, z)].TileID = EWT_GeomID::ID_Raised;
					Grid_Visual[FVector(x, y, z)].StackID = EWT_StackID::ID_Bottom;
					break;
				case EWT_SpaceID::ID_Edge:
					

					AssignLayer_Edge(FVector(x, y, z));
					AssignVisual_Edge(FVector(x,y,z));
				



					break;
				}

			}
		}
	}




}

void AWT_GeneratorCore::LoadTileSolverTable()
{

	TileSolver_EdgeTable.Add(FVector2D(1,0), FTileSolverData(90));
	TileSolver_EdgeTable.Add(FVector2D(-1, 0), FTileSolverData(-90));
	TileSolver_EdgeTable.Add(FVector2D(0, 1), FTileSolverData(180));
	TileSolver_EdgeTable.Add(FVector2D(0, -1), FTileSolverData(0));

	TileSolver_OuterCornerTable.Add(FVector2D(1,1), FTileSolverData(-90));
	TileSolver_OuterCornerTable.Add(FVector2D(-1, -1), FTileSolverData(90));
	TileSolver_OuterCornerTable.Add(FVector2D(1, -1), FTileSolverData(180));
	TileSolver_OuterCornerTable.Add(FVector2D(-1, 1), FTileSolverData(0));

	TileSolver_InnerCornerTable .Add(FVector2D(1, 1), FTileSolverData(90));
	TileSolver_InnerCornerTable.Add(FVector2D(-1, -1), FTileSolverData(-90));
	TileSolver_InnerCornerTable.Add(FVector2D(1, -1), FTileSolverData(0));
	TileSolver_InnerCornerTable.Add(FVector2D(-1, 1), FTileSolverData(180));
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
	LandmarkList.Add(In);

}

void AWT_GeneratorCore::LoadStyleLandmarks(TArray<class AWT_Landmark_Base*> InLandmarks)
{
	for (int i = 0; i < StyleLandmarks.Num(); i++)
	{
		if (IsValid(StyleLandmarks[i])) StyleLandmarks[i]->Destroy();
	}
	StyleLandmarks.Empty();
	StyleLandmarks = InLandmarks;
	UE_LOG(LogTemp, Warning, TEXT("In Landmark Total: %d"), InLandmarks.Num());
	UE_LOG(LogTemp, Warning, TEXT("Style landmark Total: %d"), StyleLandmarks.Num());
}

void AWT_GeneratorCore::OrganiseLandmarks()
{
	LandmarkChannels.Empty();
	LandmarkChannels.SetNum(5);

	for (int i = 0; i < StyleLandmarks.Num(); i++)
	{
		if (IsValid(StyleLandmarks[i]))
		{
			int InChannel = StyleLandmarks[i]->GetChannel();

			if (InChannel > LandmarkChannels.Num())
			{
				LandmarkChannels.SetNum(InChannel, false);

			}

			if (StyleLandmarks[i]->IsLandmarkAdditive())
			{
				LandmarkChannels[InChannel].AddLandmarks.Add(StyleLandmarks[i]);
			}
			else
			{
				LandmarkChannels[InChannel].SubLandmarks.Add(StyleLandmarks[i]);
			}
		}
	}

	for (int i = 0; i < LandmarkList.Num(); i++)
	{
		if (IsValid(LandmarkList[i]))
		{
			int InChannel = LandmarkList[i]->GetChannel();

			if (InChannel > LandmarkChannels.Num())
			{
				LandmarkChannels.SetNum(InChannel, false);

			}

			if (LandmarkList[i]->IsLandmarkAdditive())
			{
				LandmarkChannels[InChannel].AddLandmarks.Add(LandmarkList[i]);
			}
			else
			{
				LandmarkChannels[InChannel].SubLandmarks.Add(LandmarkList[i]);
			}
		}
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

bool AWT_GeneratorCore::IsFloorAdjacent(FVector Pos)
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

	PosList.Add(FVector(0, 0, 1));
	PosList.Add(FVector(0, 0, -1));

	for (int i = 0; i < PosList.Num(); i++)
	{
		if (GetStructureData(PosList[i]) == EWT_SpaceID::ID_Floor) return true;
	}
	return false;

	
}

bool AWT_GeneratorCore::IsWalkable(FVector Pos)
{
	EWT_SpaceID Space = GetStructureData(Pos);
	return (Space == EWT_SpaceID::ID_Empty || Space == EWT_SpaceID::ID_Floor);
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

bool AWT_GeneratorCore::IsCorner(FVector Pos)
{
	if (IsValidCoordinate(Pos))
	{
		if (IsTileType(Pos + FVector(0, 1, 0), EWT_SpaceID::ID_Edge) && IsTileType(Pos + FVector(1, 0, 0), EWT_SpaceID::ID_Edge))
		{
			if (IsTileType(Pos + FVector(1, 1, 0), EWT_SpaceID::ID_Floor))
			{

			}
			else if (IsTileType(Pos + FVector(-1, -1, 0), EWT_SpaceID::ID_Floor))
			{

			}
		}


	}



	return false;
}

void AWT_GeneratorCore::AssignVisual_Edge(FVector Position)
{

	FVector Temp = GetAdjacentAir_Directional(Position);
	FVector2D EmptyAdjacent = FVector2D(Temp.X, Temp.Y);

	if (TileSolver_EdgeTable.Find(EmptyAdjacent))
	{
		Grid_Visual[Position].Rot = TileSolver_EdgeTable[EmptyAdjacent].Rot;
		Grid_Visual[Position].TileID = EWT_GeomID::ID_Wall;
		
	}
	else
	{
		AssignVisual_Corner(Position);
	}


}

void AWT_GeneratorCore::AssignVisual_Corner(FVector Pos)
{
	
	if (IsValidCoordinate(Pos))
	{
		bool bInnerCorner = false;
		FVector Temp = GetEdgeAdjacent_Directional(Pos);

		FVector2D Adjacent = FVector2D(Temp.X, Temp.Y);

		if (IsTileType(Pos + FVector(0, 1, 0), EWT_SpaceID::ID_Edge) && IsTileType(Pos + FVector(1, 0, 0), EWT_SpaceID::ID_Edge))
		{
			if (IsWalkable(Pos + FVector(1, 1, 0)))
			{
				bInnerCorner = true;
			}
			
		}
		else if (IsTileType(Pos + FVector(0, 1, 0), EWT_SpaceID::ID_Edge) && IsTileType(Pos + FVector(-1, 0, 0), EWT_SpaceID::ID_Edge))
		{
			if (IsWalkable(Pos + FVector(-1, 1, 0)))
			{
				bInnerCorner = true;
			}
			
		}
		else if (IsTileType(Pos + FVector(0, -1, 0), EWT_SpaceID::ID_Edge) && IsTileType(Pos + FVector(1, 0, 0), EWT_SpaceID::ID_Edge))
		{
			if (IsWalkable(Pos + FVector(1, -1, 0)))
			{
				bInnerCorner = true;
			}
			
		}
		else if (IsTileType(Pos + FVector(0, -1, 0), EWT_SpaceID::ID_Edge) && IsTileType(Pos + FVector(-1, 0, 0), EWT_SpaceID::ID_Edge))
		{
			if (IsWalkable(Pos + FVector(-1, -1, 0)))
			{
				bInnerCorner = true;

			}
			
		}


		if (bInnerCorner)
		{
			if (TileSolver_OuterCornerTable.Find(Adjacent))
			{
				Grid_Visual[Pos].Rot = TileSolver_OuterCornerTable[Adjacent].Rot;
				Grid_Visual[Pos].TileID = EWT_GeomID::ID_OuterCorner;


				EWT_SpaceID TopID = GetStructureData(Pos + FVector(0, 0, 1));
				EWT_StackID StackID = Grid_Visual[Pos].StackID;

				if (TopID == EWT_SpaceID::ID_Edge && StackID == EWT_StackID::ID_Mid) 	Grid_Visual[Pos].StackID = EWT_StackID::ID_Top;
			}
		}
		else
		{
			if (TileSolver_InnerCornerTable.Find(Adjacent))
			{
				Grid_Visual[Pos].Rot = TileSolver_InnerCornerTable[Adjacent].Rot;
				Grid_Visual[Pos].TileID = EWT_GeomID::ID_InnerCorner;

				EWT_SpaceID TopID = GetStructureData(Pos + FVector(0, 0, 1));
				EWT_StackID StackID = Grid_Visual[Pos].StackID;

				if (TopID == EWT_SpaceID::ID_Edge && StackID == EWT_StackID::ID_Mid) 	Grid_Visual[Pos].StackID = EWT_StackID::ID_Top;

			}
		}

		
		
	}
}



void AWT_GeneratorCore::AssignLayer_Edge(FVector Pos)
{
	EWT_SpaceID TopID = GetStructureData(Pos + FVector(0, 0, (int)1));
	EWT_SpaceID BotID = GetStructureData(Pos + FVector(0, 0, -1));
	bool bTopFloorAdjacent = IsFloorAdjacent(Pos + FVector(0, 0, 1));
	bool bTopEmptyAdjacent = IsFloorAdjacent(Pos + FVector(0, 0, -1));
	bool bFloorAdjacent = IsFloorAdjacent(Pos);
	bool bEmptyAdjacent = IsEmptyAdjacent(Pos);

	switch (TopID)
	{
	case EWT_SpaceID::ID_Floor:

		Grid_Visual[Pos].StackID = EWT_StackID::ID_TopWalkable;

		break;
	case EWT_SpaceID::ID_Empty:
		/*
		* if (bTopFloorAdjacent)
		{
			
		}
		else if (bTopEmptyAdjacent)
		{
			Grid_Visual[Pos].StackID = EWT_StackID::ID_Top;
		}
		else if (bFloorAdjacent)
		{
			//Grid_Visual[Pos].StackID = EWT_StackID::ID_Single;
		}
		*/

		Grid_Visual[Pos].StackID = EWT_StackID::ID_Top;


		break;
	case EWT_SpaceID::ID_Edge:
		if (BotID != EWT_SpaceID::ID_Edge)
		{
			Grid_Visual[Pos].StackID = EWT_StackID::ID_Bottom;
			
		}
		else
		{
			Grid_Visual[Pos].StackID = EWT_StackID::ID_Mid;
		}
		break;
		
	}


	
	
}
