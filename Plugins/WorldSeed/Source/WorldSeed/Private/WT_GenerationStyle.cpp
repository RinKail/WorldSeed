// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GenerationStyle.h"
#include "WorldSeed/Public/WT_GeneratorCore.h"
#include "WorldSeed/Public/WT_Landmark_Base.h"



UWT_GenerationStyle::UWT_GenerationStyle()
{
	UE_LOG(LogTemp, Warning, TEXT("Normal Type instantiated"));
}

void UWT_GenerationStyle::GenerateStyle(AWT_GeneratorCore* Core)
{

	GenCore = Core;

	GridScale = Core->GetGridScale();
	GridBoundaries = 5;

	UE_LOG(LogTemp, Warning, TEXT("Running Style"));

	Run();

	RunBackend();

	Finish();


	Core->LoadStyleLandmarks(LandmarkList);

	UE_LOG(LogTemp, Warning, TEXT("Total Rooms: %d"), LandmarkList.Num());
}



FVector UWT_GenerationStyle::GetRandomScale(FVector Scale)
{
	return FVector((int)FMath::RandRange(0, (int)Scale.X), (int)FMath::RandRange(0, (int)Scale.Y), (int)FMath::RandRange(0, (int)Scale.Y));
}

FVector UWT_GenerationStyle::GetRandomScaleInRange(FVector Start, FVector End)
{
	
	return FVector((int)FMath::RandRange((int)Start.X, (int)End.X), (int)FMath::RandRange((int)Start.Y, (int)End.Y), (int)FMath::RandRange((int)Start.Z, (int)End.Y));
	

}

FVector UWT_GenerationStyle::GetPositionInRange(FVector Position, FVector Range)
{
	FVector TempResult = FVector(0, 0, 0);
	bool bValidPosition = false;
	while (!bValidPosition)
	{
		TempResult = FVector((int)FMath::RandRange((int)-Range.X, (int)Range.X), (int)FMath::RandRange((int)-Range.Y, (int)Range.Y), (int)FMath::RandRange((int)-Range.Z, (int)Range.Y));
		TempResult += FVector((int)Position.X, (int)Position.Y, (int)Position.Z);
		bValidPosition = IsPositionValid(TempResult);
		UE_LOG(LogTemp, Warning, TEXT("Position in range: [%f | %f | %f]"), TempResult.X, TempResult.Y, TempResult.Z);
	}

	return TempResult;
}

void UWT_GenerationStyle::RunBackend()
{
	UE_LOG(LogTemp, Warning, TEXT("Running base backend"));
}

void UWT_GenerationStyle::ClearLandmarks()
{
	for (int i = 0; i < LandmarkList.Num(); i++)
	{
		if (IsValid(LandmarkList[i]) && LandmarkList[i] != nullptr)
		{
			LandmarkList[i]->Destroy();
		}
	}
	LandmarkList.Empty();
}
