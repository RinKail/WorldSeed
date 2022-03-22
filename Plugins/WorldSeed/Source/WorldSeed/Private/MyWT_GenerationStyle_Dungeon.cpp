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
            for (int i = 0; i < LandmarkList.Num(); i++)
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

    Corridor->SetActorLocation((StartPosition + EndPosition) / 2);

    Corridor->InitialiseCorridor(StartPosition, EndPosition, AnchorPositions);


}
