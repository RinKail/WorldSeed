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
struct FChunk_TileData
{
	GENERATED_BODY()


		UPROPERTY()
		int InstanceID;

		UPROPERTY()
		EWT_TileID TypeID;

		UPROPERTY()
		bool bSpaceWalkable;

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


	TMap<FVector2D, FChunk_TileData> ConstructionData;



	void SetTile(FVector Position,EWT_TileDirection Dir ,EWT_TileID ID, int Channel);

public:	

	UPROPERTY(EditAnywhere, Category = "Test")
		FVector2D ChunkPosition;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void Generate(class AWT_GeneratorCore* Generator, FVector2D GridPosition);

};
