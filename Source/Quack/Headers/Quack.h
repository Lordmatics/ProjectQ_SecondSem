// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#ifndef __QUACK_H__
#define __QUACK_H__

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::White,text)

/* Stencil index mapping to PP_OutlineColored */
#define STENCIL_FRIENDLY_OUTLINE 252;
#define STENCIL_NEUTRAL_OUTLINE 253;
#define STENCIL_ENEMY_OUTLINE 254;
#define STENCIL_ITEMHIGHLIGHT 255;

#include "EngineMinimal.h"

//#include "Headers/MyConstants.h"


#endif
