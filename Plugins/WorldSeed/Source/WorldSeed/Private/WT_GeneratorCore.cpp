// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GeneratorCore.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WorldSeed/Public/WT_Landmark_Base.h"

void AWT_GeneratorCore::AddChunk(FVector2D Coordinate, AWT_WorldChunk* Chunk)
{
	ChunkList.Add(Coordinate, Chunk);

}

void AWT_GeneratorCore::UpdateChunks()
{




}


void AWT_GeneratorCore::BuildEnviroment()
{
	for (int x = 0; x < GridScale.X; x++)
	{
		for (int y = 0; y < GridScale.Y; y++)
		{
			GridData[FVector2D(x, y)] = false;
		}
	}

	for (int i = 0; i < StoredLandmarks.Num(); i++)
	{
		StoredLandmarks[i]->ApplyLandmark(this);
	}

	for (int x = 0; x < GridScale.X; x++)
	{
		for (int y = 0; y < GridScale.Y; y++)
		{
			ChunkList[FVector2D(x, y)]->Generate(this, FVector2D(x,y));
		}
	}
	
}
