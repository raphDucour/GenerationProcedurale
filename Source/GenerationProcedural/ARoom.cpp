// Fill out your copyright notice in the Description page of Project Settings.


#include "ARoom.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h" // Added for CollisionBox

// Sets default values
AARoom::AARoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	// Crée l'entrée (flèche verte)
	EntryPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("EntryPoint"));
	EntryPoint->ArrowColor = FColor::Green;
	SetRootComponent(EntryPoint);

	ExitsRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ExitsRoot"));
	ExitsRoot->SetupAttachment(RootComponent);

	// Crée la racine des sorties
	FirstExit = CreateDefaultSubobject<UArrowComponent>(TEXT("FirstExit"));
	FirstExit->ArrowColor = FColor::Red;
	FirstExit->SetupAttachment(ExitsRoot);

	// Création et attachement de la box de collision
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(200.f, 200.f, 100.f)); // Taille par défaut, à ajuster selon la taille de la salle
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

// Called when the game starts or when spawned
void AARoom::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



