// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GeneratorCore.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WorldSeed/Public/WT_Landmark_Base.h"

void AWT_GeneratorCore::AddChunk(FVector2D Coordinate, AWT_WorldChunk* Chunk)
{
	ChunkList.Add(Coordinate, Chunk);
	ChunkSize = 10;
	
}

void AWT_GeneratorCore::UpdateChunks()
{




}


void AWT_GeneratorCore::BuildEnviroment(int GridX, int GridY, int ChunkX, int ChunkY)
{
	for (int x = 0; x < GridX * ChunkX; x++)
	{
		for (int y = 0; y < GridY * ChunkY; y++)
		{
			GridData.Add(FVector2D(x,y), false);
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
