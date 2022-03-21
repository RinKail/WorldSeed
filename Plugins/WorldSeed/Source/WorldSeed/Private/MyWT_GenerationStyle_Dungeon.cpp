// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWT_GenerationStyle_Dungeon.h"

#include "WorldSeed/Public/WT_GeneratorCore.h"
#include "WorldSeed/public/WT_Landmark_Base.h"



FVector UWT_GenerationStyle_Dungeon::GetGridBounds()
{
    return FVector();
}

FVector UWT_GenerationStyle_Dungeon::GetRandomScale(FVector Scale)
{
    return FVector();
}

FVector UWT_GenerationStyle_Dungeon::GetPositionInRange(FVector Position, FVector Range)
{
    return FVector();
}

bool UWT_GenerationStyle_Dungeon::AddRoom(FVector Position, FVector Scale, bool bCanOverlapRooms)
{

    FVector PosEnd = Position + Scale;

    if ((Position.X > GridBoundaries && PosEnd.X < ActiveGridSpace.X) ||
        (Position.Y > GridBoundaries && PosEnd.Y < ActiveGridSpace.Y) ||
        (Position.Z > GridBoundaries && PosEnd.Z < ActiveGridSpace.Z))
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

            AWT_Landmark_Base* Room = GetWorld()->SpawnActor<AWT_Landmark_Base>(AWT_Landmark_Base::StaticClass());

            Room->SetLandmarkPosition(Position);
            Room->SetLandmarkScale(Position);

            LandmarkList.Add(Room);
            return true;
        }
        else return false;
    }
   
   


    return false;
}

void UWT_GenerationStyle_Dungeon::AddCorridor(FVector StartPosition, FVector EndPosition)
{
}
