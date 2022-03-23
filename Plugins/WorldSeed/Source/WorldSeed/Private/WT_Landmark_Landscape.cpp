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
			
			for (int z = 0; z < Generator->GetGridScale().Z; z++)
			{
				FVector Position = FVector( Offset.X + ((float)x / Frequency.X), Offset.Y + ((float)y / Frequency.Y), Offset.Z + ((float)z / Frequency.Z));
				float NoiseValue = FMath::PerlinNoise3D(Position);
				UE_LOG(LogTemp, Warning, TEXT("Position: : [%f | %f | %f], NoiseValue: %f"), Position.X, Position.Y, Position.Z, NoiseValue);
				if (NoiseValue <= HeightCap) Generator->SetCellState(FVector(x, y, z), true);
				else Generator->SetCellState(FVector(x, y, z), false);


				

			}
		}
	}

}
