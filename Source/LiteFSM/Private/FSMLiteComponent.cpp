// Fill out your copyright notice in the Description page of Project Settings.

#include "FSMLiteComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UFSMLiteComponent::UFSMLiteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	this->CurState = 0;
	this->NumOfStates = 0;
	this->bPrintErrorsToLog = true;
}

// Called when the game starts
void UFSMLiteComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

// Called every frame
void UFSMLiteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	OnTickState.Broadcast(CurState);
}

bool UFSMLiteComponent::InitFSMLite(const int32 NumberOfStates, const uint8 InitialState)
{
	if (NumberOfStates < 0 || NumberOfStates > 255) {
		
		if(bPrintErrorsToLog)
			UE_LOG(LogTemp, Warning, TEXT("FSMLite.InitFSMLite(): invalid NumberOfStates!"));
		
		return false;
	}
	this->NumOfStates = NumberOfStates;


	if (!IsValidState(InitialState)) {
		if(bPrintErrorsToLog) 
			UE_LOG(LogTemp, Warning, TEXT("FSMLite.InitFSMLite(): invalid InitialState!"));
		
		return false;
	}
	
	SetState(InitialState);
	IsInitialized = true;
	SetComponentTickEnabled(true);

	return true;
}

bool UFSMLiteComponent::SetState(const uint8 State)
{
	if (!IsValidState(State))
		return false;

	if (IsInitialized) {
		OnEndState.Broadcast(CurState);
	}

	CurState = State;
	CurStateBeginTime = GetOwner()->GetWorld()->GetTimeSeconds();	//save game time when cur state starts
	OnBeginState.Broadcast(CurState);
	return true;
}

bool UFSMLiteComponent::IsValidState(const uint8 State) const
{
	if (NumOfStates > 0) {
		return (State >= 0 && State < this->NumOfStates);
	}
	
	return false;
}

uint8 UFSMLiteComponent::GetCurrentState() const
{
	return CurState;
}

float UFSMLiteComponent::GetTime() const
{
	return (GetOwner()->GetWorld()->GetTimeSeconds() - CurStateBeginTime);
}

