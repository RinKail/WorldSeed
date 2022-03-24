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
public:
		AWT_Landmark_Corridor();

		void InitialiseCorridor(FVector Start, FVector End, TArray<FVector> AnchorPositions, int Thickness, bool bCanOverlap, bool bPrioritiseStraight);


		virtual void Destroy();
protected: 





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

	UPROPERTY(EditAnywhere, Category = "Pathing")
	bool bForcedStraight;


	UPROPERTY(EditAnywhere, Category = "Anchors")
	TArray<class AWT_LandmarkAnchor_Base*> Anchors;


	void StandardCorridor(class AWT_GeneratorCore* Generator);

	void StraightCorridor(class AWT_GeneratorCore* Generator);


	void ApplySpace(FVector Position, class AWT_GeneratorCore* Gen);

	TArray<FVector> GetSortedAnchors();

public:

	virtual void ApplyLandmark(class AWT_GeneratorCore* Generator);
	




};
