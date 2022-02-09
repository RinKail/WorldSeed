// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WT_Resources.h"
#include "WT_GeneratorCore.generated.h"


USTRUCT(BlueprintType)
struct FGridVisual
{
	GENERATED_BODY()



public:


	FGridVisual()
	{
		TileID = EWT_GeomID::ID_Block;
		StackID = EWT_StackID::ID_Bottom;
		Channel = 0;
		Rot = 0;
	}
	UPROPERTY()
		float Rot;
	UPROPERTY()
		EWT_GeomID TileID;
	UPROPERTY()
		EWT_StackID StackID;
	UPROPERTY()
		int Channel;


};





/**
 * 
 */


UCLASS(hidecategories = (Rendering, Replication, Collision, Input, Actor, LOD, Cooking))
class WORLDSEED_API AWT_GeneratorCore : public AActor
{
	GENERATED_BODY()



		


public: 


	AWT_GeneratorCore();


	void BuildGrid();


	

	bool GetCellState(FVector Pos)
	{
		if (Grid_Data.Find(Pos))
		{
			return Grid_Data[Pos];
		}
		else return false;
	}
	void SetCellState(FVector Pos, bool bNewState)
	{
		if (Grid_Data.Find(Pos))
		{
			Grid_Data[Pos] = bNewState;
		}
		
	}


	FGridVisual GetTileData(FVector Pos)
	{
		FGridVisual Return;
		if (Grid_Visual.Find(Pos))
		{
			return Grid_Visual[Pos];
		}
		else return Return;
	}

	void StoreLandmark(class AWT_Landmark_Base* In) { SubLandmarks.Add(In); }

	
	bool IsEmptyAdjacent(FVector Pos);

protected:

	void GenerateGeometryMap();
	void GenerateFloorMap();


	void AddChunk(FVector Position);

	UPROPERTY(EditAnywhere, Category = "Grid Setup")
	FVector GridScale;
	UPROPERTY(EditAnywhere, Category = "Grid Setup")
	FVector ChunkScale;
	UPROPERTY(EditAnywhere, Category = "Grid Setup")
	int FillHeight;



	UPROPERTY()
	TMap<FVector, bool> Grid_Data;
	UPROPERTY()
	TMap<FVector, EWT_SpaceID> Grid_Structure;
	UPROPERTY()
	TMap<FVector, FGridVisual> Grid_Visual;


	UPROPERTY()
	TArray<class AWT_Landmark_Base*> SubLandmarks;
	UPROPERTY()
	TArray<class AWT_Landmark_Base*> AddLandmarks;
	UPROPERTY()
	TMap<FVector, class AWT_WorldChunk*> ChunkList;


	bool IsEdge(FVector Pos);
	bool IsFloor(FVector Pos);





};




