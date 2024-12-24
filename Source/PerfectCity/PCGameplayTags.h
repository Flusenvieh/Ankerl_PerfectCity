#pragma once

#include "NativeGameplayTags.h"

namespace PCGameplayTags
{
	PERFECTCITY_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Declare all custom native tags this project will use

	PERFECTCITY_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	PERFECTCITY_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	PERFECTCITY_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);
}
