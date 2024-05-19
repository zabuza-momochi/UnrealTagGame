// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Ball.h"
#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/GameModeBase.h"
#include "TagGameGameMode.generated.h"

UCLASS(minimalapi)
class ATagGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	TArray<ATargetPoint*> TargetPoints;
	TArray<ABall*> GameBalls;

	int32 BallCounter;

public:
	ATagGameGameMode();

	void BeginPlay() override;
	void ResetMatch();
	void SetCounter();

	const int32& GetCounter() const;
	const TArray<ABall*>& GetBalls() const;
};



