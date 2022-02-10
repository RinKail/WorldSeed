// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "WT_Resources.h"
#include "Engine/DataTable.h"
#include "WT_WorldChunk.generated.h"


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
	UPROPERTY(EditAnywhere)
		float Rot;
	UPROPERTY(EditAnywhere)
		EWT_GeomID TileID;
	UPROPERTY(EditAnywhere)
		EWT_StackID StackID;
	UPROPERTY(EditAnywhere)
		int Channel;


};



USTRUCT(BlueprintType)
struct FTile_AssetTypes
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		 UStaticMesh* Bottom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		 UStaticMesh* Middle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		 UStaticMesh* Top;
};


USTRUCT(BlueprintType)
struct FTile_TableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTile_AssetTypes Raised;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTile_AssetTypes Wall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTile_AssetTypes WallCorner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTile_AssetTypes InnerCorner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTile_AssetTypes OuterCorner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTile_AssetTypes Block;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTile_AssetTypes ThinWall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTile_AssetTypes ThinCorner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTile_AssetTypes ThinWallEnd;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTile_AssetTypes XConnector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTile_AssetTypes TConnector;

};


USTRUCT(BlueprintType)
struct FTile_ComponentData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInstancedStaticMeshComponent* BottomComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInstancedStaticMeshComponent* MiddleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInstancedStaticMeshComponent* TopComponent;
};



USTRUCT(BlueprintType)
struct FInstanceStack
{
	GENERATED_BODY()

		TArray<FTile_ComponentData> ComponentList;
};


USTRUCT(BlueprintType)
struct FTileKey
{
	GENERATED_BODY()
public:
		int Index;
	class UInstancedStaticMeshComponent* Comp;
	
};





UCLASS()
class WORLDSEED_API AWT_WorldChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWT_WorldChunk();


	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;


	void GenerateChunk(class AWT_GeneratorCore* Gen, FVector2D ChunkScale, int WorldHeight);

	//void UpdateChunk(class AWT_GeneratorCore* Generator);


	void InitialiseChunk();

protected:

	UPROPERTY()
	class UDataTable* DataTable_Geometry;

	class USceneComponent* SceneRoot;

	


	
	TMap<EWT_GeomID, FTile_ComponentData> ComponentList;


	UPROPERTY(EditAnywhere)
	TMap<FVector,  FGridVisual> StoredData;
	UPROPERTY(EditAnywhere)
	FVector GridSize;



	
	TMap<FVector, FTileKey> TileKeys;

	void UpdateTile(FVector Position,  FGridVisual Data);
	
	void InitialiseTileData(EWT_GeomID TileID, FTile_AssetTypes Asset, FString CompName);





	void InitialiseMeshComponents();




};
