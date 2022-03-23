// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_Landmark_Landscape.h"
#include "Math/UnrealMathUtility.h"
#include "WorldSeed/public/WT_GeneratorCore.h"
#include "WorldSeed/public/WT_Resources.h"

void AWT_Landmark_Landscape::SetSeed(int In)
{
	SeedData = FRandomStream(In);

	
}

void AWT_Landmark_Landscape::RandomSeed()
{
	SeedData.GenerateNewSeed();
}

void AWT_Landmark_Landscape::ApplyLandmark(AWT_GeneratorCore* Generator)
{
	for (int x = 0; x < Generator->GetGridScale().X; x++)
	{
		for (int y = 0; y < Generator->GetGridScale().Y; y++)
		{
			FVector2D Position = FVector2D(x * TileScale, y * TileScale);
			float NoiseValue = FMath::PerlinNoise2D(Position);

			int Height = (Generator->GetGridScale().Z / 2) * NoiseValue;
			for (int z = 0; z < Generator->GetGridScale().Z; z++)
			{
				if (Height >= z) Generator->SetCellState(FVector(x, y, z), true);
				else Generator->SetCellState(FVector(x, y, z), false);

			}
		}
	}

}
