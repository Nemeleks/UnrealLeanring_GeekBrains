#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	ProjectileCannon = 0 UMETA(Display = "Fire Projectile"),
	TraceCannon = 1 UMETA(Display = "Fire Trace"),
	Machinegun = 2 UMETA(Display = "Machinegun")
};
