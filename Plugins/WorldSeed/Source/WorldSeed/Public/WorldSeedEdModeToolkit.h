// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "WorldSeed/public/WT_Landmark_Base.h"

class FWorldSeedEdModeToolkit : public FModeToolkit
{
public:

	FWorldSeedEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }



	FReply SetLandmark();

private:
	TArray<FText> Options;
	

	int Result;

	TSharedPtr<SWidget> ToolkitWidget;

	EWT_LandMarkType LandmarkTypes_Active;

	TArray<TSharedPtr<FText>> LandmarkTypes_DropDownOptions;
	TSharedPtr<FText> LandmarkTypes_SelectedTitle;

};
