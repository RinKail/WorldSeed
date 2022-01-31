// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WT_GeneratorCore.generated.h"

/**
 * 
 */
UCLASS()
class WORLDSEED_API AWT_GeneratorCore : public AActor
{
	GENERATED_BODY()



		


public: 


	AWT_GeneratorCore();

	void SetChunkSize(int NewSize) { ChunkSize = NewSize; }
	int GetChunkSize() { return ChunkSize; }


	class AWT_WorldChunk* GetChunk(FVector2D Coordinate) { return ChunkList[Coordinate]; }

	void AddChunk(FVector2D Coordinate, class AWT_WorldChunk* Chunk);

	void ClearChunkList() {
		for (int x = 0; x < ChunkSize; x++)
		{
			for (int y = 0; y < ChunkSize; y++)
			{
				ChunkList[FVector2D(x, y)]->Destroy();
			}
		}
		ChunkList.Empty();
	}


	void UpdateChunks();

	void StoreChunks();


	void SetGridScale(FVector2D Grid, FVector2D Chunk) { ChunkScale = Chunk; GridScale = Grid; }

	bool GetGridData(FVector2D Position) { return GridData[Position]; }

	void ApplyToGrid(FVector2D Position, bool bIsOccupied) { GridData[Position] = bIsOccupied; }

	void BuildEnviroment(int GridX, int GridY, int ChunkX, int ChunkY);


	void StoreLandmark(class AWT_Landmark_Base* Landmark) { StoredLandmarks.Add(Landmark); }


protected: 

	


	int ChunkSize;



	TMap<FVector2D,class AWT_WorldChunk*> ChunkList;
	TArray<class AWT_Landmark_Base*> StoredLandmarks;
	//TArray<FCachedLandmarks> StoredLandmarkData;



	FVector2D ChunkScale;
	FVector2D GridScale;


	TMap<FVector2D, bool> GridData;
	//TMap<FVector2D, EWT_TileTypes> Grid_AppearanceData;


};


/*

STRUCT() struct FCachedLandmarks
{
	GENERATED_BODY()

		FLandmarkData Data;

	EWT_LandMarkType Type;
	
		
};


UENUM(BlueprintType)
enum class EWT_TileTypes : uint8
{
	TT_Empty		UMETA(DisplayName = "None"),
	TT_Edge		UMETA(DisplayName = "None"),
	TT_EdgeCorner		UMETA(DisplayName = "None"),
	TT_Floor		UMETA(DisplayName = "None"),
	TT_FloorCorner		UMETA(DisplayName = "None"),
	

};

*/


