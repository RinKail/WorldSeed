// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSeed/Public/WT_Landmark_Base.h"

// Sets default values
AWT_Landmark_Base::AWT_Landmark_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWT_Landmark_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWT_Landmark_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

