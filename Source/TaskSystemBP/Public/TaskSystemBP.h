﻿#pragma once

#include "Modules/ModuleManager.h"

class FTaskSystemBPModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
