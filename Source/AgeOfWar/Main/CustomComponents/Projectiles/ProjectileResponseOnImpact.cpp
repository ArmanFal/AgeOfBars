// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "ProjectileResponseOnImpact.h"

class UWorld* UProjectileResponseOnImpact::GetWorld() const
{
	return World.Get();
}
