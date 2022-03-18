// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WT_GenerationStyle.generated.h"

/**
 * 
 */
UCLASS()
class WORLDSEED_API UWT_GenerationStyle : public UObject
{
	GENERATED_BODY()

public:


	UWT_GenerationStyle();

		 void GenerateStyle(class AWT_GeneratorCore* Core);

protected: 

	UFUNCTION(BlueprintImplementableEvent)
		void Run();


protected: 
	UPROPERTY()
		class AWT_GeneratorCore* GenCore;
	
};
