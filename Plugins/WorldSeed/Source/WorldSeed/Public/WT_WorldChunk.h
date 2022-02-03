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


	




};
