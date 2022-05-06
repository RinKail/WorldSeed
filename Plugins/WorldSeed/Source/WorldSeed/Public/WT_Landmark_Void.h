// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldSeed/Public/WT_Landmark_Base.h"
#include "WT_Landmark_Void.generated.h"

/**
 * 
 */
UCLASS()
class WORLDSEED_API AWT_Landmark_Void : public AWT_Landmark_Base
{
	GENERATED_BODY()


public:
		virtual void ApplyLandmark(class AWT_GeneratorCore* Generator);
	
};
