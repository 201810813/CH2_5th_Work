// Fill out your copyright notice in the Description page of Project Settings.
#include "MyActor.h"
#include "DSP/AudioDebuggingUtilities.h"


// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	totDist = 0;
	evCnt = 0;
	start.Add({0.0,0.0});
	moveCount = 0;
	maxStep = 10;
	singleDist = 0;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("시작 위치: %f, %f"),start[moveCount].X,start[moveCount].Y);
	for (int32 i = 0; i < maxStep; ++i)
	{
		Move();
		singleDist = Distance(start[moveCount], start[moveCount-1]);
		totDist += singleDist;
		UE_LOG(LogTemp, Log, TEXT("%i회차 이동 거리: %f"),i+1, singleDist);
	}
	UE_LOG(LogTemp, Log, TEXT("총합 이동 거리: %f"),totDist);
	UE_LOG(LogTemp, Log, TEXT("이벤트 발동 횟수: %i"),evCnt);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//걸음 수 리턴.
int32 AMyActor::Step()
{
	
	int32 step = FMath::RandRange(0,1);
	return step;
}

//
void AMyActor::Move()
{
	int32 eventStep = CreateEvent();//케이스 마다 값을 받아옴.
	start.Add({start[moveCount].X+Step()+eventStep,start[moveCount].Y+Step()+eventStep});
	moveCount++;
	UE_LOG(LogTemp, Warning, TEXT("%i회차 위치. x: %f y: %f"),moveCount,start[moveCount].X,start[moveCount].Y);
	
	
}

//switch-case문을 활용한 이벤트 생성 함수
//Move함수의 지역변수로 리턴.
int32 AMyActor::CreateEvent()
{
	int32 select = FMath::RandRange(1,3); //case에 넣을 변수.
	int32 eventProbability = FMath::RandRange(0,100); 
	int32 triggerProbability = 50;
	if (eventProbability > triggerProbability)//랜덤 값이 50을 초과하면
	{
		switch (select)
		{
		case 1:
			UE_LOG(LogTemp, Error, TEXT("당신은 함정에 걸렸습니다 x,y축 1칸 씩 추가로 빠집니다."));
			evCnt++;
			return -1;
		case 2:
			UE_LOG(LogTemp, Error, TEXT("걷기의 여신이 당신을 응원합니다. 최대 걸음 수가 1증가합니다."));
			evCnt++;
			maxStep++;
			return 0;
		case 3:
			UE_LOG(LogTemp, Error, TEXT("뒷풍이 붑니다 x,y축으로 1칸 씩 추가로 전진합니다."));
			evCnt++;
			return 1;	
		default:
			return 0;
		}
	}
	else// 그렇지 않으면 0리턴 
	{
		UE_LOG(LogTemp, Log, TEXT("이번엔 아무일도 없었습니다."));
		return 0;
	}
}

float AMyActor::Distance(FVector2d first, FVector2d second)
{
	float distance = sqrt(FMath::Pow(first.X - second.X, 2) + FMath::Pow(first.Y - second.Y, 2));
	return distance;
}