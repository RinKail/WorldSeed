// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldSeedEdMode.h"
#include "WorldSeedEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "WorldSeed/Public/WT_GeneratorCore.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Exited WorldSeed Editor"));

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

void FWorldSeedEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{

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




