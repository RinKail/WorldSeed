// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WT_Landmark_Base.h"
#include "WT_Landmark_Corridor.generated.h"

/**
 * 
 */





UCLASS()
class WORLDSEED_API AWT_Landmark_Corridor : public AWT_Landmark_Base
{
	GENERATED_BODY()

		AWT_Landmark_Corridor();


	UPROPERTY()
	class USceneComponent* CorridorRoot;
	UPROPERTY(EditAnywhere)
	class USceneComponent* CorridorStart;
	UPROPERTY(EditAnywhere)
	class USceneComponent* CorridorEnd;



		//Adds a path anchor that the pathing algorithm will priorities.
		UFUNCTION(CallInEditor)
		void AddAnchor();
	

	UPROPERTY(EditAnywhere, Category = "Visuals")
	int CorridorThickness;
	UPROPERTY(EditAnywhere, Category = "Visuals")
	int CorridorHeight;

	UPROPERTY(EditAnywhere, Category = "Pathing")
	bool bCanOverlapExistingAreas;


	UPROPERTY(EditAnywhere, Category = "Anchors")
	TArray<UBillboardComponent*> Anchors;


	




	virtual void ApplyLandmark(class AWT_GeneratorCore* Generator);
	

	TArray<FVector> GetSortedAnchors();


};
