// Fill out your copyright notice in the Description page of Project Settings.


#include "WT_LandmarkAnchor_Base.h"
#include "Components/BillboardComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AWT_LandmarkAnchor_Base::AWT_LandmarkAnchor_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	
	
	AnchorIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("Icon"));

	SetRootComponent(AnchorIcon);


}

// Called when the game starts or when spawned
void AWT_LandmarkAnchor_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWT_LandmarkAnchor_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

