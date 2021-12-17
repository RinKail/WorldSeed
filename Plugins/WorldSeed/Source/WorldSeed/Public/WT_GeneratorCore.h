// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WT_GeneratorCore.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCellData
{
	GENERATED_BODY()
	
	EWT_TileID Type;
	int Channel;
};


UCLASS()
class WORLDSEED_API AWT_GeneratorCore : public AActor
{
	GENERATED_BODY()
	


public: 

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


	FTileRenderData GetTile(FVector Position);


protected: 

	


	int ChunkSize;



	TMap<FVector,class AWT_WorldChunk*> ChunkList;
	TArray<class AWT_Landmark_Base*> StoredLandmarks;
	//TArray<FCachedLandmarks> StoredLandmarkData;



	FVector ChunkScale;
	FVector GridScale;


	TMap<FVector, bool> GridData;
	TMap<FVector, FCellData> Grid_AppearanceData;


	//Designed for open and more natural enviroments. Generates from the bottom and enviroments work on the basis that the initial state is a empty floor.
	void ProcessGrid_LayerDependant();
	//Designed for internal, man-made structures. Layers work independently and ignore tile's rendered above and below.
	void ProcessGrid_LayerIndependant();




private: 

	EWT_TileDirection CalculateTile_InnerCorner(FVector Position);
	EWT_TileDirection CalculateTile_OuterCorner(FVector Position);
	EWT_TileDirection CalculateTile_Edge(FVector Position);



	bool IsAdjacentTileOfType(FVector Positiom, EWT_TileID ID);


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


