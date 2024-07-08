// Copyright Space Kiwi Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UGitTaggingVersion/Classes/UGitTaggingVersionActionBase.h"
#include "Engine/DeveloperSettings.h"
#include "UGitTaggingVersionSettings.generated.h"

/**
 * @author Space Kiwi Studio
 */
UCLASS(Config=GitTaggingVersion, DefaultConfig, MinimalAPI, meta = (DisplayName="Git Tagging Version Config"))
class UGitTaggingVersionSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UGitTaggingVersionSettings() = default;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="GitTaggingVersionGeneral")
	FString GitPath = "C:/Program Files/Git/bin/git.exe";

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="GitTaggingVersionGeneral")
	FString RepositoryPath = FPaths::ProjectDir();

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="GitTaggingVersionGeneral")
	FString GameConfigFile = FPaths::ProjectDir().Append("/Config/DefaultGame.ini");

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="GitTaggingVersionGeneral")
	TSubclassOf<UGitTaggingVersionActionBase> ActionToTrigger = UGitTaggingVersionActionBase::StaticClass();
};
