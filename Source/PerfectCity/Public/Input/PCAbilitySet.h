// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbility/PCGameplayAbility.h"
#include "PCAbilitySet.generated.h"

USTRUCT()
struct FAbilitySetItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<UPCGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	int AbilityLevel;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	FGameplayTag InputTag;
};

UCLASS()
class PERFECTCITY_API UPCAbilitySet : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TArray<FAbilitySetItem> AbilitySetItems;
	
	TArray<FGameplayAbilitySpecHandle> GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const;
};
