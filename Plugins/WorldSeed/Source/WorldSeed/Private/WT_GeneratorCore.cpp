// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GeneratorCore.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WorldSeed/Public/WT_Landmark_Base.h"

void AWT_GeneratorCore::AddChunk(FVector Coordinate, AWT_WorldChunk* Chunk)
{
	ChunkList.Add(Coordinate, Chunk);
	ChunkSize = 10;
	
}

void AWT_GeneratorCore::UpdateChunks()
{




}


void AWT_GeneratorCore::BuildEnviroment(int GridX, int GridY, int ChunkX, int ChunkY)
{
	GridScale = FVector(GridX, GridY, 0);
	ChunkScale = FVector(ChunkX, ChunkY, 0);


	for (int z = 0; z < GridX * ChunkX; z++)
	{
		for (int x = 0; x < GridY * ChunkY; x++)
		{
			for (int y = 0; y < GridY * ChunkY; y++)
			{
				GridData.Add(FVector(x, y, z), false);

			}
			
		}
	}

	for (int i = 0; i < StoredLandmarks.Num(); i++)
	{
		StoredLandmarks[i]->ApplyLandmark(this);
	}

	for (int x = 0; x < GridX; x++)
	{
		for (int y = 0; y < GridY; y++)
		{
			ChunkList[FVector2D(x, y)]->Generate(this, FVector2D(x,y));
		}
	}
	
}

FTileRenderData AWT_GeneratorCore::GetTile(FVector Position)
{
	FTileRenderData TempData;
	FCellData Cell = Grid_AppearanceData[Position];

	TempData.Type = Cell.Type;
	
	if (Grid_AppearanceData[Position + FVector(0, 0, 1)].Type != EWT_TileID::TI_Empty)
		TempData.Direction = EWT_TileDirection::TD_None;
	else
	{
		switch (Cell.Type)
		{
		case EWT_TileID::TI_Edge:
			TempData.Direction = CalculateTile_Edge(Position);

			break;
		case EWT_TileID::TI_InnerCorner:
			TempData.Direction = CalculateTile_OuterCorner(Position);
			break;
		case EWT_TileID::TI_OuterCorner:
			TempData.Direction = CalculateTile_OuterCorner(Position);
			break;

		}
	}

	return FTileRenderData();
}

void AWT_GeneratorCore::ProcessGrid_LayerDependant()
{
	for (int z = 0; z < GridScale.X * ChunkScale.X; z++)
	{
		for (int x = 0; x < GridScale.Y * ChunkScale.Y; x++)
		{
			for (int y = 0; y < GridScale.Z * ChunkScale.Z; y++)
			{
				FCellData TempCell = FCellData();
				if (GridData[FVector(x, y, z)])
				{
					TempCell.Type = EWT_TileID::TI_Floor;

					

					

				}
				else
				{
					TempCell.Type = EWT_TileID::TI_Raised;
				}


				Grid_AppearanceData.Add(FVector(x, y, z), TempCell);

			}

		}
	}
	for (int z = 0; z < GridScale.X * ChunkScale.X; z++)
	{
		for (int x = 0; x < GridScale.Y * ChunkScale.Y; x++)
		{
			for (int y = 0; y < GridScale.Z * ChunkScale.Z; y++)
			{
				if (Grid_AppearanceData[FVector(x, y, z)].Type == EWT_TileID::TI_Floor)
				{
					if (IsAdjacentTileOfType(FVector(x, y, z) + FVector(1, 0, 0), EWT_TileID::TI_Raised))
					{
						Grid_AppearanceData[FVector(x, y, z) + FVector(1, 0, 0)].Type = EWT_TileID::TI_Edge;
					}
					if (
						IsAdjacentTileOfType(FVector(x, y, z) + FVector(-1, 0, 0), EWT_TileID::TI_Raised) ||
						IsAdjacentTileOfType(FVector(x, y, z) + FVector(0, 1, 0), EWT_TileID::TI_Raised) ||
						IsAdjacentTileOfType(FVector(x, y, z) + FVector(0, -1, 0), EWT_TileID::TI_Raised))
					{
						Grid_AppearanceData[FVector(x, y, z) + FVector(1, 0, 0)].Type = EWT_TileID::TI_Edge;
					}
					if (IsAdjacentTileOfType(FVector(x, y, z) + FVector(1, 0, 0), EWT_TileID::TI_Raised) ||
						IsAdjacentTileOfType(FVector(x, y, z) + FVector(-1, 0, 0), EWT_TileID::TI_Raised) ||
						IsAdjacentTileOfType(FVector(x, y, z) + FVector(0, 1, 0), EWT_TileID::TI_Raised) ||
						IsAdjacentTileOfType(FVector(x, y, z) + FVector(0, -1, 0), EWT_TileID::TI_Raised))
					{
						Grid_AppearanceData[FVector(x, y, z) + FVector(1, 0, 0)].Type = EWT_TileID::TI_Edge;
					}
					if (IsAdjacentTileOfType(FVector(x, y, z) + FVector(1, 0, 0), EWT_TileID::TI_Raised) ||
						IsAdjacentTileOfType(FVector(x, y, z) + FVector(-1, 0, 0), EWT_TileID::TI_Raised) ||
						IsAdjacentTileOfType(FVector(x, y, z) + FVector(0, 1, 0), EWT_TileID::TI_Raised) ||
						IsAdjacentTileOfType(FVector(x, y, z) + FVector(0, -1, 0), EWT_TileID::TI_Raised))
					{
						Grid_AppearanceData[FVector(x, y, z) + FVector(1, 0, 0)].Type = EWT_TileID::TI_Edge;
					}
				}

			}

		}
	}






	
}

EWT_TileDirection AWT_GeneratorCore::CalculateTile_Edge(FVector Position)
{
	


	FString Result;
	if (IsAdjacentTileOfType(Position + FVector(1, 0, 0), EWT_TileID::TI_Floor))
		Result += '1';
	else Result += '0';
	if (IsAdjacentTileOfType(Position + FVector(0, 1, 0), EWT_TileID::TI_Floor))
		Result += '1';
	else Result += '0';
	if (IsAdjacentTileOfType(Position + FVector(-1, 0, 0), EWT_TileID::TI_Floor))
		Result += '1';
	else Result += '0';
	if (IsAdjacentTileOfType(Position + FVector(0, -1, 0), EWT_TileID::TI_Floor))
		Result += '1';
	else Result += '0';

	int32 IntResult = FCString::Atoi(*Result);

	switch (IntResult)
	{
	case 1000:
		return EWT_TileDirection::TD_Up;
		break;
	case 0100: 
		return EWT_TileDirection::TD_Right;
		break;
	case 0010:
		return EWT_TileDirection::TD_Down;
		break;
	case 0001:
		return EWT_TileDirection::TD_Left;
		break;
	}





	return EWT_TileDirection::TD_None;
}

bool AWT_GeneratorCore::IsAdjacentTileOfType(FVector Position, EWT_TileID ID)
{

	return Grid_AppearanceData[Position].Type == ID;
}
