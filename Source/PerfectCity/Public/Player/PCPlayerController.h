// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PCPlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class PERFECTCITY_API APCPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input Mapping Context to be used for player input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;
};
