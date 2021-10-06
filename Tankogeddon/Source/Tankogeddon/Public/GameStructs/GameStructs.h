#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireProjectiles = 0 UMETA(Display = "Fire Projectile"),
	FireTrace = 1 UMETA(Display = "Fire Trace")
};
