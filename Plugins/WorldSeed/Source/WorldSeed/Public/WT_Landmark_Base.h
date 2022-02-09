// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WT_Landmark_Base.generated.h"




UENUM(BlueprintType)
 enum class EWT_LandMarkType : uint8	
{ 
	LMT_None		UMETA(DisplayName = "None"),	//Empty Landmark
	LMT_Sice		UMETA(DisplayName = "Slice"), //removes terrainBlocks
	LMT_Add			UMETA(DisplayName = "Add"),  //Adds additional Terain blocks
	LMT_Remove		UMETA(DisplayName = "Clear"), // Removes all tiles and leaves an empty space
	LMT_Room		UMETA(DisplayName = "Room"),   // Adds a room landmark
	LMT_Corridor    UMETA(DisplayName = "Corridor"),//Adds a corridor landmark



	LMT_Raise		UMETA(DisplayName = "Raise"), //Raises the floor 
	LMT_Lower		UMETA(DisplayName = "Lower"), //Lowers the floor 
	
    
};

/*
* 
STRUCT() struct FLandmarkData
{
	GENERATED_BODY()

		FVector2D Position;
		FVector2D Scale;



};
*/




UCLASS(hidecategories=(Rendering,Replication,Collision,Input,Actor,LOD,Cooking))
class WORLDSEED_API AWT_Landmark_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWT_Landmark_Base();

protected:

	class USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, Category = "Landmark")
	FVector LandmarkScale;
	UPROPERTY(EditAnywhere, Category = "Landmark")
	FVector2D LandmarkPosition;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "TestEvent")
	void OnMovementDetected();

	virtual void ApplyLandmark(class AWT_GeneratorCore* Generator);

};
