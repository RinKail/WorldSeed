// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSeed/Public/WT_WorldChunk.h"

// Sets default values
AWT_WorldChunk::AWT_WorldChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWT_WorldChunk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWT_WorldChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

