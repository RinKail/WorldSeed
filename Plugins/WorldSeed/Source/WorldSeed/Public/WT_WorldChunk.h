// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "WT_Resources.h"
#include "WT_WorldChunk.generated.h"



USTRUCT(BlueprintType)
struct FInstanceStack
{
	GENERATED_BODY()
		TArray<class UInstancedStaticMeshComponent*> ComponentList;
};


USTRUCT(BlueprintType)
struct FWT_TileData
{
	GENERATED_BODY()


	FWT_TileData(int ID, int ChannelID, bool bWalkable, EWT_TileID TileID) : InstanceID(ID), Channel(ChannelID), bIsWalkable(bWalkable), MeshID(TileID) {}
	FWT_TileData()
	{
		InstanceID = 0;
		Channel = 0;
		bIsWalkable = false;
		MeshID = EWT_TileID::TI_Raised;
	}




	UPROPERTY(BlueprintReadOnly)
		int InstanceID;
	UPROPERTY(BlueprintReadOnly)
		int Channel;
	UPROPERTY(BlueprintReadOnly)
		bool bIsWalkable;
	UPROPERTY(BlueprintReadOnly)
		EWT_TileID MeshID;
};






UCLASS()
class WORLDSEED_API AWT_WorldChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWT_WorldChunk();


	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	


	UStaticMesh* Floor;

	UStaticMesh* Raised;

	TMap<EWT_TileID, FInstanceStack*> MeshInstances;

	

	TMap<FVector2D,class UStaticMeshComponent*> Meshes;

	class USceneComponent* SceneRoot;

	int ChunkSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FVector2D, FWT_TileData> ConstructionData;



	int SetTile(FVector Position,EWT_TileDirection Dir ,EWT_TileID ID, int Channel);
	void SetTile(FVector2D Position, EWT_TileID ID, int Channel, int AdjacentData);
	void ClearTile(int ID, int Channel,  EWT_TileID MeshID);

public:	

	UPROPERTY(EditAnywhere, Category = "Test")
		FVector2D ChunkPosition;

	


	void Generate(class AWT_GeneratorCore* Generator, FVector2D GridPosition);

private: 



	bool IsFloorAdjacent(FVector2D Pos, AWT_GeneratorCore* Generator);

	void SetCorner(FVector2D Pos);




	bool IsTile(FVector2D Pos, EWT_TileID ID) 
	{
		if (ConstructionData.Find(Pos))
		{
			return (ConstructionData[Pos].MeshID == ID);
		}
		else return false;
	}

	bool IsEdge(FVector2D Pos)
	{
		if (ConstructionData.Find(Pos))
		{
			EWT_TileID CachedID = ConstructionData[Pos].MeshID;
			return (CachedID == EWT_TileID::TI_InnerCorner || CachedID == EWT_TileID::TI_OuterCorner || CachedID == EWT_TileID::TI_Edge);
		}
		else return false;
	}


	void GenerateMeshes();

};
