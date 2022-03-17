// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"

#define TileScale 150
/**
 * 
 */

UENUM()
enum class EWT_TileID : uint8
{
	ID_Empty		   UMETA(DisplayName = "Empty"),
	ID_Raised		   UMETA(DisplayName = "Raised"),
	ID_Floor		   UMETA(DisplayName = "Floor"),
	ID_Edge			   UMETA(DisplayName = "Edge"),
	ID_InnerCorner     UMETA(DisplayName = "Inner Corner"),
	ID_OuterCorner     UMETA(DisplayName = "Outer Corner")
	

};


UENUM()
enum class EWT_GeomID : uint8
{
	ID_Empty		   UMETA(DisplayName = "Empty"),
	ID_Floor		   UMETA(DisplayName = "Floor"),
	ID_Raised		   UMETA(DisplayName = "Raised"),

	ID_Wall			   UMETA(DisplayName = "Wall"),
	ID_WallCorner	   UMETA(DisplayName = "Wall Corner"),
	ID_ThinWall		   UMETA(DisplayName = "Thin Wall"),
	ID_ThinWallEnd	   UMETA(DisplayName = "Thin Wall End"),

	ID_InnerCorner     UMETA(DisplayName = "Inner Corner"),
	ID_OuterCorner     UMETA(DisplayName = "Outer Corner"),
	ID_ThinCorner      UMETA(DisplayName = "Thin Corner"),

	ID_TConnector      UMETA(DisplayName = "T Connector"),
	ID_XConnector      UMETA(DisplayName = "X Connector"),
	ID_Block      UMETA(DisplayName = "X Connector")


};

UENUM()
enum class EWT_StackID : uint8
{

	ID_Bottom		 UMETA(DisplayName = "Bottom"),
	ID_Mid		 UMETA(DisplayName = "Mid"),
	ID_Top			 UMETA(DisplayName = "Top"),
	ID_Single			 UMETA(DisplayName = "Single")
		

};

UENUM()
enum class EWT_SpaceID : uint8
{
	ID_Null		 UMETA(DisplayName = "Null"),
	ID_Empty		 UMETA(DisplayName = "Empty"),
	ID_Edge			 UMETA(DisplayName = "Floor"),
	ID_Floor		 UMETA(DisplayName = "Wall"),
	ID_Raised		 UMETA(DisplayName = "Raised")


};



