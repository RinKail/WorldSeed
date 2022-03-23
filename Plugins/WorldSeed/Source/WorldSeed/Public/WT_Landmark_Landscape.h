// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/RandomStream.h"
#include "WT_Landmark_Base.h"
#include "WT_Landmark_Landscape.generated.h"

/**
 * 
 */
UCLASS()
class WORLDSEED_API AWT_Landmark_Landscape : public AWT_Landmark_Base
{
	GENERATED_BODY()

		//int Seed;


	void SetSeed(int In);

	void RandomSeed();

	bool bCustomSeed;

	FRandomStream SeedData;

public:


	virtual void ApplyLandmark(class AWT_GeneratorCore* Generator);
	

	
};
