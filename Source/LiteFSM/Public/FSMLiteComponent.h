// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMLiteComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginState, uint8, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickState, uint8, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndState, uint8, State);


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LITEFSM_API UFSMLiteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSMLiteComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
private:
	int32 NumOfStates;
	uint8 CurState;
	float CurStateBeginTime;
	bool IsInitialized;

public:
	UFUNCTION(BlueprintCallable, Category="FSMLite")
	virtual bool InitFSMLite(const int32 NumberOfStates, const uint8 InitialState);

	UFUNCTION(BlueprintCallable, Category = "FSMLite")
	virtual bool SetState(const uint8 State);

	UFUNCTION(BlueprintPure, Category = "FSMLite")
	bool IsValidState(const uint8 State) const;

	UFUNCTION(BlueprintPure, Category = "FSMLite")
	uint8 GetCurrentState() const;

	UFUNCTION(BlueprintPure, Category = "FSMLite")
	float GetTime() const;
	
	UPROPERTY(BlueprintAssignable, Category = "FSMLite")
	FOnBeginState OnBeginState;

	UPROPERTY(BlueprintAssignable, Category = "FSMLite")
	FOnTickState OnTickState;

	UPROPERTY(BlueprintAssignable, Category = "FSMLite")
	FOnEndState OnEndState;
	
	UPROPERTY(BlueprintReadWrite, Category = "FSMLite")
	bool bPrintErrorsToLog;
};
