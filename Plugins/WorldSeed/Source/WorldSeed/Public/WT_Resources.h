// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#define TileScale 150
/**
 * 
 */

UENUM(BlueprintType)
enum class EWT_TileType : uint8
{
	TT_Empty     UMETA(DisplayName = "Empty"),
	TT_Floor     UMETA(DisplayName = "Empty"),
	TT_Edge     UMETA(DisplayName = "Empty"),
	TT_Raised     UMETA(DisplayName = "Empty"),
	TT_Corner     UMETA(DisplayName = "Empty"),

};

UENUM(BlueprintType)
enum class EWT_TileID : uint8
{
	TI_Raised        UMETA(DisplayName = "Empty"),
	TI_Edge			 UMETA(DisplayName = "Empty"),
	TI_InnerCorner   UMETA(DisplayName = "Empty"),
	TI_OuterCorner   UMETA(DisplayName = "Empty"),
	TI_Floor		 UMETA(DisplayName = "Empty"),
	

};

UENUM(BlueprintType)
enum class EWT_TileDirection : uint8
{
	TD_Up        UMETA(DisplayName = "Empty"),
	TD_Down        UMETA(DisplayName = "Empty"),
	TD_Left       UMETA(DisplayName = "Empty"),
	TD_Right        UMETA(DisplayName = "Empty"),

	TD_UpDown       UMETA(DisplayName = "Empty"),
	TD_LeftRight       UMETA(DisplayName = "Empty"),

	TD_LeftUpRight       UMETA(DisplayName = "Empty"),
	TD_LeftDownRight      UMETA(DisplayName = "Empty"),
	TD_UpLeftDown       UMETA(DisplayName = "Empty"),
	TD_UpRightDown      UMETA(DisplayName = "Empty"),

	TD_All      UMETA(DisplayName = "Empty"),



};


