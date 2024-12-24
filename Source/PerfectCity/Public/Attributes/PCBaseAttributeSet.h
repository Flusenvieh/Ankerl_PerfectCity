// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "PCBaseAttributeSet.generated.h"

DECLARE_MULTICAST_DELEGATE_FiveParams(FAttributeEvent, AActor* /*Effect Instigator*/, AActor* /*Effect Causer*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/)


/**
 * 
 */
UCLASS()
class PERFECTCITY_API UPCBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
protected:
	// Called before changes to base value are applied
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Called before changes to current value are applied
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// Function to clamp new value before base and/or current value are modified
	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& newValue) const;
};
