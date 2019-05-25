// Fill out your copyright notice in the Description page of Project Settings.

#include "FSMLiteComponent.h"

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

bool UFSMLiteComponent::InitFSMLite(int32 NumOfStates, uint8 InitialState)
{
	if (NumOfStates < 0 || NumOfStates > 255) {
		
		if(bPrintErrorsToLog)
			UE_LOG(LogTemp, Warning, TEXT("FSMLite.InitFSMLite(): invalid NumOfStates!"));
		
		return false;
	}
	else {
		this->NumOfStates = NumOfStates;
	}


	if (!IsValidState(InitialState)) {
		
		if(bPrintErrorsToLog)
			UE_LOG(LogTemp, Warning, TEXT("FSMLite.InitFSMLite(): invalid InitialState!"));
		
		return false;
	}
	else {
		//this->CurState = InitialState;
		SetState(InitialState);
	}

	
	IsInitialized = true;
	SetComponentTickEnabled(true);
	
	return true;
}

bool UFSMLiteComponent::SetState(uint8 State)
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

bool UFSMLiteComponent::IsValidState(uint8 State)
{
	if (NumOfStates > 0) {
		return (State >= 0 && State < this->NumOfStates);
	}
	
	return false;
}

uint8 UFSMLiteComponent::GetCurrentState(uint8 State)
{
	return CurState;
}

float UFSMLiteComponent::GetTime()
{
	return (GetOwner()->GetWorld()->GetTimeSeconds() - CurStateBeginTime);
}

