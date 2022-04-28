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
	FTileSolverData(float Rotation)
	{
		Rot = Rotation;
		StackID = EWT_StackID::ID_Bottom;
	}
	UPROPERTY(EditAnywhere)
		float Rot;
	UPROPERTY(EditAnywhere)
		EWT_StackID StackID;
	


};

USTRUCT(BlueprintType)
struct FLandmarkChannel
{
	GENERATED_BODY()
		UPROPERTY()
		TArray<class AWT_Landmark_Base*> SubLandmarks;
	UPROPERTY()
		TArray<class AWT_Landmark_Base*> AddLandmarks;
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
		else return true;
	}
	void SetCellState(FVector Pos, bool bNewState)
	{
		if (Grid_Data.Find(Pos))
		{
			Grid_Data[Pos] = bNewState;
		}
		
	}
	void AddFloorCell(FVector Pos)
	{
		Grid_FloorData.Add(Pos, true);
	}
	void RemoveFloorCell(FVector Pos)
	{
		if (Grid_FloorData.Find(Pos))
		{
			Grid_FloorData.Remove(Pos);
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


	EWT_SpaceID GetStructureData(FVector Pos)
	{
	
		if (Grid_Structure.Find(Pos))
		{
			return Grid_Structure[Pos];
		}
		else return EWT_SpaceID::ID_Null;
	}

	void StoreLandmark(class AWT_Landmark_Base* In);

	void LoadStyleLandmarks(TArray<class AWT_Landmark_Base*> InLandmarks);


	void OrganiseLandmarks();

	
	bool IsEmptyAdjacent(FVector Pos);
	bool IsFloorAdjacent(FVector Pos);
	bool IsWalkable(FVector Pos);

	bool IsCorner(FVector Pos);


	FVector GetAdjacentEmpty_Directional(FVector Position);

	FVector GetAdjacentAir_Directional(FVector Position);

	FVector GetAdjacent_Directional(FVector Position, EWT_GeomID GeomType);

	FVector GetEdgeAdjacent_Directional(FVector Position);

	bool IsTileType(FVector Position, EWT_SpaceID ID)
	{
		return (IsValidCoordinate(Position) && Grid_Structure[Position] == ID);
	}

	
	FVector GetGridScale() { return GridScale; }


	void AssignVisual_Edge(FVector Position);
	void AssignVisual_Corner(FVector Pos);
	void AssignLayer_Edge(FVector Pos);

protected:


	UPROPERTY(EditAnywhere, Category = "Generation Style")
		TSubclassOf<class UWT_GenerationStyle> GenerationStyle;

	class UWT_GenerationStyle* GenerationStyle_Object;

	UFUNCTION(CallInEditor, Category = "Generation Style")
	void RebuildGrid() 
	{
		BuildGrid();
	}


	UFUNCTION(CallInEditor, Category = "Generation Style")
		void GenerateLevel();


	void Reset();

	void GenerateGeometryMap();
	void GenerateFloorMap();


	void LoadTileSolverTable();




	void AddChunk(FVector Position);

	void CreateLandmark();


	

	UPROPERTY(EditAnywhere, Category = "Grid Setup")
	FVector GridScale;
	UPROPERTY(EditAnywhere, Category = "Grid Setup")
	FVector2D ChunkScale;
	UPROPERTY(EditAnywhere, Category = "Grid Setup")
	int FillHeight;


	



	UPROPERTY()
	TMap<FVector, bool> Grid_Data;
	UPROPERTY()
	TMap<FVector, bool> Grid_FloorData;
	UPROPERTY()
	TMap<FVector, EWT_SpaceID> Grid_Structure;
	UPROPERTY()
	TMap<FVector, FGridVisual> Grid_Visual;

	UPROPERTY()
	TArray<FLandmarkChannel> LandmarkChannels;

	
	UPROPERTY()
	TArray<class AWT_Landmark_Base*> LandmarkList;

	UPROPERTY()
	TArray<class AWT_Landmark_Base*> StyleLandmarks;


	UPROPERTY(EditAnywhere)
	TMap<FVector, class AWT_WorldChunk*> ChunkList;


	bool IsEdge(FVector Pos);
	bool IsFloor(FVector Pos);


	UPROPERTY();
	FVector Stored_GridScale;
	UPROPERTY();
	FVector2D Stored_ChunkScale;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileSolver");
	TMap<FVector2D, FTileSolverData> TileSolver_EdgeTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileSolver");
	TMap<FVector2D, FTileSolverData> TileSolver_InnerCornerTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileSolver");
	TMap<FVector2D, FTileSolverData> TileSolver_OuterCornerTable;


};




