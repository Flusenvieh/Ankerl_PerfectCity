// Fill out your copyright notice in the Description page of Project Settings.

#include "PCCharacter.h"

#include "AbilitySystemComponent.h"
#include "Attributes/PCHealthAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
APCCharacter::APCCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");

	HealthAttributeSet = CreateDefaultSubobject<UPCHealthAttributeSet>("HealthAttributeSet");
}

// Called when the game starts or when spawned
void APCCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddUObject(this, &APCCharacter::OnHealthAttributeChanged);
}

UAbilitySystemComponent* APCCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APCCharacter::InitializeEffects()
{
	if (!AbilitySystemComponent)
		return;

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect>& Effect : DefaultEffects)
	{
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContextHandle);
		if (GameplayEffectSpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
		}
	}
}

void APCCharacter::InitializeAbilities()
{
	if (!AbilitySystemComponent)
		return;

	for (auto Set : DefaultAbilitySets)
	{
		if (!IsValid(Set))
		{
			continue;
		}

		Set->GrantAbilitiesToAbilitySystem(AbilitySystemComponent);
	}
}

void APCCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

// Called every frame
void APCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCCharacter::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser,
	float DamageMagnitude, float OldValue, float NewValue)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AddReplicatedLooseGameplayTag(PCGameplayTags::Status_Death_Dying);
	}
}