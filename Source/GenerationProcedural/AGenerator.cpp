// Fill out your copyright notice in the Description page of Project Settings.


#include "AGenerator.h"
#include "ARoom.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
//a faire
// avoir la possibilité qu'une des rooms ne spawn qu'une seule fois, avoir la possibilité de mettre des probabilité d'appartitions pour chaques room.
// Sets default values
AAGenerator::AAGenerator()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<UArrowComponent>(TEXT("Root"));
    SetRootComponent(Root);
}

// Called when the game starts or when spawned
void AAGenerator::BeginPlay()
{
    Super::BeginPlay();
    GenerateMap(numGeneration);
}

// Called every frame
void AAGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    int a = 1;
}



void AAGenerator::GenerateMap(int NumRoomGenerated)
{
    if (RoomConfigs.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No RoomConfigs specified!"));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("GenerateMap called. RoomConfigs count: %d , NumGeneration: %d"), RoomConfigs.Num(), NumRoomGenerated);

    TArray<AARoom*> SpawnedRooms;

    // 1. Ajoute le spawn de la première salle à la position de l'acteur
    FTransform FirstRoomSpawn = this->GetTransform();
    OpenExits.Add(FirstRoomSpawn);

    // 2. Génère les salles
    for (int i = 1; i < NumRoomGenerated && OpenExits.Num() > 0; ++i)
    {
        //choix de la sortie aleatoirement
        int ExitIdx = FMath::RandRange(0, OpenExits.Num() - 1);
        FTransform ChosenExitTransform = OpenExits[ExitIdx];

        //choix de la room aléatoirement
        int RoomIdx = PickRoomIndexByProbability();

        //spawn de la room
        AARoom* NewRoom = SpawnRoomAtExit(RoomIdx, ChosenExitTransform);
        SpawnedRooms.Add(NewRoom);
        OpenExits.RemoveAt(ExitIdx);
    }

    //verifie si bool bMustSpawn = true pour chaque salle et spawn à une exit si true 
    for (int idx = 0; idx < RoomConfigs.Num() && OpenExits.Num() > 0; ++idx)
    {
        if (RoomConfigs[idx].bMustSpawn)
        {
            bool HasSpawned = false;
            while (!HasSpawned) {
                int ExitIdx = FMath::RandRange(0, OpenExits.Num() - 1);
                FTransform ChosenExitTransform = OpenExits[ExitIdx];
                AARoom* NewRoom = SpawnRoomAtExit(idx, ChosenExitTransform);
                if (NewRoom)
                {
                    UE_LOG(LogTemp, Log, TEXT("Room forcée à spawn (bMustSpawn) : index %d, nom %s."), idx, *NewRoom->GetName());
                    RoomConfigs[idx].bMustSpawn = false;
                    HasSpawned = true;
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Echec du spawn pour la room bMustSpawn à l'index %d."), idx);
                }
                OpenExits.RemoveAt(ExitIdx);
                if (OpenExits.Num() == 0)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Plus d'exits disponibles pour les rooms bMustSpawn!"));
                    break;
                }
            }
        }
    }

    //verifie si bool bMustSpawn = true pour chaque salle et spawn à une exit si true 
    UE_LOG(LogTemp, Log, TEXT("Generation complete. Rooms spawned: %d"), SpawnedRooms.Num());
}

AARoom* AAGenerator::SpawnRoomAtExit(int RoomIdx, const FTransform& ExitTransform)
{
    //check index
    if (!RoomConfigs.IsValidIndex(RoomIdx)) {
        UE_LOG(LogTemp, Warning, TEXT("ivalid index %d!"), RoomIdx);
        return nullptr;
    }

    //room at index
    TSubclassOf<AARoom> RoomClass = RoomConfigs[RoomIdx].RoomClass;
    if (!RoomClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("RoomClass is null in RoomConfigs at index %d!"), RoomIdx);
        return nullptr;
    }
    AARoom* NewRoom = GetWorld()->SpawnActor<AARoom>(RoomClass, FTransform::Identity);
    NewRoom->SetActorTransform(ExitTransform);
    if (!NewRoom)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn room!"));
        return nullptr;
    }


    //check if overlapping whith another room
    if (IsRoomOverlapping(NewRoom))
    {
        NewRoom->Destroy();
        return nullptr;
    }
    else {
        AddNewExits(NewRoom);
    }

    //check si la room devait spawn forcement et si elle devait etre unique
    if (RoomConfigs[RoomIdx].bMustSpawn)
    {
        RoomConfigs[RoomIdx].bMustSpawn = false;
        UE_LOG(LogTemp, Log, TEXT("Room qui devait spawn a bien spawn (index %d)."), RoomIdx);
    }
    if (RoomConfigs[RoomIdx].bUnique)
    {
        RoomConfigs.RemoveAt(RoomIdx);
        UE_LOG(LogTemp, Log, TEXT("Room unique supprimée de RoomConfigs après spawn (index %d)."), RoomIdx);
    }
    return NewRoom;
}

void AAGenerator::AddNewExits(AARoom* Room) {
    TArray<USceneComponent*> Exits;
    Room->ExitsRoot->GetChildrenComponents(false, Exits);

    const FTransform& RoomTransform = Room->GetActorTransform();

    for (USceneComponent* Exit : Exits)
    {
        FTransform RelativeExitTransform = Exit->GetRelativeTransform();
        FTransform WorldExitTransform = RelativeExitTransform * RoomTransform;;

        OpenExits.Add(WorldExitTransform);
    }
}

bool AAGenerator::IsRoomOverlapping(AARoom* Room)
{
    if (!Room || !Room->CollisionBox) return true;
    TArray<AActor*> OverlappingActors;
    Room->CollisionBox->GetOverlappingActors(OverlappingActors, AARoom::StaticClass());
    OverlappingActors.Remove(Room); // On retire soi-même
    if (OverlappingActors.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Collision détectée pour la salle %s avec %d autres salles:"), *Room->GetName(), OverlappingActors.Num());
        for (AActor* Overlap : OverlappingActors)
        {
            UE_LOG(LogTemp, Warning, TEXT(" - %s"), *Overlap->GetName());
        }
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Aucune collision détectée pour la salle %s."), *Room->GetName());
    }
    return false;
}

int AAGenerator::PickRoomIndexByProbability()
{
    float TotalProb = 0.f;
    for (const FRoomConfig& Config : RoomConfigs)
    {
        TotalProb += Config.Probability;
    }
    if (TotalProb <= 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Total probability is zero or negative! Defaulting to index 0."));
        return 0;
    }
    float Rand = FMath::FRandRange(0.f, TotalProb);
    float Accum = 0.f;
    for (int i = 0; i < RoomConfigs.Num(); ++i)
    {
        Accum += RoomConfigs[i].Probability;
        if (Rand <= Accum)
        {
            return i;
        }
    }
    // Sécurité : retourne le dernier index si jamais
    return RoomConfigs.Num() - 1;
}