// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "PCAbilityCost.generated.h"

class UPCGameplayAbility;

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class PERFECTCITY_API UPCAbilityCost : public UObject
{
	GENERATED_BODY()

public:
	
	virtual bool CheckCost(const UPCGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
	{
		return true;
	}

	virtual void ApplyCost(const UPCGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
	{
	}
};
