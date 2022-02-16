// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WT_Resources.h"
#include "WT_GeneratorCore.generated.h"





/**
 * 
 */


UCLASS(hidecategories = (Rendering, Replication, Collision, Input, Actor, LOD, Cooking))
class WORLDSEED_API AWT_GeneratorCore : public AActor
{
	GENERATED_BODY()



		
public: 

	class USceneComponent* SceneRoot;

	TMap<EWT_GeomID, FTile_ComponentData> ComponentList;

public: 


	AWT_GeneratorCore();


	void BuildGrid();


	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

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

	void StoreLandmark(class AWT_Landmark_Base* In);

	
	bool IsEmptyAdjacent(FVector Pos);

protected:

	void InitialiseTileData(EWT_GeomID TileID, FTile_AssetTypes Asset, FString CompName);


	void UpdateTile(FVector Position, FGridVisual Data);


	void InitialiseMeshComponents();



protected:
	void Reset();

	void GenerateGeometryMap();
	void GenerateFloorMap();


	void AddChunk(FVector Position);

	UPROPERTY(EditAnywhere, Category = "Grid Setup")
	FVector GridScale;
	UPROPERTY(EditAnywhere, Category = "Grid Setup")
	FVector2D ChunkScale;
	UPROPERTY(EditAnywhere, Category = "Grid Setup")
	int FillHeight;

	UPROPERTY(EditAnywhere)
	bool bClear;

	UPROPERTY()
		class UDataTable* DataTable_Geometry;

	UPROPERTY()
	TMap<FVector, bool> Grid_Data;
	UPROPERTY()
	TMap<FVector, EWT_SpaceID> Grid_Structure;
	UPROPERTY()
	TMap<FVector, FGridVisual> Grid_Visual;



	
	TMap<FVector, FTileKey> TileKeys;

	UPROPERTY()
	TArray<class AWT_Landmark_Base*> SubLandmarks;
	UPROPERTY()
	TArray<class AWT_Landmark_Base*> AddLandmarks;


	UPROPERTY(EditAnywhere)
	TMap<FVector, class AWT_WorldChunk*> ChunkList;


	bool IsEdge(FVector Pos);
	bool IsFloor(FVector Pos);


	UPROPERTY();
	FVector Stored_GridScale;
	UPROPERTY();
	FVector2D Stored_ChunkScale;


};




