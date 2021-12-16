/* m_unreal_privdeaf: Add support for umode +D
 *
 * Copyright (C) 2016 Michael Hazell <michaelhazell@hotmail.com>
 *
 * You may use this module under the terms of docs/COPYING,
 * located in the Anope source directory
 */
#include "module.h"

class ProtoUnrealPrivDeaf : public Module
{
 public:
	ProtoUnrealPrivDeaf(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator, THIRD)
	{
		this->SetAuthor("Techman");
		this->SetVersion("1.0.2");

		if ((!ModuleManager::FindModule("unreal") && !ModuleManager::FindModule("unreal4")) || !IRCD)
		{
			throw ModuleException("This module is only designed to work with UnrealIRCd.");
		}

		UserMode *um = ModeManager::FindUserModeByChar('D');
		if (um)
		{
			throw ModuleException("An existing user mode '" + um->name + "' already in Anope with character 'D'");
		}
		ModeManager::AddUserMode(new UserMode("PRIVDEAF", 'D'));
		Log(this) << "Added Anope support for Unreal PRIVDEAF (+D).";
	}

	~ProtoUnrealPrivDeaf()
	{
		UserMode *um = ModeManager::FindUserModeByName("PRIVDEAF");
		if (um)
		{
			ModeManager::RemoveUserMode(um);
		}
		Log(this) << "Removed Anope support for Unreal PRIVDEAF (+D).";
	}
};

MODULE_INIT(ProtoUnrealPrivDeaf)
