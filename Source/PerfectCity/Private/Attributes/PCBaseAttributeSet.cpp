// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/PCBaseAttributeSet.h"

void UPCBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttributeOnChange(Attribute, NewValue);
}

void UPCBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttributeOnChange(Attribute, NewValue);
}

void UPCBaseAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& newValue) const
{
}
