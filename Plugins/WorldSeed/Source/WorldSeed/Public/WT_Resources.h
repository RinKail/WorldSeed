// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#define TileScale 150
/**
 * 
 */

UENUM(BlueprintType)
enum EWT_TileType
{
	TT_Empty     UMETA(DisplayName = "Empty"),
	TT_Floor     UMETA(DisplayName = "Empty"),
	TT_Edge		 UMETA(DisplayName = "Empty"),
	TT_Raised    UMETA(DisplayName = "Empty"),
	TT_Corner    UMETA(DisplayName = "Empty"),

};

UENUM(BlueprintType)
enum EWT_TileID
{
	TI_Empty        UMETA(DisplayName = "Empty"),
	TI_Raised        UMETA(DisplayName = "Raised"),
	TI_Edge			 UMETA(DisplayName = "Edge"),
	TI_InnerCorner   UMETA(DisplayName = "InnerCorner"),
	TI_OuterCorner   UMETA(DisplayName = "OuterCorner"),
	TI_Floor		 UMETA(DisplayName = "Floor"),
	

};

UENUM(BlueprintType)
enum EWT_TileDirection
{
	TD_None				UMETA(DisplayName = "None"),
	TD_Up				UMETA(DisplayName = "Up"),
	TD_Down				UMETA(DisplayName = "Down"),
	TD_Left				UMETA(DisplayName = "Left"),
	TD_Right			UMETA(DisplayName = "Right"),

	TD_UpDown			UMETA(DisplayName = "UpDown"),
	TD_LeftRight		UMETA(DisplayName = "LeftRight"),

	TD_LeftUpRight      UMETA(DisplayName = "LeftUpRight"),
	TD_LeftDownRight    UMETA(DisplayName = "LeftDownRight"),
	TD_UpLeftDown       UMETA(DisplayName = "UpLeftDown"),
	TD_UpRightDown      UMETA(DisplayName = "UpRightDown"),

	TD_All				UMETA(DisplayName = "All"),



};



