// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCAbilityCost.h"
#include "Abilities/GameplayAbility.h"
#include "PCGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class PERFECTCITY_API UPCGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	
	// Additional costs that must be paid to activate this ability, as base class only supports one cost class per ability and lacks handling
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Costs)
	TArray<TObjectPtr<UPCAbilityCost>> AdditionalCosts;

public:
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
};
