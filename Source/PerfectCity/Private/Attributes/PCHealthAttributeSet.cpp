// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/PCHealthAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UPCHealthAttributeSet::UPCHealthAttributeSet(): Health(150.f), MaxHealth(200.f)
{

}

void UPCHealthAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPCHealthAttributeSet, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UPCHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)
}

bool UPCHealthAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}
	
	HealthBeforeAttributeChange = GetHealth();

	return true;
}

void UPCHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Convert into -Health and then clamp
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0.f, GetMaxHealth()));
		SetDamage(0.f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Convert into +Health and then clamp
		SetHealth(FMath::Clamp(GetHealth() + GetHealing(), 0.f, GetMaxHealth()));
		SetHealing(0.0f);
	}

	if (GetHealth() != HealthBeforeAttributeChange)
	{
		OnHealthChanged.Broadcast(Instigator, Causer, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
	}
	
	if ((GetHealth() <= 0.0f) && HealthBeforeAttributeChange > 0.f)
	{
		OnHealthDepleted.Broadcast(Instigator, Causer, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
	}
}

void UPCHealthAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& newValue) const
{
	Super::ClampAttributeOnChange(Attribute, newValue); //does nothing, but call just in case

	if (Attribute == GetHealthAttribute())
	{
		newValue = FMath::Clamp(newValue, 0.f, GetMaxHealth()); // Health cannot be negative
	}
}


void UPCHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPCHealthAttributeSet, Health, OldHealth);
}

void UPCHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPCHealthAttributeSet, MaxHealth, OldMaxHealth);
}
