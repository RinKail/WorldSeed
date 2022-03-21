// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_GenerationStyle.h"
#include "WorldSeed/Public/WT_GeneratorCore.h"



UWT_GenerationStyle::UWT_GenerationStyle()
{
	UE_LOG(LogTemp, Warning, TEXT("Normal Type instantiated"));
}

void UWT_GenerationStyle::GenerateStyle(AWT_GeneratorCore* Core)
{

	GenCore = Core;


	UE_LOG(LogTemp, Warning, TEXT("Running Style"));

	Run();

	RunBackend();
}

void UWT_GenerationStyle::RunBackend()
{
	UE_LOG(LogTemp, Warning, TEXT("Running base backend"));
}
