// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WT_GenerationStyle.h"
#include "MyWT_GenerationStyle_Dungeon.generated.h"

/**
 * 
 */


/*
* USTRUCT(BlueprintType)
struct FGenerationStyleData
{
	GENERATED_BODY()



public:

	FGenerationStyleData();

	TArray<class AWT_Landmark_Base*> LandmarkList;



};


*/




UCLASS(Blueprintable, BlueprintType)
class WORLDSEED_API UWT_GenerationStyle_Dungeon : public UWT_GenerationStyle
{
	GENERATED_BODY()


public: 


	

	
	

protected: 
	
	UPROPERTY(EditAnywhere, Category = "Grid")
	int GridBoundaries;





protected: 

	//Useful functions releated to the grid

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
	FVector GetGridBounds();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
	FVector GetRandomScale(FVector Scale);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
	FVector GetPositionInRange(FVector Position, FVector Range);


	//Attempts to add a room to the grid, returns false if parts of the room went out of bounds.
	UFUNCTION(BlueprintCallable, Category = "Layout")
		bool AddRoom(FVector Position, FVector Scale, bool bCanOverlapRooms);

	//Adds a corridor onto the grid.
	UFUNCTION(BlueprintCallable, Category = "Layout")
		void AddCorridor(FVector StartPosition, FVector EndPosition);






private: 


};
