// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWT_GenerationStyle_Dungeon.h"

#include "WorldSeed/Public/WT_GeneratorCore.h"
#include "WorldSeed/public/WT_Landmark_Base.h"
#include "WorldSeed/public/WT_Landmark_Corridor.h"

UWT_GenerationStyle_Dungeon::UWT_GenerationStyle_Dungeon()
{
    UE_LOG(LogTemp, Warning, TEXT("Dungeon Type instantiated"));
}

void UWT_GenerationStyle_Dungeon::RunBackend()
{
    GenerateRooms();

    CalculateRoomData();

    GenerateCorridors(RoomList);
   
}




bool UWT_GenerationStyle_Dungeon::AddRoom(FVector Position, FVector Scale, bool bCanOverlapRooms)
{

    FVector PosEnd = FVector((int)Position.X, (int)Position.Y, (int)Position.Z) + Scale;
   // FVector Pos = FVector((int)Position.X / TileScale, (int)Position.Y / TileScale, (int)Position.Z / TileScale);


    UE_LOG(LogTemp, Warning, TEXT("Pos: [%f | %f],PosEnd: [%f | %f], Boundary: [%f | %f]"), Position.X, Position.Y, PosEnd.X, PosEnd.Y, GetGridBounds().X, GetGridBounds().Y);
   // UE_LOG(LogTemp, Warning, TEXT("PosEnd: [%f | %f]"), PosEnd.X, PosEnd.Y);

    
    if ((Position.X > GridBoundaries && PosEnd.X < GetGridBounds().X) &&
        (Position.Y > GridBoundaries && PosEnd.Y < GetGridBounds().Y))
        
    {
        
        bool bValidRoom = true;
        if (!bCanOverlapRooms)
        {
            for (int i = 0; i < Landmark_RoomList.Num(); i++)
            {
                if (DoRoomsOverlap(Position, Scale, LandmarkList[i]->GetLandmarkPosition(), LandmarkList[i]->GetLandmarkScale()))
                {
                    bValidRoom = false;
                    break;
                }

            }
        }
        if (bValidRoom)
        {
            UE_LOG(LogTemp, Warning, TEXT("Generating Room"));

            AWT_Landmark_Base* Room = GenCore->GetWorld()->SpawnActor<AWT_Landmark_Base>(AWT_Landmark_Base::StaticClass());

            Room->SetLandmarkPosition(Position * TileScale);
            Room->SetActorLocation(Position * TileScale);
            Room->SetLandmarkScale(Scale);

            Landmark_RoomList.Add(Room);
            LandmarkList.Add(Room);

            return true;
        }
        else return false;
       
    }
   
   
    


 
   


    return false;
}

void UWT_GenerationStyle_Dungeon::AddCorridor(FVector StartPosition, FVector EndPosition, TArray<FVector> AnchorPositions)
{

    AWT_Landmark_Corridor* Corridor = GenCore->GetWorld()->SpawnActor<AWT_Landmark_Corridor>(AWT_Landmark_Corridor::StaticClass());

 //   Corridor->SetActorLocation(((StartPosition + EndPosition) / 2));

    Corridor->InitialiseCorridor(StartPosition, EndPosition, AnchorPositions);


    Landmark_CorridorList.Add(Corridor);
    LandmarkList.Add(Corridor);


}

FRoomData UWT_GenerationStyle_Dungeon::FindRandomRoom()
{
    int Index = FMath::RandRange((int)0, (int)RoomList.Num() - 1);
    return RoomList[Index];
}

bool UWT_GenerationStyle_Dungeon::ConnectRoom(FRoomData Room, FRoomData Room2)
{
    bool bValidRoomFound = false;
  
   

    bool bFailed = false;
    for (int i = 0; i < Room.ConnectedRooms.Num(); i++)
    {
        if (Room.ConnectedRooms[i].RoomId == Room2.RoomId)
        {
            bFailed = true;
        }
    }

    if (!bFailed)
    {
       
        FVector Pos1 = Room.AveragePosition;
        FVector Pos2 = Room2.AveragePosition;

        TArray<FVector> AnchorList;
        AddCorridor(Pos1, Pos2, AnchorList);

        Room.ConnectedRooms.Add(Room2);
        Room2.ConnectedRooms.Add(Room);
       
    }

   

    return bFailed;

}

TArray<FRoomData> UWT_GenerationStyle_Dungeon::CalculateRoomData()
{
    int ID = 0;
    TArray<FRoomData> ReturnData;
    TArray<AWT_Landmark_Base*> SortedList;
    for (int i = 0; i < Landmark_RoomList.Num(); i++)
    {
        if (IsValid(Landmark_RoomList[i]) )
        {
            SortedList.Add(Landmark_RoomList[i]);
            FRoomData CurrentRoom;
            CurrentRoom.Landmarks.Add(Landmark_RoomList[i]);
            for (int x = 0; x < Landmark_RoomList.Num(); x++)
            {
                FVector L1Pos = FVector(Landmark_RoomList[i]->GetActorLocation().X / TileScale, Landmark_RoomList[i]->GetActorLocation().Y / TileScale, Landmark_RoomList[i]->GetActorLocation().Z / TileScale);
                FVector L2Pos = FVector(Landmark_RoomList[x]->GetActorLocation().X / TileScale, Landmark_RoomList[x]->GetActorLocation().Y / TileScale, Landmark_RoomList[x]->GetActorLocation().Z / TileScale);
                if (DoRoomsOverlap(L1Pos, Landmark_RoomList[i]->GetLandmarkScale(), L2Pos, Landmark_RoomList[x]->GetLandmarkScale()))
                {
                    CurrentRoom.Landmarks.Add(Landmark_RoomList[x]);
                    SortedList.Add(Landmark_RoomList[x]);
                }

            }
            FVector AveragePosition = FVector(0, 0, 0);
            for (int x = 0; x < CurrentRoom.Landmarks.Num(); x++)
            {
                
                AveragePosition += CurrentRoom.Landmarks[x]->GetActorLocation();
            }
            AveragePosition /= CurrentRoom.Landmarks.Num();
            CurrentRoom.AveragePosition = FVector((int)AveragePosition.X / TileScale, (int)AveragePosition.Y / TileScale, (int)AveragePosition.Z / TileScale);
            CurrentRoom.RoomId = ID;
            ReturnData.Add(CurrentRoom);
            ID++;
       }




    }
    SortedList.Empty();
    RoomList.Empty();
    RoomList = ReturnData;
    return ReturnData;
}
