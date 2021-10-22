// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldSeedEdMode.h"
#include "WorldSeedEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Selection.h"
#include "WorldSeed/Public/WT_GeneratorCore.h"
#include "WorldSeed/Public/WT_WorldChunk.h"
#include "WorldSeed/Public/WT_Resources.h"
#include "EditorModeManager.h"


const FEditorModeID FWorldSeedEdMode::EM_WorldSeedEdModeId = TEXT("EM_WorldSeedEditor");

FWorldSeedEdMode::FWorldSeedEdMode()
{

}

FWorldSeedEdMode::~FWorldSeedEdMode()
{

}

void FWorldSeedEdMode::Enter()
{
	FEdMode::Enter();


	UE_LOG(LogTemp, Warning, TEXT("Entered WorldSeed Editor"));

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FWorldSeedEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());

		FWorldSeedEdModeToolkit* Temp = static_cast<FWorldSeedEdModeToolkit*>(Toolkit.Get());
		Temp->SetEditorReference(this);
	}

	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWT_GeneratorCore::StaticClass(), ActorList);
	if (ActorList.Num() > 0 && ActiveGenerator == nullptr)
	{
		ActiveGenerator = Cast<AWT_GeneratorCore>(ActorList[0]);
	}
	else
	{
		if (ActorList.Num() == 0)
		{
			ActiveGenerator = GetWorld()->SpawnActor<AWT_GeneratorCore>();
		}
	}

	//GetWorld()->SpawnActor<class AWT_GeneratorCore>();

	
}

void FWorldSeedEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}
	for (int i = 0; i < Landmark_List.Num(); i++)
	{
		Landmark_List[i]->Destroy();
	}
	Landmark_List.Empty();


	UE_LOG(LogTemp, Warning, TEXT("Exited WorldSeed Editor"));

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

void FWorldSeedEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{

	if (IsALandmarkSelected())
	{
	
		if (FVector::Distance(SelectedLandmark->GetActorLocation(), CachedLandmarkPosition) >= TileScale)
		{
			CachedLandmarkPosition = SelectedLandmark->GetActorLocation();
			ActiveGenerator->UpdateChunks();
		}
	}
	//GEditor->GetSelectedActors()
}

bool FWorldSeedEdMode::UsesToolkits() const
{
	return true;
}

void FWorldSeedEdMode::CreateLandmark(TSubclassOf<AWT_Landmark_Base> Class)
{
	
	Landmark_List.Add(GetWorld()->SpawnActor<AWT_Landmark_Base>(Class));
}

void FWorldSeedEdMode::GenerateGrid(int GridX, int GridY, int ChunkX, int ChunkY)
{
	ActiveGenerator->ClearChunkList();
	for (int x = 0; x < GridX; x++)
	{
		for (int y = 0; y < GridY; y++)
		{
			GetWorld()->SpawnActor<AWT_WorldChunk>(FVector((ChunkX * TileScale) * x, (ChunkY * TileScale) * y, 0), FRotator(0,0,0));
		}
	}
}

bool FWorldSeedEdMode::IsALandmarkSelected()
{
	//TArray<AActor*> ActorList;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWT_Landmark_Base::StaticClass(), ActorList);
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		if (AActor* LevelActor = Cast<AWT_Landmark_Base>(*Iter))
		{
			SelectedLandmark = Cast<AWT_Landmark_Base>(*Iter);
			CachedLandmarkPosition = SelectedLandmark->GetActorLocation();
			return true;
		}
	}

	return false;
}




