// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WT_GenerationStyle.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class WORLDSEED_API UWT_GenerationStyle : public UObject
{
	GENERATED_BODY()

public:






	UWT_GenerationStyle();

		 void GenerateStyle(class AWT_GeneratorCore* Core);

protected:


	//This determines how close the the grid exterior rooms and corridors can be placed.
	UPROPERTY(EditAnywhere, Category = "Grid")
		int GridBoundaries;

	//How large the grid actually is
	UPROPERTY(EditAnywhere, Category = "Grid")
		FVector GridScale;

	UFUNCTION(BlueprintPure, Category = "Grid Manipulation")
	bool IsPositionValid(FVector Position)
	{
		return (Position.X > GridBoundaries && Position.Y > GridBoundaries && Position.Z > GridBoundaries &&
			Position.X > (GridScale.X - GridBoundaries) && Position.Y > (GridScale.Y - GridBoundaries) && Position.Z > (GridScale.Z - GridBoundaries));
	}


	//Useful functions releated to the grid

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
		FVector GetGridBounds() { return FVector(GridScale.X - GridBoundaries, GridScale.Y - GridBoundaries, GridScale.Y - GridBoundaries); }

	//Returns a scale between 0 and provided value
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
		FVector GetRandomScale(FVector Scale);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
		FVector GetPositionInRange(FVector Position, FVector Range);





	UFUNCTION(BlueprintImplementableEvent)
		void Run();

	virtual void RunBackend();

protected: 
	UPROPERTY()
		class AWT_GeneratorCore* GenCore;

	UPROPERTY()
	TArray<class AWT_Landmark_Base*> LandmarkList;
	
};
