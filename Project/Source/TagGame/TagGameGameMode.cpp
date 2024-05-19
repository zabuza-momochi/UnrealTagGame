// Copyright Epic Games, Inc. All Rights Reserved.

#include "TagGameGameMode.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "TagGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATagGameGameMode::ATagGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATagGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	ResetMatch();
}

void ATagGameGameMode::ResetMatch()
{
	TargetPoints.Empty();

	for (TActorIterator<ATargetPoint> Points(GetWorld()); Points; ++Points)
	{
		TargetPoints.Add(*Points);
	}

	GameBalls.Empty();

	for (TActorIterator<ABall> Ball(GetWorld()); Ball; ++Ball)
	{
		if (Ball->GetAttachParentActor())
		{
			Ball->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		}

		GameBalls.Add(*Ball);

		if (TargetPoints.Num() > 0) 
		{
			int32 RandomIndex = FMath::RandRange(0, TargetPoints.Num() - 1);

			Ball->SetActorLocation(TargetPoints[RandomIndex]->GetActorLocation());
			TargetPoints.RemoveAt(RandomIndex);
		}
	}

	BallCounter = 0;
}

void ATagGameGameMode::SetCounter()
{
	BallCounter++;

	if (BallCounter >= GameBalls.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("RESET MATCH"));
		ResetMatch();
	}
}

const int32& ATagGameGameMode::GetCounter() const
{
	return BallCounter;
}

const TArray<ABall*>& ATagGameGameMode::GetBalls() const
{
	// TODO: insert return statement here
	return GameBalls;
}

