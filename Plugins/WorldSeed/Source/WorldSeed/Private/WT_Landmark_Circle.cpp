// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_Landmark_Circle.h"
#include "WorldSeed/public/WT_GeneratorCore.h"

void AWT_Landmark_Circle::ApplyLandmark(class AWT_GeneratorCore* Generator)
{
	int Diameter = 3 - (2 * Radius);

	int X = 0;
	int Y = Radius;

	FVector Pos = FVector((int)GetActorLocation().X / TileScale, (int)GetActorLocation().Y / TileScale, (int)GetActorLocation().Z / TileScale);

	while (X <= Y)
	{
		for (int Z = 0; Z < Height; Z++)
		{
			Generator->SetCellState(FVector(Pos.X + X, Pos.Y + Y, Pos.Z + Z), bAdditive);
			Generator->SetCellState(FVector(Pos.X + X, Pos.Y - Y, Pos.Z + Z), bAdditive);
			Generator->SetCellState(FVector(Pos.X - X, Pos.Y + Y, Pos.Z + Z), bAdditive);
			Generator->SetCellState(FVector(Pos.X - X, Pos.Y - Y, Pos.Z + Z), bAdditive);

			Generator->SetCellState(FVector(Pos.X + Y, Pos.Y + X, Pos.Z + Z), bAdditive);
			Generator->SetCellState(FVector(Pos.X + Y, Pos.Y - X, Pos.Z + Z), bAdditive);
			Generator->SetCellState(FVector(Pos.X - Y, Pos.Y + X, Pos.Z + Z), bAdditive);
			Generator->SetCellState(FVector(Pos.X - Y, Pos.Y - X, Pos.Z + Z), bAdditive);
			

		}
		if (Diameter < 0)
		{
			Diameter = Diameter + (4 * X) + 6;

		}
		else
		{
			Diameter = Diameter + 4 * (X - Y) + 10;
			Y--;
		}
		X++;

	}


	for (int Z = 0; Z < Height; Z++)
	{
		FloodFill(Generator, Pos + FVector(0, 0, (int)Z));


		if (bIsWalkable)
		{
			if (bAdditive)
			{
				if (Z >= Height - 1)
				{
					FloodFill_Floor(Generator, Pos + FVector(0, 0, (int)Z));
				}
			}
			else
			{
				if (Z == 0)
				{
					FloodFill_Floor(Generator, Pos + FVector(0, 0, (int)Z));
				}
			}
		}


		


	}



}

void AWT_Landmark_Circle::FloodFill(AWT_GeneratorCore* Generator, FVector Position)
{
	if (Generator->GetCellState(Position) != bAdditive)
	{
		Generator->SetCellState(Position, bAdditive);
		FloodFill(Generator, (Position + FVector((int)1, (int)0, (int)0)));
		FloodFill(Generator, (Position + FVector((int)-1, (int)0, (int)0)));
		FloodFill(Generator, (Position + FVector((int)0, (int)1, (int)0)));
		FloodFill(Generator, (Position + FVector((int)0, (int)-1, (int)0)));
	}



}

void AWT_Landmark_Circle::FloodFill_Floor(AWT_GeneratorCore* Generator, FVector Position)
{
	if (Generator->GetCellState(Position) != bAdditive)
	{
		Generator->AddFloorCell(Position);
		FloodFill(Generator, (Position + FVector((int)1, (int)0, (int)0)));
		FloodFill(Generator, (Position + FVector((int)-1, (int)0, (int)0)));
		FloodFill(Generator, (Position + FVector((int)0, (int)1, (int)0)));
		FloodFill(Generator, (Position + FVector((int)0, (int)-1, (int)0)));
	}
}
