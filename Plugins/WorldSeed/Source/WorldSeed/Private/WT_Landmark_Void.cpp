// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSeed/Public/WT_Landmark_Void.h"
#include "WorldSeed/Public/WT_Resources.h"
#include "WorldSeed/Public/WT_GeneratorCore.h"

void AWT_Landmark_Void::ApplyLandmark(class AWT_GeneratorCore* Generator)
{
	FVector ConvertedPosition = FVector(0, 0, 0);

	ConvertedPosition.X = (int)(GetActorLocation().X / TileScale);
	ConvertedPosition.Y = (int)(GetActorLocation().Y / TileScale);
	ConvertedPosition.Z = (int)(GetActorLocation().Z / TileScale);


	for (int z = 0; z < LandmarkScale.Z; z++)
	{
		for (int x = 0; x < LandmarkScale.X; x++)
		{
			for (int y = 0; y < LandmarkScale.Y; y++)
			{
				Generator->SetCellRenderState(ConvertedPosition + FVector(x, y, z), true);
			}
		}
	}
}