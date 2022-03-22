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

void AWT_Landmark_Corridor::InitialiseCorridor(FVector Start, FVector End, TArray<FVector> AnchorPositions)
{
	CorridorStart->SetWorldLocation(Start);
	CorridorEnd->SetWorldLocation(End);

	for (int i = 0; i < AnchorPositions.Num(); i++)
	{
		AWT_LandmarkAnchor_Base* NewAnchor = GetWorld()->SpawnActor<AWT_LandmarkAnchor_Base>(AWT_LandmarkAnchor_Base::StaticClass());

		FAttachmentTransformRules AttachRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		NewAnchor->AttachToActor(this, AttachRules);
		NewAnchor->SetActorLocation(AnchorPositions[i]);
		Anchors.Add(NewAnchor);
	}

}

void AWT_Landmark_Corridor::AddAnchor()
{
	AWT_LandmarkAnchor_Base* NewAnchor = GetWorld()->SpawnActor<AWT_LandmarkAnchor_Base>(AWT_LandmarkAnchor_Base::StaticClass());
	
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	NewAnchor->AttachToActor(this, AttachRules);
	Anchors.Add(NewAnchor);
	
}


void AWT_Landmark_Corridor::ApplySpace(FVector Position, AWT_GeneratorCore* Gen)
{
	for (int z = 0; z < CorridorHeight; z++)
	{
		Gen->SetCellState(Position + FVector(0,0, z), bAdditive);
	}


	
}

void AWT_Landmark_Corridor::ApplyLandmark(class AWT_GeneratorCore* Generator)
{
	if (bCanOverlapExistingAreas)
	{
		FVector Start = FVector((int)CorridorStart->GetComponentLocation().X / TileScale, (int)CorridorStart->GetComponentLocation().Y / TileScale, (int)CorridorStart->GetComponentLocation().Z / TileScale);
		FVector End = FVector((int)CorridorEnd->GetComponentLocation().X / TileScale, (int)CorridorEnd->GetComponentLocation().Y / TileScale, (int)CorridorEnd->GetComponentLocation().Z / TileScale);
	

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

		UE_LOG(LogTemp, Warning, TEXT("Start: [%f | %f], End: [%f | %f]"), Start.X, Start.Y, End.X, End.Y);


		while (CurrentPos != End)
		{
			PathCap--;

			if ((PathCap % 10) == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Current Position: [%f | %f], Target Position: [%f | %f]"), CurrentPos.X, CurrentPos.Y, AnchorList[CurrentTarget].X, AnchorList[CurrentTarget].Y);
			}
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


			ApplySpace(CurrentPos + Best, Generator);
			CorridorPositions.Add(CurrentPos + Best);
			CurrentPos += Best;

			if (CurrentPos == AnchorList[CurrentTarget] && CurrentPos != End)
			{

				CurrentTarget++;
			}
			else if (PathCap <= 0)
			{
				break;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Current Position: [%f | %f], Target Position: [%f | %f]"), CurrentPos.X, CurrentPos.Y, AnchorList[CurrentTarget].X, AnchorList[CurrentTarget].Y);

		

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
		if (IsValid(Anchors[i])) SortedAnchors.Add(Anchors[i]->GetActorLocation());

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
		
		ReturnList.Add(FVector((int)SortedAnchors[i].X / TileScale, (int)SortedAnchors[i].Y / TileScale, (int)SortedAnchors[i].Z / TileScale));
		CurrentAnchor = SortedAnchors[i];

		

	}

	ReturnList.Add(FVector((int)CorridorEnd->GetComponentLocation().X / TileScale, (int)CorridorEnd->GetComponentLocation().Y / TileScale, (int)CorridorEnd->GetComponentLocation().Z / TileScale));

	return ReturnList;
}