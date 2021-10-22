// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WT_WorldChunk.generated.h"

UCLASS()
class WORLDSEED_API AWT_WorldChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWT_WorldChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




	UStaticMesh* Floor;

	UStaticMesh* Raised;



	TMap<FVector2D,class UStaticMeshComponent*> Meshes;

	class USceneComponent* SceneRoot;

	int ChunkSize;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void Generate(class AWT_GeneratorCore* Generator, FVector2D GridPosition);

};
