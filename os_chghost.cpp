/* os_chghost: a module that implements services-side CHGHOST
 *
 * Copyright (C) 2016 Michael Hazell <michaelhazell@hotmail.com>
 * 
 * You may use this module under the terms of the GPLv3
 * 
 * Configuration:
 * module { name = "os_chghost"; }
 * command { name = "CHGHOST"; service = "OperServ"; command = "operserv/chghost"; permission = "hostserv/set"; }
 */

#include "module.h"

class CommandOSChghost : public Command
{
 public:
	CommandOSChghost(Module *creator, const Anope::string &sname = "operserv/chghost") : Command(creator, sname, 2, 2)
	{
		this->SetDesc(_("Forcibly change a user's displayed host"));
		this->SetSyntax(_("\037nick\037 \037vhost\037"));
	}
	
	void Execute(CommandSource &source, const std::vector<Anope::string> &params) anope_override
	{
		User *u = User::Find(params[0], true);
		
		if (!u)
		{
			source.Reply(_("User %s is not online"), params[0].c_str());
			return;
		}
		
		Anope::string vhost = params[1];
		Anope::string ident; // We don't handle Vidents
		
		if (!IRCD->IsHostValid(vhost))
		{
			source.Reply("Invalid vhost");
			return;
		}
		
		IRCD->SendVhost(u, ident, vhost);
		u->UpdateHost();
		
		Log(LOG_ADMIN, source, this) << "to change the displayed host of " << u->nick.c_str() << " to " << vhost.c_str();
		source.Reply(_("Displayed host for \002%s\002 changed to \002%s\002"), u->nick.c_str(), vhost.c_str());
		
	}
	
	bool OnHelp (CommandSource &source, const Anope::string &subcommand) anope_override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply(_("Allows a Services Operator to forcibly change a user's displayed host"));
		return true;
	}
};

class OSChghost : public Module
{
	CommandOSChghost commandoschghost;
	
 public:
	OSChghost(const Anope::string &modname, const Anope::string &creator) : 
	Module(modname, creator, THIRD), commandoschghost(this)
	{
		this->SetAuthor("Techman");
		this->SetVersion("1.0");
		
		if (!IRCD || !IRCD->CanSetVHost)
			throw ModuleException("Your IRCd does not support vhosts");
	}
};

MODULE_INIT(OSChghost)