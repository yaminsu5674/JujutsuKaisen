#pragma once

#include "CoreMinimal.h"
#include "GameModes/JujutsuKaisenGameMode.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SingleLobbyGameMode.generated.h"

/**
 * 싱글 로비 전용 게임모드
 * 난이도별 AI 비헤이비어 트리를 설정해 두고, 다른 게임모드가 참조할 수 있게 한다.
 */
UCLASS()
class JUJUTSUKAISEN_API ASingleLobbyGameMode : public AJujutsuKaisenGameMode
{
	GENERATED_BODY()

public:
	ASingleLobbyGameMode();

	// 난이도별 비헤이비어 트리 소프트 참조
	UPROPERTY(EditDefaultsOnly, Category = "AI|Behavior")
	TSoftObjectPtr<UBehaviorTree> EasyBehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Behavior")
	TSoftObjectPtr<UBehaviorTree> NormalBehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Behavior")
	TSoftObjectPtr<UBehaviorTree> HardBehaviorTree;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Behavior", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UBehaviorTree> SelectedBehaviorTree;

public:
	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	void SetSelectedBehaviorTree(TSoftObjectPtr<UBehaviorTree> InBehaviorTree) { SelectedBehaviorTree = InBehaviorTree; }

	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	TSoftObjectPtr<UBehaviorTree> GetSelectedBehaviorTree() const { return SelectedBehaviorTree; }
};

