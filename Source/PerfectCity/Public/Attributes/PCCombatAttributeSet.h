// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PCAttributeSetHelpers.h"
#include "Attributes/PCBaseAttributeSet.h"
#include "PCCombatAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PERFECTCITY_API UPCCombatAttributeSet : public UPCBaseAttributeSet
{
	GENERATED_BODY()

public:
	UPCCombatAttributeSet();
	
	ATTRIBUTE_ACCESSORS(UPCCombatAttributeSet, Ammo);
	ATTRIBUTE_ACCESSORS(UPCCombatAttributeSet, MaxAmmo);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	mutable FAttributeEvent OnAmmoChanged;
	mutable FAttributeEvent OnAmmoDepleted;

protected:
	
	// Current Ammo Value, capped by MaxAmmo attribute. Hidden from modifiers to only allow executions to modify
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ammo, Category="Attributes|Ammo", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Ammo;
	
	// Maximum Ammo Value
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxAmmo, Category="Attributes|Ammo", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData MaxAmmo;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& newValue) const override;

	UFUNCTION()
	virtual void OnRep_Ammo(const FGameplayAttributeData& OldAmmo);

	UFUNCTION()
	virtual void OnRep_MaxAmmo(const FGameplayAttributeData& OldMaxAmmo);

	// Store  ammo before any changes 
	float AmmoBeforeAttributeChange;
};
