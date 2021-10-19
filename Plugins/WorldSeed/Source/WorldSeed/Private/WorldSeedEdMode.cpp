// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldSeedEdMode.h"
#include "WorldSeedEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FWorldSeedEdMode::EM_WorldSeedEdModeId = TEXT("EM_WorldSeedEdMode");

FWorldSeedEdMode::FWorldSeedEdMode()
{

}

FWorldSeedEdMode::~FWorldSeedEdMode()
{

}

void FWorldSeedEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FWorldSeedEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FWorldSeedEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FWorldSeedEdMode::UsesToolkits() const
{
	return true;
}




