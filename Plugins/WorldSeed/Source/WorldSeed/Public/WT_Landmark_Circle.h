// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WT_Landmark_Base.h"
#include "WT_Landmark_Circle.generated.h"

/**
 * 
 */
UCLASS()
class WORLDSEED_API AWT_Landmark_Circle : public AWT_Landmark_Base
{
	GENERATED_BODY()
public: 

	virtual void ApplyLandmark(class AWT_GeneratorCore* Generator);


	void SetRadius(int InRadius) { Radius = InRadius; }
	void SetHeight(int InHeight) { Height = InHeight; }

protected: 


	UPROPERTY(EditAnywhere, Category = "Circle Landmark")
		int Radius;
	UPROPERTY(EditAnywhere, Category = "Circle Landmark")
	int Height;


	void FloodFill(class AWT_GeneratorCore* Generator, FVector Position);
	void FloodFill_Floor(class AWT_GeneratorCore* Generator, FVector Position);



};
