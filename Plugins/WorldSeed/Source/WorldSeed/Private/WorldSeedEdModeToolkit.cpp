// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldSeedEdModeToolkit.h"
#include "WorldSeedEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FWorldSeedEdModeToolkit"

FWorldSeedEdModeToolkit::FWorldSeedEdModeToolkit()
{
}

void FWorldSeedEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		static FReply OnButtonClick(FVector InOffset)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, InOffset);
		}

	};


	struct LandmarkSelector
	{
		FReply SetActiveLandmarkType()
		{
			//LandmarkTypes_Active;
		}

		


	};

	const float Factor = 256.0f;



	Options = {
		LOCTEXT("Slice", "Slice"),
			LOCTEXT("Add", "Add"),
			LOCTEXT("Remove", "Remove"),
			LOCTEXT("Room", "Room"),
			LOCTEXT("Corridor", "Corridor"),
	};
	
	LandmarkTypes_SelectedTitle = MakeShareable(new FText(Options[0]));
	for (const auto& ActiveOption : Options)
	{
		LandmarkTypes_DropDownOptions.Add(MakeShareable(new FText(ActiveOption)));
	}
	TSharedPtr<SComboBox<TSharedPtr<FText>>> LandmarkOptions_ComboBox;

	
	
		

	SAssignNew(ToolkitWidget, SBorder).HAlign(HAlign_Left).Padding(25)
		[
			SNew(SVerticalBox) + SVerticalBox::Slot()
			[
				SNew(SBorder)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().MaxHeight(50)
					[
						SNew(SHorizontalBox) + SHorizontalBox::Slot().AutoWidth().MaxWidth(275)
						[
							SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("TitleLabel", "Edit the properties beneath or select a landmark in the scene"))
						]
					]
					+ SVerticalBox::Slot().Padding(0, 0).MaxHeight(25)
					[
						SNew(SHorizontalBox) + SHorizontalBox::Slot().AutoWidth()
						[
							SAssignNew(LandmarkOptions_ComboBox, SComboBox<TSharedPtr<FText>>).InitiallySelectedItem(LandmarkTypes_SelectedTitle).OptionsSource(&LandmarkTypes_DropDownOptions)
								.OnSelectionChanged_Lambda([this](TSharedPtr<FText>NewSelection, ESelectInfo::Type SelectInfo) { LandmarkTypes_SelectedTitle = NewSelection; })
								.OnGenerateWidget_Lambda([](TSharedPtr<FText>Option) {return SNew(STextBlock).Font(FCoreStyle::GetDefaultFontStyle("Regular", 11)).Text(*Option); })
								[
									SNew(STextBlock).Font(FCoreStyle::GetDefaultFontStyle("Regular", 11)).Text_Lambda([this]() { return LandmarkTypes_SelectedTitle.IsValid() ? *LandmarkTypes_SelectedTitle : FText::GetEmpty(); })
								]
						]
						+ SHorizontalBox::Slot().Padding(5, 0).AutoWidth()
						[
							SNew(SButton).OnClicked(this, &FWorldSeedEdModeToolkit::SetLandmark)
						[
						SNew(STextBlock).Font(FCoreStyle::GetDefaultFontStyle("Regular", 11))
						.Text(LOCTEXT("ButtonName", "Generate Landmark"))
						]
					]

				]
			]
		]
	+SVerticalBox::Slot()
		[
			SNew(SBorder)
			[
				SNew(SVerticalBox) + SVerticalBox::Slot()
				[
					SNew(SHorizontalBox) + SHorizontalBox::Slot().AutoWidth().MaxWidth(275)
					[
						SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("TitleLabel", "Setup the size of the grid here"))
					]
				]
				+SVerticalBox::Slot()
				[
					SNew(SHorizontalBox) + SHorizontalBox::Slot()
					[
						SNew()
					]
					+SHorizontalBox::Slot()
					[
						SAssignNew(DataEntry_ChunkScale_Y, SNumericEntryBox)
					]
				]
	
			]
		]
	

			
		];
	
	




		/*
		* .HAlign(HAlign_Center)
		.Padding(25)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(50)
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "Select some actors and move them around using buttons below"))
			]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("UpButtonLabel", "Up"), FVector(0, 0, Factor))
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						Locals::MakeButton(LOCTEXT("LeftButtonLabel", "Left"), FVector(0, -Factor, 0))
					]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							Locals::MakeButton(LOCTEXT("RightButtonLabel", "Right"), FVector(0, Factor, 0))
						]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("DownButtonLabel", "Down"), FVector(0, 0, -Factor))
				]
		*/
		
		



		
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FWorldSeedEdModeToolkit::GetToolkitFName() const
{
	return FName("Level Designer");
}

FText FWorldSeedEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("WorldSeed_EditorToolkit", "DisplayName", "WorldSeed Tool");
}

class FEdMode* FWorldSeedEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FWorldSeedEdMode::EM_WorldSeedEdModeId);
}

FReply FWorldSeedEdModeToolkit::SetLandmark()
{

	int ResulingIndex = 0;
	for (int i = 0; i < Options.Num(); i++)
	{
		if ((FString)Options[i].ToString() == *LandmarkTypes_SelectedTitle.Get()->ToString())
		{
			ResulingIndex = i;
			break;
		}
	}

	TSubclassOf<AWT_Landmark_Base> LandmarkType;

	switch (ResulingIndex)
	{
	case 0:
		LandmarkType = AWT_Landmark_Base::StaticClass();

		break;
	case 1:
		LandmarkType = AWT_Landmark_Base::StaticClass();
		break;
	case 2:
		LandmarkType = AWT_Landmark_Base::StaticClass();
		break;
	}


	
	GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));
	//FWorldSeedEdMode* EditorMode = Cast<FWorldSeedEdMode>(GetEditorMode());
	EditorReference->CreateLandmark(LandmarkType);

	GEditor->EndTransaction();



	return FReply::Handled();
}



#undef LOCTEXT_NAMESPACE




/*
[
	SNew(SBorder)
	.BorderImage(InArgs._Background)
	.Padding(FMargin(20))
	[
		SNew(SVerticalBox) + SVerticalBox::Slot().AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
		SAssignNew(TitleComboBox, SComboBox<TSharedPtr<FText>>)
			.InitiallySelectedItem(SelectedTitle)
			.OptionsSource(&TitleOptions)
			.OnSelectionChanged_Lambda([this](TSharedPtr<FText> NewSelection, ESelectInfo::Type SelectInfo)
			{
			SelectedTitle = NewSelection;
			})
				.OnGenerateWidget_Lambda([](TSharedPtr<FText> Option)
				{
					return SNew(STextBlock)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 18))
						.Text(*Option);
				})
				[
				SNew(STextBlock)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 18))
			.Text_Lambda([this]()
				{
					return SelectedTitle.IsValid() ? *SelectedTitle : FText::GetEmpty();
				})
				]
	]
+ SHorizontalBox::Slot()
.Padding(5, 0)
[
	SAssignNew(NameBox, SEditableTextBox)
	.Font(FCoreStyle::GetDefaultFontStyle("Regular", 18))
	.HintText(LOCTEXT("YourName", "Fill your name"))
]
+ SHorizontalBox::Slot()
.Padding(5, 0)
[
	SNew(SButton)
	.OnClicked(this, &SGreetings::Greet)
	[
		SNew(STextBlock)
		.Font(FCoreStyle::GetDefaultFontStyle("Regular", 18))
	.Text(LOCTEXT("Hello", "Hello!"))
	]
]
	]
+ SVerticalBox::Slot()
.Padding(0, 10)
[
	SAssignNew(GreetBox, STextBlock)
	.Font(FCoreStyle::GetDefaultFontStyle("Regular", 18))
]
	]
];
*/