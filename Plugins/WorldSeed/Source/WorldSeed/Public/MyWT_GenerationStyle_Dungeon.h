// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WT_GenerationStyle.h"
#include "MyWT_GenerationStyle_Dungeon.generated.h"

/**
 * 
 */



 USTRUCT(BlueprintType)
struct FRoomData
{
	GENERATED_BODY()



public:

	FRoomData()
	{
		AveragePosition = FVector(0, 0, 0);
	}


	//Array of landmarks that make up this room
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	TArray<class AWT_Landmark_Base*> Landmarks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FVector AveragePosition;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	TArray<FRoomData> ConnectedRooms;

	int RoomId;


};







UCLASS(Blueprintable, BlueprintType)
class WORLDSEED_API UWT_GenerationStyle_Dungeon : public UWT_GenerationStyle
{
	GENERATED_BODY()


public: 


	UWT_GenerationStyle_Dungeon();

	
	

protected: 
	

	//Generate Rooms using this event.
	UFUNCTION(BlueprintImplementableEvent)
	void GenerateRooms();
	//Generate Corridors using this event. The room array provides a list of all the rooms created in the first generation pass.
	UFUNCTION(BlueprintImplementableEvent)
		void GenerateCorridors(const TArray<FRoomData>& Rooms);
	
	virtual void RunBackend();


protected: 

	


	//Attempts to add a room to the grid, returns false if parts of the room went out of bounds.
	UFUNCTION(BlueprintCallable, Category = "Layout")
		bool AddRoom(FVector Position, FVector Scale, bool bCanOverlapRooms);

	//Adds a corridor onto the grid.
	UFUNCTION(BlueprintCallable, Category = "Layout")
	void AddCorridor(FVector StartPosition, FVector EndPosition, TArray<FVector> AnchorPoints);


	//Directly connects two rooms, this should be used over AddCorridor when building links between a lot of rooms. Will not connect rooms already directly connected
	UFUNCTION(BlueprintCallable, Category = "Layout")
	bool ConnectRoom(FRoomData Room, FRoomData Room2);

	UFUNCTION(BlueprintPure, Category = "Grid")
	FRoomData FindRandomRoom();


	UFUNCTION(BlueprintCallable, Category = "Rooms")
	TArray<FRoomData> CalculateRoomData();


	TArray<FRoomData> RoomList;


private: 



	bool DoRoomsOverlap(FVector PosA, FVector ScaleA, FVector PosB, FVector ScaleB)
	{
		FVector PosAEnd = PosA + ScaleA;
		FVector PosBEnd = PosB + ScaleB;
		return (PosA.X <= PosBEnd.X && PosAEnd.X >= PosB.X &&
			PosA.Y <= PosBEnd.Y && PosAEnd.Y >= PosB.Y &&
			PosA.Z <= PosBEnd.Z && PosAEnd.Z >= PosB.Z
			);
	}

};
