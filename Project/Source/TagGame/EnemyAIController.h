// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "Ball.h"
#include "TagGameGameMode.h"
#include "EnemyAIController.generated.h"

struct FAIVState : public TSharedFromThis<FAIVState>
{
private:
	TFunction<void(AAIController*)> Enter;
	TFunction<void(AAIController*)> Exit;
	TFunction<TSharedPtr<FAIVState>(AAIController*, const float)> Tick;

public:
	FAIVState() {
		Enter = nullptr;
		Exit = nullptr;
		Tick = nullptr;
	}

	FAIVState(const FAIVState& Other) = delete;
	FAIVState& operator =(const FAIVState& Other) = delete;
	FAIVState(FAIVState&& Other) = delete;
	FAIVState& operator =(FAIVState&& Other) = delete;

	FAIVState(TFunction<void(AAIController*)> InEnter, TFunction<void(AAIController*)> InExit, TFunction<TSharedPtr<FAIVState>(AAIController*, const float)> InTick)
	{
		Enter = InEnter;
		Exit = InExit;
		Tick = InTick;
	}

	void CallEnter(AAIController* AIController)
	{
		if (Enter)
		{
			Enter(AIController);
		}
	}
	void CallExit(AAIController* AIController)
	{
		if (Exit)
		{
			Exit(AIController);
		}
	}

	TSharedPtr<FAIVState> CallTick(AAIController* AIController, const float DeltaTime)
	{
		if (Tick)
		{
			TSharedPtr<FAIVState> NewTick = Tick(AIController, DeltaTime);

			if (NewTick != nullptr && NewTick != AsShared())
			{
				CallExit(AIController);
				NewTick->CallEnter(AIController);

				return NewTick;
			}
		}

		return AsShared();
	}
};

UCLASS()
class TAGGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	TSharedPtr<FAIVState> CurrentState;
	TSharedPtr<FAIVState> GoToPlayer;
	TSharedPtr<FAIVState> SearchForBall;
	TSharedPtr<FAIVState> GoToBall;
	TSharedPtr<FAIVState> GrabBall;

public:
	void BeginPlay() override;
	void ResetEnemy();
	void Tick(float DeltaTime) override;

	AActor* GetActorFromBB(const FString Name) const;
	ATagGameGameMode* TagGameMode;

	AEnemyAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
