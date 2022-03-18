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
    return false;
}

void UWT_GenerationStyle_Dungeon::AddCorridor(FVector StartPosition, FVector EndPosition)
{
}
