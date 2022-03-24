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

int UWT_GenerationStyle_Dungeon::GetSurfaceHeight(FVector2D Pos)
{
    for (int z = GenCore->GetGridScale().Z - 1; z > 0; z--)
    {
        if (GenCore->GetCellState(FVector(Pos.X, Pos.Y, z)))
        {
            return z;
        }
    }
    return GenCore->GetGridScale().Z;

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


void UWT_GenerationStyle_Dungeon::AddCorridor(FVector StartPosition, FVector EndPosition, TArray<FVector> AnchorPositions, int Thickness, bool bCanOverlap, bool bPrioritiseStraight)
{

    AWT_Landmark_Corridor* Corridor = GenCore->GetWorld()->SpawnActor<AWT_Landmark_Corridor>(AWT_Landmark_Corridor::StaticClass());

 //   Corridor->SetActorLocation(((StartPosition + EndPosition) / 2));

    Corridor->InitialiseCorridor(StartPosition, EndPosition, AnchorPositions, Thickness, bCanOverlap, bPrioritiseStraight);


    Landmark_CorridorList.Add(Corridor);
    LandmarkList.Add(Corridor);


}

void UWT_GenerationStyle_Dungeon::AddCube(FVector Position, FVector Scale, bool bAdditive)
{

    AWT_Landmark_Base* Shape = GenCore->GetWorld()->SpawnActor<AWT_Landmark_Base>(AWT_Landmark_Base::StaticClass());
    Shape->SetLandmarkPosition(Position);
    Shape->SetLandmarkScale(Scale);
    

}

FRoomData UWT_GenerationStyle_Dungeon::FindRandomRoom()
{
    int Index = FMath::RandRange((int)0, (int)RoomList.Num() - 1);
    return RoomList[Index];
}

FRoomData UWT_GenerationStyle_Dungeon::FindNearestValidRoom(FRoomData Room)
{
    TArray<FRoomData> OrderedDistance = GetSortedList_Distance(Room.AveragePosition);
    
   

    UE_LOG(LogTemp, Warning, TEXT("Ordered List Length: %d"), OrderedDistance.Num());

    for (int i = 0; i < OrderedDistance.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("Building Corridor, Room 1: %d | Room 2: %d"), Room.RoomId, OrderedDistance[i].RoomId);

        if (Room.RoomId != OrderedDistance[i].RoomId)
        {
            bool bValid = true;
            for (int x = 0; x < Room.ConnectedRooms.Num(); x++)
            {
              
                if (Room.ConnectedRooms[x].RoomId == OrderedDistance[i].RoomId)
                {
                    bValid = false;
                }

                
            }
            if (bValid)
            {
                return OrderedDistance[i];
            }


            

        }

        
    }

    return OrderedDistance[(int)FMath::RandRange(0, (OrderedDistance.Num()- 1) / 3)];
   



  
}

TArray<FRoomData> UWT_GenerationStyle_Dungeon::GetRoomList()
{
    for (int i = 0; i < RoomList.Num(); i++)
    {
        if (RoomList[i].ConnectedRooms.Num() > 0) RoomList[i].bIsConnected = true;
        else RoomList[i].bIsConnected = false;
    }
    return RoomList;
}

TArray<FRoomData> UWT_GenerationStyle_Dungeon::GetSortedList_Distance(FVector Position)
{
    TArray<FRoomData> SortedList = RoomList;

    bool bSorted = false;
    while (!bSorted)
    {
        bSorted = true;
        for (int i = 0; i < SortedList.Num(); i++)
        {
            if ((i + 1) < (SortedList.Num() - 1))
            {
                if (FVector::Distance(Position, SortedList[i].AveragePosition) > FVector::Distance(Position, SortedList[i + 1].AveragePosition))
                {
                    FRoomData Temp = SortedList[i + 1];
                    SortedList[i + 1] = SortedList[i];
                    SortedList[i] = Temp;

                    bSorted = false;
                    
                }
            }
        }
    }


    return SortedList;
}

float UWT_GenerationStyle_Dungeon::GetRoomDistance(FRoomData R1, FRoomData R2)
{
    return FVector::Distance(R1.AveragePosition, R2.AveragePosition);
}

void UWT_GenerationStyle_Dungeon::ApplyRoomChanges(FRoomData Room)
{
    for (int i = 0; i < RoomList.Num(); i++)
    {
        if (RoomList[i].RoomId == Room.RoomId)
        {
            RoomList[i] = Room;
        }
    }
}

bool UWT_GenerationStyle_Dungeon::ConnectRoom(FRoomData Room, FRoomData Room2, int Thickness, bool bCanOverlap, bool bPrioritiseStraight)
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
        AddCorridor(Pos1, Pos2, AnchorList, Thickness, bCanOverlap, bPrioritiseStraight);

        Room.ConnectedRooms.Add(Room2);
        ApplyRoomChanges(Room);
        Room2.ConnectedRooms.Add(Room);
        ApplyRoomChanges(Room2);
       
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
                FVector HalfScale = FVector((int)CurrentRoom.Landmarks[x]->GetLandmarkScale().X / 2, (int)CurrentRoom.Landmarks[x]->GetLandmarkScale().Y / 2, 0);
                FVector ConvertedPosition = FVector((int)CurrentRoom.Landmarks[x]->GetActorLocation().X / TileScale, (int)CurrentRoom.Landmarks[x]->GetActorLocation().Y / TileScale, (int)CurrentRoom.Landmarks[x]->GetActorLocation().Z / TileScale);
                AveragePosition += ConvertedPosition + HalfScale;
            }
            AveragePosition /= CurrentRoom.Landmarks.Num();
            CurrentRoom.AveragePosition = FVector((int)AveragePosition.X, (int)AveragePosition.Y, (int)AveragePosition.Z);
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
