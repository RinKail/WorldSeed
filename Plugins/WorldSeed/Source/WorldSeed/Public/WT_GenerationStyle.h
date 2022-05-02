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


		 void ClearLandmarks();

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
		return (Position.X > GridBoundaries && Position.Y > GridBoundaries &&
			Position.X < (GridScale.X - GridBoundaries) && Position.Y < (GridScale.Y - GridBoundaries));
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
	bool AddBox(FVector Position, FVector Scale);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
	bool AddCylinder(FVector Position, int Height, int Radius);


	//Useful functions releated to the grid

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
		FVector GetGridBounds() { return FVector((int)GridScale.X - GridBoundaries, (int)GridScale.Y - GridBoundaries, (int)GridScale.Y - GridBoundaries); }

	//Returns a scale between 0 and provided value
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
		FVector GetRandomScale(FVector Scale);

	//Returns a scale between 0 and provided value
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
		FVector GetRandomScaleInRange(FVector Start, FVector End);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
		FVector GetPositionInRange(FVector Position, FVector Range);




	//Generic generation function. This runs before any style specific events. 
	UFUNCTION(BlueprintImplementableEvent)
		void Run();

	virtual void RunBackend();

	//Generic generation functions, this runs after any style speficic events. 
	UFUNCTION(BlueprintImplementableEvent)
		void Finish();


protected: 
	UPROPERTY()
		class AWT_GeneratorCore* GenCore;

	UPROPERTY()
	TArray<class AWT_Landmark_Base*> LandmarkList;

	UPROPERTY()
		TArray<class AWT_Landmark_Base*> Landmark_CorridorList;

	UPROPERTY()
		TArray<class AWT_Landmark_Base*> Landmark_RoomList;

	
	
};
