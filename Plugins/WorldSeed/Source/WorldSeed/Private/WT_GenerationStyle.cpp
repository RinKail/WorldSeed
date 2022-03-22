// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GenerationStyle.h"
#include "WorldSeed/Public/WT_GeneratorCore.h"




UWT_GenerationStyle::UWT_GenerationStyle()
{
	UE_LOG(LogTemp, Warning, TEXT("Normal Type instantiated"));
}

void UWT_GenerationStyle::GenerateStyle(AWT_GeneratorCore* Core)
{

	GenCore = Core;

	

	UE_LOG(LogTemp, Warning, TEXT("Running Style"));

	Run();

	RunBackend();

	Core->LoadStyleLandmarks(LandmarkList);
}



FVector UWT_GenerationStyle::GetRandomScale(FVector Scale)
{
	return FVector((int)FMath::RandRange(0, (int)Scale.X), (int)FMath::RandRange(0, (int)Scale.Y), (int)FMath::RandRange(0, (int)Scale.Y));
}

FVector UWT_GenerationStyle::GetPositionInRange(FVector Position, FVector Range)
{
	FVector TempResult = FVector(0, 0, 0);
	bool bValidPosition = false;
	while (!bValidPosition)
	{
		TempResult = FVector((int)FMath::RandRange((int)-Range.X, (int)Range.X), (int)FMath::RandRange((int)-Range.Y, (int)Range.Y), (int)FMath::RandRange((int)-Range.Z, (int)Range.Y));
		bValidPosition = IsPositionValid(TempResult);
	}

	return TempResult;
}

void UWT_GenerationStyle::RunBackend()
{
	UE_LOG(LogTemp, Warning, TEXT("Running base backend"));
}
