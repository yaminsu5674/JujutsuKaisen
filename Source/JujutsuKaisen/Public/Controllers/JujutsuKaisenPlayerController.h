// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JujutsuKaisenPlayerController.generated.h"

class AJujutsuKaisenCharacter;

/**
 * 플레이어 컨트롤러 클래스
 * 캐릭터를 플레이어 모드로 설정하는 역할
 */
UCLASS()
class JUJUTSUKAISEN_API AJujutsuKaisenPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AJujutsuKaisenPlayerController();

protected:
	virtual void BeginPlay() override;
};
