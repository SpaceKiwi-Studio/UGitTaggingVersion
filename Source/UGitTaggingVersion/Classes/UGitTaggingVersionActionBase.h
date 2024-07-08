// Copyright Space Kiwi Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UGitTaggingVersionActionBase.generated.h"

/**
 * @author Space Kiwi Studio
 */
UCLASS(MinimalAPI)
class UGitTaggingVersionActionBase : public UObject
{
	GENERATED_BODY()

public:
	UGitTaggingVersionActionBase() = default;

	virtual bool OnActionTrigger(FString& StdErr);
};
