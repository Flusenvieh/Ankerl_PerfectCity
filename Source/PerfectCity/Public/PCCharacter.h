// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Attributes/PCHealthAttributeSet.h"
#include "Input/PCAbilitySet.h"
#include "PerfectCity/PCGameplayTags.h"
#include "PCCharacter.generated.h"

struct FOnAttributeChangeData;
class UPCHealthAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class PERFECTCITY_API APCCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	APCCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// "Interface" for accessing Gameplay Ability System, present for all player and enemy chara
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS", meta=(AllowPrivateAccess = true))
	UAbilitySystemComponent* AbilitySystemComponent;

	// Attributes that manage health, both for player and AI characters
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS", meta=(AllowPrivateAccess = true))
	UPCHealthAttributeSet* HealthAttributeSet;

	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GAS")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GAS")
	TArray<UPCAbilitySet*> DefaultAbilitySets;
	

	virtual void PostInitializeComponents() override;

	virtual void InitializeEffects();
	virtual void InitializeAbilities();

	//TODO: clear up callbacks
	
	virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);
	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, float DamageMagnitude, float OldValue, float NewValue);

	UFUNCTION(BlueprintImplementableEvent, Category="GAS|Health")
	void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, float DamageMagnitude, float OldValue, float NewValue);
	
	UFUNCTION(BlueprintImplementableEvent, Category="GAS|Health")
	void OnHealthChanged(float OldValue, float NewValue);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};



inline void APCCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!AbilitySystemComponent)
		return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	HealthAttributeSet->OnHealthDepleted.AddUObject(this, &ThisClass::HandleOutOfHealth);
	HealthAttributeSet->OnHealthChanged.AddUObject(this, &APCCharacter::HandleHealthChanged);
	
	InitializeEffects();

	InitializeAbilities();
}
