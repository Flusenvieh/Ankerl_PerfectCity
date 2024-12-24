// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCCharacter.h"
#include "Input/PCInputMapping.h"
#include "Logging/LogMacros.h"
#include "PCPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UPCCombatAttributeSet;

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, Log, All);

/**
 * 
 */
UCLASS()
class PERFECTCITY_API APCPlayerCharacter : public APCCharacter
{
	GENERATED_BODY()

public:
	// Constructor, setup components
	APCPlayerCharacter();
	
protected:
	
	// Third person camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCameraComponent;

	// Springarm for Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	// Move Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// Look Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TArray<UPCInputMapping*> AbilityMappings;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void PostInitializeComponents() override;

	// Attributes that manage ammo, currently for player only
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS", meta=(AllowPrivateAccess = true))
	UPCCombatAttributeSet* CombatAttributeSet;

	virtual void OnAmmoAttributeChanged(const FOnAttributeChangeData& Data);
	UFUNCTION(BlueprintImplementableEvent, Category="GAS|Combat")
	void OnAmmoChanged(float OldValue, float NewValue);
	
	// Called for movement input
	void Move(const FInputActionValue& Value);

	// Called for looking input
	void Look(const FInputActionValue& Value);

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
};
