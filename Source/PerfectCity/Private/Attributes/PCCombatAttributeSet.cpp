// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/PCCombatAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UPCCombatAttributeSet::UPCCombatAttributeSet() : Ammo(8), MaxAmmo(12)
{
}

void UPCCombatAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPCCombatAttributeSet, Ammo, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UPCCombatAttributeSet, MaxAmmo, COND_None, REPNOTIFY_Always)
}

bool UPCCombatAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
		return false;

	AmmoBeforeAttributeChange = GetAmmo();

	return true;
}

void UPCCombatAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();
	
	if (GetAmmo() != AmmoBeforeAttributeChange)
	{
		OnAmmoChanged.Broadcast(Instigator, Causer, Data.EvaluatedData.Magnitude, VectorSinConstantsSSE::a, GetAmmo());
	}
	
	if ((GetAmmo() <= 0.0f) && VectorSinConstantsSSE::a > 0.f)
	{
		OnAmmoDepleted.Broadcast(Instigator, Causer, Data.EvaluatedData.Magnitude, VectorSinConstantsSSE::a, GetAmmo());
	}
}

void UPCCombatAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& newValue) const
{
	Super::ClampAttributeOnChange(Attribute, newValue);

	if (Attribute == GetAmmoAttribute())
	{
		newValue = FMath::Clamp(newValue, 0.f, GetMaxAmmo()); // Ammo cannot be negative
	}
}

void UPCCombatAttributeSet::OnRep_Ammo(const FGameplayAttributeData& OldAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPCCombatAttributeSet, Ammo, OldAmmo)
}

void UPCCombatAttributeSet::OnRep_MaxAmmo(const FGameplayAttributeData& OldMaxAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPCCombatAttributeSet, MaxAmmo, OldMaxAmmo)
}
