// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GenerationStyle.h"
#include "WorldSeed/Public/WT_GeneratorCore.h"



UWT_GenerationStyle::UWT_GenerationStyle()
{

}

void UWT_GenerationStyle::GenerateStyle(AWT_GeneratorCore* Core)
{

	GenCore = Core;

	Run();
}
