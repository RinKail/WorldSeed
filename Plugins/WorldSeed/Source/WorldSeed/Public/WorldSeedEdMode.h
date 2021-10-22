// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorldSeed/public/WT_Landmark_Base.h"
#include "EdMode.h"

class FWorldSeedEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_WorldSeedEdModeId;
public:
	FWorldSeedEdMode();
	virtual ~FWorldSeedEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface


	void CreateLandmark(TSubclassOf<AWT_Landmark_Base> Class);

	void GenerateGrid(int GridX, int GridY, int ChunkX, int ChunkY);


	bool bMovingLandmark;

	bool IsALandmarkSelected();


	FVector CachedLandmarkPosition;
	AWT_Landmark_Base* SelectedLandmark;

	UPROPERTY()
	class AWT_GeneratorCore* ActiveGenerator;


	UPROPERTY()
	TArray<AWT_Landmark_Base*> Landmark_List;


};
