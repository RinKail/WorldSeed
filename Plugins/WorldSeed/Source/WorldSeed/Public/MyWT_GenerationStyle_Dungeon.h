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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	bool bIsConnected;

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

	int GetSurfaceHeight(FVector2D Pos);


	//Attempts to add a room to the grid, returns false if parts of the room went out of bounds.
	UFUNCTION(BlueprintCallable, Category = "Layout")
		bool AddRoom(FVector Position, FVector Scale, bool bCanOverlapRooms);

	//Adds a corridor onto the grid.
	UFUNCTION(BlueprintCallable, Category = "Layout")
	void AddCorridor(FVector StartPosition, FVector EndPosition, TArray<FVector> AnchorPoints, int Thickness, bool bCanOverlap, bool bPrioritiseStraight);

	UFUNCTION(BlueprintCallable, Category = "Layout")
	void AddCube(FVector Position, FVector Scale, bool bAdditive);


	//Directly connects two rooms, this should be used over AddCorridor when building links between a lot of rooms. Will not connect rooms already directly connected
	UFUNCTION(BlueprintCallable, Category = "Layout")
	bool ConnectRoom(FRoomData Room, FRoomData Room2, int Thickness, bool bCanOverlap, bool bPrioritiseStraight);

	UFUNCTION(BlueprintPure, Category = "Grid")
	FRoomData FindRandomRoom();
	UFUNCTION(BlueprintPure, Category = "Grid")
	FRoomData FindNearestValidRoom(FRoomData Room);

	FRoomData FindNearestRoom();

	UFUNCTION(BlueprintCallable, Category = "Rooms")
	TArray<FRoomData> CalculateRoomData();

	UFUNCTION(BlueprintPure, Category = "Rooms")
		TArray<FRoomData> GetRoomList();


	TArray<FRoomData> RoomList;

	TArray<FRoomData> GetSortedList_Distance(FVector Position);

	float GetRoomDistance(FRoomData R1, FRoomData R2);


private: 

	void ApplyRoomChanges(FRoomData Room);

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
