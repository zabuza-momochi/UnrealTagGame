// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Navigation/PathFollowingComponent.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	TagGameMode = Cast<ATagGameGameMode>(GetWorld()->GetAuthGameMode());

	ResetEnemy();
}

void AEnemyAIController::ResetEnemy()
{
	UE_LOG(LogTemp, Warning, TEXT("RESET ENEMY"));

	GetBlackboardComponent()->SetValueAsObject("Player", GetWorld()->GetFirstPlayerController()->GetPawn());

	//UE_LOG(LogTemp, Warning, TEXT("NOME BB: %s"), *GetBlackboardComponent()->GetValueAsObject("Player")->GetName());
	//GetBlackboardComponent()->ClearValue("BestBall");

	GoToPlayer = MakeShared<FAIVState>(
		[this](AAIController* AIController)
		{
			AIController->MoveToActor(GetActorFromBB("Player"), 100.0f);
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAIVState>
		{
			EPathFollowingStatus::Type State = AIController->GetMoveStatus();

			if (State == EPathFollowingStatus::Moving)
			{
				return nullptr;
			}

			if (GetActorFromBB("BestBall") && State == EPathFollowingStatus::Idle)
			{
				if (FVector::Distance(GetActorFromBB("Player")->GetActorLocation(), GetPawn()->GetActorLocation()) < 200.0f)
				{
					GetActorFromBB("BestBall")->AttachToActor(AIController->GetWorld()->GetFirstPlayerController()->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
					GetActorFromBB("BestBall")->SetActorRelativeLocation(FVector::ZeroVector);
					GetBlackboardComponent()->ClearValue("BestBall");
				}

				TagGameMode->SetCounter();
			}

			return SearchForBall;
		}
	);

	SearchForBall = MakeShared<FAIVState>(
		[this](AAIController* AIController)
		{
			const TArray<ABall*>& BallsList = TagGameMode->GetBalls();

			ABall* NearestBall = nullptr;

			for (int32 i = 0; i < BallsList.Num(); i++)
			{
				if (!BallsList[i]->GetAttachParentActor() && (!NearestBall || FVector::Distance(AIController->GetPawn()->GetActorLocation(), BallsList[i]->GetActorLocation())
					< FVector::Distance(AIController->GetPawn()->GetActorLocation(), NearestBall->GetActorLocation())))
				{
					NearestBall = BallsList[i];
				}
			}

			GetBlackboardComponent()->SetValueAsObject("BestBall", NearestBall);
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime)->TSharedPtr<FAIVState>
		{
			if (GetBlackboardComponent()->GetValueAsObject("BestBall"))
			{
				return GoToBall;
			}
			else
			{
				CurrentState->CallEnter(this);
				return nullptr;
			}
		}
	);

	GoToBall = MakeShared<FAIVState>(
		[this](AAIController* AIController)
		{
			AIController->MoveToActor(Cast<AActor>(GetBlackboardComponent()->GetValueAsObject("BestBall")), 50.0f);
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAIVState>
		{
			EPathFollowingStatus::Type State = AIController->GetMoveStatus();

			if (GetActorFromBB("BestBall")->GetAttachParentActor())
			{
				GetBlackboardComponent()->ClearValue("BestBall");

				return SearchForBall;
			}

			if (State == EPathFollowingStatus::Moving)
			{
				return nullptr;
			}

			return GrabBall;
		}
	);

	GrabBall = MakeShared<FAIVState>(
		[this](AAIController* AIController)
		{
			if (GetActorFromBB("BestBall")->GetAttachParentActor())
			{
				GetBlackboardComponent()->ClearValue("BestBall");
			}
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAIVState>
		{
			if (!GetActorFromBB("BestBall"))
			{
				return SearchForBall;
			}

			GetActorFromBB("BestBall")->AttachToActor(AIController->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
			GetActorFromBB("BestBall")->SetActorRelativeLocation(FVector::ZeroVector);

			return GoToPlayer;
		}
	);

	CurrentState = SearchForBall;
	CurrentState->CallEnter(this);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState)
	{
		CurrentState = CurrentState->CallTick(this, DeltaTime);
	}
}

AActor* AEnemyAIController::GetActorFromBB(const FString Name) const
{
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(*Name));
}

