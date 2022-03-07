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

USTRUCT(BlueprintType)
struct FTileSolverData
{
	GENERATED_BODY()



public:


	FTileSolverData()
	{
		
		StackID = EWT_StackID::ID_Bottom;
		
		Rot = 0;
	}
	UPROPERTY(EditAnywhere)
		float Rot;
	UPROPERTY(EditAnywhere)
		EWT_StackID StackID;
	


};





UCLASS(hidecategories = (Rendering, Replication, Collision, Input, Actor, LOD, Cooking))
class WORLDSEED_API AWT_GeneratorCore : public AActor
{
	GENERATED_BODY()



		


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

	bool IsValidCoordinate(FVector Pos)
	{
		return (Pos.X > 0 && Pos.Y > 0 && Pos.Z > 0) && (Pos.X < GridScale.X&& Pos.Y < GridScale.Y&& Pos.Z < GridScale.Z);
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


	FVector GetAdjacentEmpty_Directional(FVector Position);

	FVector GetAdjacentAir_Directional(FVector Position);

	FVector GetAdjacent_Directional(FVector Position, EWT_GeomID GeomType);

	FVector GetEdgeAdjacent_Directional(FVector Position);

	bool IsTileType(FVector Position, EWT_SpaceID ID)
	{
		return (IsValidCoordinate(Position) && Grid_Structure[Position] == ID);
	}



	void AssignVisual_Edge(FVector Position);
	void AssignVisual_Corner();

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


	UPROPERTY(EditAnywhere)
	TMap<FVector, class AWT_WorldChunk*> ChunkList;


	bool IsEdge(FVector Pos);
	bool IsFloor(FVector Pos);


	UPROPERTY();
	FVector Stored_GridScale;
	UPROPERTY();
	FVector2D Stored_ChunkScale;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileSolver");
	TMap<FVector, FTileSolverData> TileSolver_EdgeTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileSolver");
	TMap<FVector, FTileSolverData> TileSolver_CornerTable;


};




