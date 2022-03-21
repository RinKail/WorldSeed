// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WT_LandmarkAnchor_Base.generated.h"

UCLASS()
class WORLDSEED_API AWT_LandmarkAnchor_Base : public AActor
{
	GENERATED_BODY()
	
public:	

	
	UPROPERTY()
	class UBillboardComponent* AnchorIcon;

	// Sets default values for this actor's properties
	AWT_LandmarkAnchor_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
