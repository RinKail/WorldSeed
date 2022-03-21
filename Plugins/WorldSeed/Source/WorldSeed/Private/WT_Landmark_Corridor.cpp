// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_Landmark_Corridor.h"
#include "Engine/World.h"
#include "WorldSeed/Public/WT_LandmarkAnchor_Base.h"
#include "Components/BillboardComponent.h"
#include "WorldSeed/public/WT_Resources.h"
#include "WorldSeed/public/WT_GeneratorCore.h"


AWT_Landmark_Corridor::AWT_Landmark_Corridor()
{
	CorridorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CorridorRoot"));
	SetRootComponent(CorridorRoot);

	CorridorStart = CreateDefaultSubobject<USceneComponent>(TEXT("CorridorStart"));
	CorridorStart->SetupAttachment(RootComponent);


	CorridorEnd = CreateDefaultSubobject<USceneComponent>(TEXT("CorridorEnd"));
	CorridorEnd->SetupAttachment(RootComponent);
	Channel = 0;
	bAdditive = false;

}

void AWT_Landmark_Corridor::AddAnchor()
{
	UBillboardComponent* NewAnchor = NewObject<UBillboardComponent>(this);
	NewAnchor->RegisterComponent();
	NewAnchor->SetupAttachment(RootComponent);
	
}


void AWT_Landmark_Corridor::ApplyLandmark(class AWT_GeneratorCore* Generator)
{
	if (bCanOverlapExistingAreas)
	{
		FVector Start = FVector((int)CorridorStart->GetComponentLocation().X, (int)CorridorStart->GetComponentLocation().Y, (int)CorridorStart->GetComponentLocation().Z);
		FVector End = FVector((int)CorridorEnd->GetComponentLocation().X, (int)CorridorEnd->GetComponentLocation().Y, (int)CorridorEnd->GetComponentLocation().Z);
		Start /= TileScale;
		End /= TileScale;


		FVector CurrentPos = Start;
		
		TArray<FVector> CorridorPositions;


		TArray<FVector> AnchorList = GetSortedAnchors();
		int CurrentTarget = 0;

		TArray<FVector> Directions;
		Directions.Add(FVector(1, 0, 0));
		Directions.Add(FVector(-1, 0, 0));
		Directions.Add(FVector(0, 1, 0));
		Directions.Add(FVector(0, -1, 0));
		Directions.Add(FVector(0, 0, 1));
		Directions.Add(FVector(0, 0, -1));

		int PathCap = 100;
		while (CurrentPos != End)
		{
			PathCap--;
			float Distance = 100000000;
			FVector Best = FVector(0,0,0);
			for (int i = 0; i < Directions.Num(); i++)
			{
				if (FVector::Distance(CurrentPos + Directions[i], AnchorList[CurrentTarget]) < Distance)
				{
					Best = Directions[i];
					Distance = FVector::Distance(CurrentPos + Directions[i], AnchorList[CurrentTarget]);
				}
			}


			Generator->SetCellState(CurrentPos + Best, bAdditive);
			CorridorPositions.Add(CurrentPos + Best);
			CurrentPos += Best;

			if (CurrentPos == AnchorList[CurrentTarget])
			{
				CurrentTarget++;
			}
			else if (PathCap <= 0)
			{
				break;
			}
		}

		

	}
	else
	{

	}
	


}


TArray<FVector> AWT_Landmark_Corridor::GetSortedAnchors()
{
	TArray<FVector> SortedAnchors;
	TArray<FVector> ReturnList;


	FVector CurrentAnchor = CorridorStart->GetComponentLocation() / TileScale;
	for (int i = 0; i < Anchors.Num(); i++)
	{
		SortedAnchors.Add(Anchors[i]->GetComponentLocation() / TileScale);

	}

	for (int i = 0; i < SortedAnchors.Num(); i++)
	{


		float Distance = 100000;
		int BestIndex = 0;
		for (int x = 0; x < SortedAnchors.Num(); x++)
		{
			if (FVector::Distance(CurrentAnchor, SortedAnchors[0]) > FVector::Distance(CurrentAnchor, SortedAnchors[i]) && !ReturnList.Contains(SortedAnchors[i]))
			{
				Distance = FVector::Distance(CurrentAnchor, SortedAnchors[0]);
				BestIndex = i;
			}

		}
		
		ReturnList.Add(SortedAnchors[i]);
		CurrentAnchor = SortedAnchors[i];

		

	}

	ReturnList.Add(FVector((int)CorridorEnd->GetComponentLocation().X, (int)CorridorEnd->GetComponentLocation().Y, (int)CorridorEnd->GetComponentLocation().Z) / TileScale);

	return ReturnList;
}