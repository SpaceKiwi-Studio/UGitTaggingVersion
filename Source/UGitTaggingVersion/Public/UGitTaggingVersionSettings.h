// Copyright Space Kiwi Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UGitTaggingVersion/Classes/UGitTaggingVersionActionBase.h"
#include "Engine/DeveloperSettings.h"
#include "UGitTaggingVersionSettings.generated.h"

/**
 * @author Space Kiwi Studio
 */
UCLASS(Config=GitATG, DefaultConfig, MinimalAPI, meta = (DisplayName="Git Auto Tagging Game Config"))
class UGitTaggingVersionSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UGitTaggingVersionSettings() = default;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="GitATGGeneral")
	FString GitPath = "C:/Program Files/Git/bin/git.exe";

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="GitATGGeneral")
	FString RepositoryPath = FPaths::ProjectDir();

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="GitATGGeneral")
	FString GameConfigFile = FPaths::ProjectDir().Append("/Config/DefaultGame.ini");

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="GitATGGeneral")
	TSubclassOf<UGitTaggingVersionActionBase> ActionToTrigger = UGitTaggingVersionActionBase::StaticClass();
};
