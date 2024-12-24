// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/PCAbilitySet.h"

#include "AbilitySystemComponent.h"
#include "PerfectCity/PCLogChannels.h"

TArray<FGameplayAbilitySpecHandle> UPCAbilitySet::GrantAbilitiesToAbilitySystem(
	UAbilitySystemComponent* AbilitySystemComponent) const
{
	check(AbilitySystemComponent)

	if (!AbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return TArray<FGameplayAbilitySpecHandle>();
	}
	
	TArray<FGameplayAbilitySpecHandle> Handles;
	Handles.Reserve(AbilitySetItems.Num());

	for (const auto AbilityItem : AbilitySetItems)
	{
		if (!IsValid(AbilityItem.GameplayAbility))
		{
			UE_LOG(LogPCAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%s] on ability set [%s] is not valid."), *AbilityItem.InputTag.ToString(), *GetNameSafe(this));
			continue;
		}
		
		FGameplayAbilitySpec AbilitySpec(AbilityItem.GameplayAbility->GetDefaultObject<UPCGameplayAbility>(), AbilityItem.AbilityLevel);
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityItem.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
		UE_LOG(LogPCAbilitySystem, Display, TEXT("GrantedGameplayAbilities[%s] on ability set [%s] granted."), *AbilityItem.InputTag.ToString(), *GetNameSafe(this));
		
		Handles.Add(AbilitySpecHandle);
	}

	return Handles;
}
