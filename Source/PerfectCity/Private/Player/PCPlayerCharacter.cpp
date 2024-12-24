// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PCPlayerCharacter.h"

#include "Logging/LogMacros.h"
#include "EnhancedInputComponent.h"
#include "Attributes/PCCombatAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

class UEnhancedInputLocalPlayerSubsystem;

APCPlayerCharacter::APCPlayerCharacter()
{
	// Enable replication
	bReplicates = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Deactivate using rotation from controller, should only apply to camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure Character movement
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	
	// Create SpringArm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 150.f;
	SpringArmComponent->SocketOffset = FVector(0.f, 50.f, 0.f);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	// Create CameraComponent	
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	ThirdPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	ThirdPersonCameraComponent->bUsePawnControlRotation = false;

	CombatAttributeSet = CreateDefaultSubobject<UPCCombatAttributeSet>("CombatAttributeSet");
}

void APCPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APCPlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APCPlayerCharacter::Look);

		for (UPCInputMapping* mapping : AbilityMappings)
		{
			for (auto ability : mapping->InputMappings)
			{
				if (ability.InputAction && ability.InputTag.IsValid())
				{
					EnhancedInputComponent->BindAction(ability.InputAction, ETriggerEvent::Triggered, this, &ThisClass::Input_AbilityInputTagPressed, ability.InputTag).GetHandle();
				}
			}
		}
	}
	else
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"), *GetNameSafe(this));
	}
}

void APCPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetAmmoAttribute()).AddUObject(this, &APCPlayerCharacter::OnAmmoAttributeChanged);
}

void APCPlayerCharacter::OnAmmoAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAmmoChanged(Data.OldValue, Data.NewValue);
}

void APCPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(PCGameplayTags::Status_Death_Dying))
	{
		return;
	}
	
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find forward rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APCPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APCPlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
		{
			AbilitySystemComponent->TryActivateAbility(AbilitySpec.Handle);
		}
	}
}