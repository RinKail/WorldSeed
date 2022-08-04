
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
	CorridorHeight = 5;
	CorridorThickness = 1;
	bAdditive = false;
	bIsWalkable = true;
	bCanOverlapExistingAreas = true;

}

void AWT_Landmark_Corridor::InitialiseCorridor(const FVector& Start, const FVector& End, TArray<FVector>& AnchorPositions, int Thickness, bool bCanOverlap, bool bPrioritiseStraight)
{
	CorridorStart->SetWorldLocation(FVector((int)Start.X * TileScale, (int)Start.Y * TileScale, (int)Start.Z * TileScale));
	CorridorEnd->SetWorldLocation(FVector((int)End.X * TileScale, (int)End.Y * TileScale, (int)End.Z * TileScale));
	bCanOverlapExistingAreas = bCanOverlap;
	bForcedStraight = bPrioritiseStraight;
	CorridorThickness = Thickness;

	for (int i = 0; i < AnchorPositions.Num(); i++)
	{
		AWT_LandmarkAnchor_Base* NewAnchor = GetWorld()->SpawnActor<AWT_LandmarkAnchor_Base>(AWT_LandmarkAnchor_Base::StaticClass());

		FAttachmentTransformRules AttachRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		NewAnchor->AttachToActor(this, AttachRules);
		NewAnchor->SetActorLocation(FVector((int)AnchorPositions[i].X * TileScale, (int)AnchorPositions[i].Y * TileScale, (int)AnchorPositions[i].Z * TileScale));
		Anchors.Add(NewAnchor);
	}

}

void AWT_Landmark_Corridor::Destroy()
{
	for (int i = 0; i < Anchors.Num(); i++)
	{
		Anchors[i]->Destroy();
	}
	Super::Destroy();
}



void AWT_Landmark_Corridor::AddAnchor()
{
	AWT_LandmarkAnchor_Base* NewAnchor = GetWorld()->SpawnActor<AWT_LandmarkAnchor_Base>(AWT_LandmarkAnchor_Base::StaticClass());
	
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	NewAnchor->AttachToActor(this, AttachRules);
	Anchors.Add(NewAnchor);
	
}


void AWT_Landmark_Corridor::StandardCorridor(class AWT_GeneratorCore* Generator)
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

	int PathCap = 1000;




	while (CurrentPos != End)
	{
		PathCap--;

		if ((PathCap % 10) == 0)
		{

		}
		float Distance = 100000000;
		FVector Best = FVector(0, 0, 0);
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
}

void AWT_Landmark_Corridor::StraightCorridor(AWT_GeneratorCore* Generator)
{
	FVector Start = FVector((int)CorridorStart->GetComponentLocation().X / TileScale, (int)CorridorStart->GetComponentLocation().Y / TileScale, (int)CorridorStart->GetComponentLocation().Z / TileScale);
	FVector End = FVector((int)CorridorEnd->GetComponentLocation().X / TileScale, (int)CorridorEnd->GetComponentLocation().Y / TileScale, (int)CorridorEnd->GetComponentLocation().Z / TileScale);



	FVector CurrentPos = Start;

	TArray<FVector> CorridorPositions;

	TArray<FVector> AnchorList = GetSortedAnchors();
	int CurrentTarget = 0;

	int PathCap = 1000;

	int XSteps = 0;
	int YSteps = 0;

	bool bHorizontal = false;

	

	if ((AnchorList[CurrentTarget].X - Start.X) > (AnchorList[CurrentTarget].Y - Start.Y))
	{
		bHorizontal = true;



		

	}
	XSteps = (AnchorList[CurrentTarget].X - Start.X);
	YSteps = AnchorList[CurrentTarget].Y - Start.Y;
	UE_LOG(LogTemp, Warning, TEXT("Start: [%f | %f], End: [%f | %f], Steps total: X: %d | Y: %d"), Start.X, Start.Y, End.X, End.Y, XSteps, YSteps);

	while (CurrentPos != End)
	{
		

		PathCap--;
		FVector Dir = FVector(0, 0, 0);

		
		if ((YSteps - XSteps) != YSteps)
		{
			if (XSteps > 0)
			{
				Dir.X = 1;
			}
			else
			{
				Dir.X = -1;
			}
			XSteps -= Dir.X;
		}
		else
		{
			if (YSteps > 0)
			{
				Dir.Y = 1;
			}
			else
			{
				Dir.Y = -1;
			}

			YSteps -= Dir.Y;
		}

		

		
		


		ApplySpace(CurrentPos + Dir, Generator);
		CorridorPositions.Add(CurrentPos + Dir);
		CurrentPos += Dir;
		



		if (CurrentPos == AnchorList[CurrentTarget] && CurrentPos != End)
		{

			CurrentTarget++;
		}
		else if (PathCap <= 0)
		{
			break;
		}
	}




}

void AWT_Landmark_Corridor::ApplySpace(const FVector& Position, AWT_GeneratorCore* Gen)
{
	for (int z = 0; z < CorridorHeight; z++)
	{
		for (int x = 0; x < CorridorThickness; x++)
		{
			for (int y = 0; y < CorridorThickness; y++)
			{
				Gen->SetCellState(Position + FVector(x, y, z), bAdditive);

				if (bIsWalkable)
				{
					if (bAdditive)
					{
						if (z >= CorridorHeight - 1)
						{
							Gen->AddFloorCell(Position + FVector(x, y, z));
						}
					}
					else
					{
						if (z == 0)
						{
							Gen->AddFloorCell(Position + FVector(x, y, z));
						}
					}
				}
			}
		}
	}


	
}

void AWT_Landmark_Corridor::ApplyLandmark(class AWT_GeneratorCore* Generator)
{
	if (bCanOverlapExistingAreas)
	{
		
		if (bForcedStraight)
		{
			StraightCorridor(Generator);
		}
		else
		{
			StandardCorridor(Generator);
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