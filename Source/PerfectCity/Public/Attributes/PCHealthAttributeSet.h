// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCBaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PCAttributeSetHelpers.h"
#include "PCHealthAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PERFECTCITY_API UPCHealthAttributeSet : public UPCBaseAttributeSet
{
	GENERATED_BODY()

public:
	UPCHealthAttributeSet();
	
	ATTRIBUTE_ACCESSORS(UPCHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UPCHealthAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UPCHealthAttributeSet, Healing);
	ATTRIBUTE_ACCESSORS(UPCHealthAttributeSet, Damage);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	mutable FAttributeEvent OnHealthChanged;
	mutable FAttributeEvent OnHealthDepleted;

protected:
	
	// Current Health Value, capped by MaxHealth attribute. Hidden from modifiers to only allow executions to modify
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category="Attributes|Health", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	
	// Maximum Health Value
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category="Attributes|Health", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	
	// META ATTRIBUTES (no state, no replication)
	
	// Incoming healing. This is mapped directly to +Health
	UPROPERTY(BlueprintReadOnly, Category="Attributes|Health", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Healing;
	
	// Incoming healing. This is mapped directly to -Health
	UPROPERTY(BlueprintReadOnly, Category="Attributes|Health", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Damage;


	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& newValue) const override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	// Store the health before any changes 
	float HealthBeforeAttributeChange;
};


