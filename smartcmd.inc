/*******************************************************************************************************************************
SmartCMD a.k.a Improved ZCMD Plus (iZCMD+)
This include is a feature rich version of iZCMD.

Version: 0.3.3 beta (25th December 2017)

GitHub Link: https://github.com/YashasSamaga/SmartCMD
*******************************************************************************************************************************/
#if defined SCMD_INCLUDED
	#endinput
#endif
#define SCMD_INCLUDED
#define SCMD_INCLUDED_VMAJOR 0
#define SCMD_INCLUDED_VMINOR 3
#define SCMD_INCLUDED_VPATCH 3
/******************************************************************************************************************************/
#if !defined CMD_DEFAULT_FLAG
	#define CMD_DEFAULT_FLAG 0
#endif

/* return value to be used in command functions */
#define CMD_SUCCESS 1
#define CMD_FAILURE 0

#define INVALID_COMMAND_ID			(-1)

#define MAX_CLIENT_MSG_LENGTH		(144)
#define MAX_FUNC_NAME				(32)

#define MAX_COMMAND_NAME            (28)

#if !defined MAX_PUBLIC_FUNCTIONS
	#define MAX_PUBLIC_FUNCTIONS	(1024)
#endif

#if !defined MAX_COMMANDS
	#define MAX_COMMANDS			(500)
#endif

#if !defined isnull
	#define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif
/******************************************************************************************************************************/
/* preprocessor directives to handle command declaration syntax */
/*
	CCCM = create command mode
	CVARS = create command variables
		CCF = create command flags
		CCRM = remove
		CCID = create command id
*/

const __smartcmd_default_flag = (CMD_DEFAULT_FLAG);

#define cid_%0\32;%1; cid_%0%1
#define flg_%0\32;%1; flg_%0%1
#define flg@%0\32;%1; flg@%0%1
#define alt_%0\32;%1; alt_%0%1
#define alt@%0\32;%1; alt@%0%1
#define tag@%0\32;%1; tag@%0%1
#define cmd_%0\32;%1( cmd_%1(

#define CMD%0:%1(%2) \
            forward __smartcmd_CCCM:tag@%1();__smartcmd_CVARS(%0,%1,%2)forward cmd_%1(%2);public cmd_%1(%2)<>return __smartcmd_cmd_handled=0;public cmd_%1(%2)<cmdMode:normal>

#define COMMAND%0:%1(%2) \
            CMD%0:%1(%2)

#define command%0(%1,%2,%3,%4) \
            CMD%0:%1(%2, %3, %4)

#define cmd%0(%1,%2,%3,%4) \
            CMD%0:%1(%2, %3, %4)

#define __smartcmd_CCCM:tag@%0[%1](%9);__smartcmd_CVARS(%9,%9,%2)%9;%9;%9<%9> \
            __smartcmd_CCCM:tag@%0();public cmd_%0(%2)<cmdMode:%1>

#define __smartcmd_CVARS(%0,%1,%2) __smartcmd_CCF(%1,%0)__smartcmd_CCRM(%2)__smartcmd_CCID(cid_%1)

#define ALT:%1=%2CMD:%0; \
            __smartcmd_CCAV(%1)__smartcmd_CCF(%1,)__smartcmd_CCID(cid_%1)forward cmd_%1(cmdid, playerid, params[]);public cmd_%1(cmdid, playerid, params[])<>return __smartcmd_cmd_handled=0;public cmd_%1(cmdid, playerid, params[])<cmdMode:normal>return __smartcmd_register_alias(GetCommandID(#%0), cmdid);

#define __smartcmd_CCID(%0) \
            public stock const %0=-1;

#define __smartcmd_default_flag%9__smartcmd_REMOVE<%1> %1
#define __smartcmd_REMOVE

#define __smartcmd_CCF(%1,%2) \
            public flg_%1=(__smartcmd_default_flag __smartcmd_REMOVE%2);stock flg@%1=flg_%1;

#define __smartcmd_CCRM(%2)

#define __smartcmd_CCAV(%1) \
            public alt_%1=0; \
            stock alt@%1=alt_%1;
/******************************************************************************************************************************/
#if defined OnPlayerCommandReceived
	forward OnPlayerCommandReceived(cmdid, playerid, cmdtext[]);
#endif
#if defined OnPlayerCommandPerformed
	forward OnPlayerCommandPerformed(cmdid, playerid, cmdtext[], success);
#endif
/******************************************************************************************************************************/
enum __smartcmd_CommandProperties(*=2)
{
	__smartcmd_CMD_ENABLED = 1,
	__smartcmd_CMD_HAS_ALTERNATE,
	__smartcmd_CMD_IS_ALTERNATE
}
static __smartcmd_CommandProperties:izcmd_cmd_properties[MAX_COMMANDS];

#define __IsCommandPropertySet(%0,%1)	((izcmd_cmd_properties[%0]) & (%1))
#define __SetCommandProperty(%0,%1)		((izcmd_cmd_properties[%0]) |= (%1))
#define __UnsetCommandProperty(%0,%1)	((izcmd_cmd_properties[%0]) &= ~(%1))
/******************************************************************************************************************************/
//Internal Arrays
static __smartcmd_fidx_pcid_map[MAX_PUBLIC_FUNCTIONS] = { INVALID_COMMAND_ID, INVALID_COMMAND_ID, INVALID_COMMAND_ID, ... }; //cmdid to be used for the given funcidx in OnPlayerCommandText
static __smartcmd_cid_pfidx_map[MAX_COMMANDS] = { -1, -1, -1, ... }; //pointing command function
static __smartcmd_cid_pid_map[MAX_COMMANDS] = { INVALID_COMMAND_ID, INVALID_COMMAND_ID, INVALID_COMMAND_ID, ... }; //pointing cmdid

//Constant Arrays (must not be modified)
static __const_smartcmd_funcname[MAX_COMMANDS][MAX_FUNC_NAME];
static __const_smartcmd_fidx_ocid_map[MAX_PUBLIC_FUNCTIONS] = { INVALID_COMMAND_ID, INVALID_COMMAND_ID, INVALID_COMMAND_ID, ... };
static __const_smartcmd_cid_ofidx_map[MAX_COMMANDS] = { -1, -1, -1, ... }; //original command function

//Per-Command Properties
static __smartcmd_pfaddress[MAX_COMMANDS] = { -1, -1, -1, ... };
static __smartcmd_cflags[MAX_COMMANDS][1];

//General Command Statstics
static __smartcmd_cmd_total_count = 0;
static __smartcmd_cmd_enabled_count = 0;

//Internal Variables
static __smartcmd_offsetDAT, __smartcmd_offsetPFT, __smartcmd_offsetNFT, __smartcmd_offsetPVT, __smartcmd_offsetTAGS, __smartcmd_number_of_publics, __smartcmd_number_of_pubvars;

//Addtitional Support Variables (used internally)
new __smartcmd_cmd_handled = 1;
/******************************************************************************************************************************/
static stock __smartcmd_dummy() < > return 0;
static stock __smartcmd_dummy() < cmdMode:normal > return 0;

static __smartcmd_LoadString(addr, str[])
{
	new chr, j;
	do
	{
		#emit LREF.S.pri addr
		#emit STOR.S.pri chr

		str[j++] = chr = (chr & 0xFF);

		#emit INC.S addr
	} while (chr != EOS);
}
static __smartcmd_init()
{
	new addr, tmp, cmd_name[MAX_FUNC_NAME] = "cmd_", bool:crash = false;

	state cmdMode:normal;

	#emit LCTRL 1
	#emit NEG
	#emit STOR.pri __smartcmd_offsetDAT

	#emit ADD.C 32
	#emit STOR.S.pri addr
	#emit LREF.S.pri addr
	#emit LOAD.S.alt addr
	#emit ADD
	#emit CONST.alt 32
	#emit SUB
	#emit STOR.pri __smartcmd_offsetPFT

	#emit LOAD.pri __smartcmd_offsetDAT
	#emit ADD.C 36
	#emit STOR.S.pri addr
	#emit LREF.S.pri addr
	#emit LOAD.S.alt addr
	#emit ADD
	#emit CONST.alt 36
	#emit SUB
	#emit STOR.pri __smartcmd_offsetNFT

	#emit LOAD.pri __smartcmd_offsetDAT
	#emit ADD.C 44
	#emit STOR.S.pri addr
	#emit LREF.S.pri addr
	#emit LOAD.S.alt addr
	#emit ADD
	#emit CONST.alt 44
	#emit SUB
	#emit STOR.pri __smartcmd_offsetPVT
		
	#emit LOAD.pri __smartcmd_offsetDAT
	#emit ADD.C 48
	#emit STOR.S.pri addr
	#emit LREF.S.pri addr
	#emit LOAD.S.alt addr
	#emit ADD
	#emit CONST.alt 48
	#emit SUB
	#emit STOR.pri __smartcmd_offsetTAGS

	__smartcmd_number_of_publics = ((__smartcmd_offsetNFT - __smartcmd_offsetPFT) >>> 3);
	__smartcmd_number_of_pubvars = ((__smartcmd_offsetTAGS - __smartcmd_offsetPVT) >>> 3);

	for (new pubfunc_idx = 0; pubfunc_idx < __smartcmd_number_of_publics; pubfunc_idx++)
	{
		if (pubfunc_idx == MAX_PUBLIC_FUNCTIONS)
		{
			printf("[ERROR] MAX_PUBLIC_FUNCTIONS (Current Value: %d) must be set to %d or higher.", MAX_PUBLIC_FUNCTIONS, __smartcmd_number_of_publics);
			print("[WARNING] All commands may not work as expected.");
			crash = true;
			break;
		}

		addr = (pubfunc_idx << 3) + __smartcmd_offsetPFT + 4;

		#emit LREF.S.pri addr
		#emit LOAD.alt __smartcmd_offsetDAT
		#emit ADD
		#emit STOR.S.pri addr

		#emit LREF.S.pri addr
		#emit STOR.S.pri tmp

		if (tmp != ((('_') << 24) | (('d') << 16) | (('m') << 8) | ('c')))	continue; {}

		strcat(__const_smartcmd_funcname[__smartcmd_cmd_total_count], "cmd_", MAX_FUNC_NAME);

		#emit LOAD.S.pri addr
		#emit ADD.C 4
		#emit STOR.S.pri addr

		__smartcmd_LoadString(addr, __const_smartcmd_funcname[__smartcmd_cmd_total_count][4]);

		#emit LOAD.pri __smartcmd_offsetPFT
		#emit LOAD.S.alt  pubfunc_idx
		#emit SHL.C.alt   3
		#emit ADD
		#emit STOR.S.pri  addr
		#emit LREF.S.pri  addr
		#emit STOR.S.pri  addr

		__const_smartcmd_fidx_ocid_map[pubfunc_idx] = __smartcmd_fidx_pcid_map[pubfunc_idx] = __smartcmd_cmd_total_count;
		__const_smartcmd_cid_ofidx_map[__smartcmd_cmd_total_count] = __smartcmd_cid_pfidx_map[__smartcmd_cmd_total_count] = pubfunc_idx;

		__SetCommandProperty(__smartcmd_cmd_total_count, __smartcmd_CMD_ENABLED);
		__smartcmd_pfaddress[__smartcmd_cmd_total_count] = addr;

		__smartcmd_cmd_total_count++;
		__smartcmd_cmd_enabled_count++;

		if (__smartcmd_cmd_total_count == MAX_COMMANDS)
		{
			printf("[ERROR] MAX_COMMANDS (Current Value: %d) must be set to larger number.", __smartcmd_cmd_total_count);
			print("[WARNING] All commands may not work as expected.");
			crash = true;
			break;
		}
	}
	for (new pubvar_idx = 0; pubvar_idx < __smartcmd_number_of_pubvars; pubvar_idx++)
	{
		addr = (pubvar_idx << 3) + __smartcmd_offsetPVT + 4;

		#emit LREF.S.pri addr
		#emit LOAD.alt __smartcmd_offsetDAT
		#emit ADD
		#emit STOR.S.pri addr

		#emit LREF.S.pri addr
		#emit STOR.S.pri tmp

		if (tmp == ((('_') << 24) | (('g') << 16) | (('l') << 8) | ('f')))
		{
			#emit LOAD.S.pri addr
			#emit ADD.C 4
			#emit STOR.S.pri addr

			__smartcmd_LoadString(addr, cmd_name[4]);
			
			new cmd_idx = funcidx(cmd_name);
			if (cmd_idx == -1) break;
			if (cmd_idx >= MAX_PUBLIC_FUNCTIONS) break; {}

			#emit LOAD.pri __smartcmd_offsetPVT
			#emit LOAD.S.alt  pubvar_idx
			#emit SHL.C.alt   3
			#emit ADD
			#emit STOR.S.pri  addr
			#emit LREF.S.pri  addr
			#emit STOR.S.pri  addr

			#emit LOAD.I
			#emit STOR.S.pri  tmp

			new x, n;

			#emit CONST.alt __const_smartcmd_fidx_ocid_map
			#emit LOAD.S.pri cmd_idx
			#emit LIDX
			#emit CONST.alt __smartcmd_cflags
			#emit IDXADDR

			#emit STOR.S.pri x

			n = addr - x;

			#emit LOAD.S.alt x
			#emit LOAD.S.pri n
			#emit STOR.I
		}
		else if (tmp == ((('_') << 24) + (('t') << 16) + (('l') << 8) + ('a')))
		{
			#emit LOAD.S.pri addr
			#emit ADD.C 4
			#emit STOR.S.pri addr

			__smartcmd_LoadString(addr, cmd_name[4]);

			new cmd_idx = funcidx(cmd_name);
			if (cmd_idx == -1) break;
			if (cmd_idx >= MAX_PUBLIC_FUNCTIONS) break;

			new cmdid = __const_smartcmd_fidx_ocid_map[cmd_idx];

			#emit PUSH.S cmd_name //doesn't matter what is pushed
			#emit PUSH.C 0
			#emit PUSH.S cmdid
			#emit PUSH.C 12

			#emit LCTRL      6
			#emit ADD.C      40
			#emit PUSH.pri
			#emit CONST.alt __smartcmd_pfaddress
			#emit LOAD.S.pri cmdid
			#emit LIDX
			#emit SCTRL      6
		}
		else if (tmp == ((('_') << 24) + (('d') << 16) + (('i') << 8) + ('c')))
		{
			#emit LOAD.S.pri addr
			#emit ADD.C 4
			#emit STOR.S.pri addr

			__smartcmd_LoadString(addr, cmd_name[4]);

			new cmd_idx = funcidx(cmd_name);
			if (cmd_idx == -1) break;
			if (cmd_idx >= MAX_PUBLIC_FUNCTIONS) break; {}

			#emit LOAD.pri __smartcmd_offsetPVT
			#emit LOAD.S.alt  pubvar_idx
			#emit SHL.C.alt   3
			#emit ADD
			#emit STOR.S.pri  addr
			#emit LREF.S.pri  addr
			#emit STOR.S.pri  addr

			#emit CONST.alt __const_smartcmd_fidx_ocid_map
			#emit LOAD.S.pri cmd_idx
			#emit LIDX
			#emit LOAD.S.alt addr
			#emit STOR.I
		}
	}
	if (crash)
	{
		new x = MAX_COMMANDS;
		return __smartcmd_pfaddress[x];
	}
	return 1;
}
/******************************************************************************************************************************/
#if defined FILTERSCRIPT
	public OnFilterScriptInit()
	{
		__smartcmd_init();
		#if defined izcmd_OnFilterScriptInit
			return izcmd_OnFilterScriptInit();
		#else
			return 1;
		#endif
	}

	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit izcmd_OnFilterScriptInit

	#if defined OnFilterScriptInit
		forward OnFilterScriptInit();
	#endif
#else
	public OnGameModeInit()
	{
		__smartcmd_init();
		#if defined izcmd_OnGameModeInit
			return izcmd_OnGameModeInit();
		#else
			return 1;
		#endif
	}

	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit izcmd_OnGameModeInit

	#if defined izcmd_OnGameModeInit
		forward izcmd_OnGameModeInit();
	#endif
#endif
/******************************************************************************************************************************/
public OnPlayerCommandText(playerid, cmdtext[])
{
	static prefix[4] = { 'c', 'm', 'd', '_' }, funcname[MAX_CLIENT_MSG_LENGTH - 4];
	new cmdid, tmp, addr, faddr;

	state cmdMode:normal;

	#emit CONST.pri funcname
	#emit STOR.S.pri faddr

	#emit LOAD.S.pri cmdtext
	#emit ADD.C 4
	#emit STOR.S.pri addr

	#emit LOAD.I
	#emit STOR.S.pri tmp

	while (tmp > ' ')
	{
		#if !defined IZCMD_ENABLE_CASE_SENSITIVITY
		if ('A' <= tmp <= 'Z')
		{
			#emit LOAD.S.pri tmp
			#emit CONST.alt 32
			#emit OR

			#emit LOAD.S.alt faddr
			#emit STOR.I
		}
		else
		#endif
		{
			#emit LOAD.S.alt faddr
			#emit LOAD.S.pri tmp
			#emit STOR.I
		}
		#emit LOAD.S.pri faddr
		#emit ADD.C 4
		#emit STOR.S.pri faddr

		#emit LOAD.S.pri addr
		#emit ADD.C 4
		#emit STOR.S.pri addr

		#emit LOAD.I
		#emit STOR.S.pri tmp
	}

	#emit LOAD.S.alt faddr
	#emit ZERO.pri
	#emit STOR.I

	while (tmp == ' ')
	{
		#emit LOAD.S.pri addr
		#emit ADD.C 4
		#emit STOR.S.pri addr

		#emit LOAD.I
		#emit STOR.S.pri tmp
	}

	if ((tmp = funcidx(prefix)) != -1)
	{
		if ((cmdid = __smartcmd_fidx_pcid_map[tmp]) != INVALID_COMMAND_ID && __IsCommandPropertySet(cmdid, __smartcmd_CMD_ENABLED))
		{
			#if defined OnPlayerCommandReceived
				if (!OnPlayerCommandReceived(cmdid, playerid, cmdtext)) return 1; {}
			#endif

			#emit PUSH.S addr
			#emit PUSH.S playerid
			#emit PUSH.S cmdid
			#emit PUSH.C 12

			#emit LCTRL      6
			#emit ADD.C      40
			#emit PUSH.pri

			#emit CONST.alt __smartcmd_pfaddress
			#emit LOAD.S.pri cmdid
			#emit LIDX
			#emit SCTRL      6

			#emit STOR.S.pri tmp

			#if defined OnPlayerCommandPerformed
				return OnPlayerCommandPerformed(cmdid, playerid, cmdtext, tmp);
			#else
				return tmp;
			#endif
		}
	}
	#if defined OnPlayerCommandReceived
		if (!OnPlayerCommandReceived(INVALID_COMMAND_ID, playerid, cmdtext)) return 1;
	#endif

	#if defined OnPlayerCommandPerformed
		return OnPlayerCommandPerformed(INVALID_COMMAND_ID, playerid, cmdtext, 0);
	#else
		return 0;
	#endif
}
/******************************************************************************************************************************/
/*
	native DoesCommandExist(cmdid)
	native GetCommandID(const cmd[])
	native GetCommandName(cmdid, cmd[], len = sizeof(cmd))
	native GetAlternateCommands(cmdid, cmdidlist[])
	native IsCommandAlternate(cmdid)
	native GetCommandFunctionID(cmdid)
	native GetPointingCommandFunctionID(cmdid)
	native GetPointingCommandID(cmdid)
	native GetCommandFunctionName(cmdid, dest[], len = sizeof(dest))
	native GetEnabledCommandCount()
	native GetDisabledCommandCount()
	native GetTotalCommandCount()
	native EnableCommand(cmdid)
	native DisableCommand(cmdid)
	native IsCommandEnabled(cmdid)
	native SetCommandFlags(cmdid, flags)
	native GetCommandFlags(cmdid)
	native SetPointingCommandIDToSelf(cmdid)
	native ReassignCommandFunction(cmdid, const funcname[], bool:updateCID = false, bool:updatePFT = false)
	native EmulateCommandEx(cmdid, playerid, params[])
	native EmulateCommand(playerid, cmdtext[])
	native ExecuteCommand(const cmd[], command_mode, playerid, &success, params[]="")
*/
/*******************************************************************************************************************************
	<summary>DoesCommandExist</summary>
	<para>checks if a command exists</para>
	<param name="cmd">command name</param>
	<returns>
		Returns true if the command exists
	</returns>
	<example>
		<code>
			if(DoesCommandExist("ban"))
				printf("Ban command exists");
		</code>
	</example>
*******************************************************************************************************************************/
#define DoesCommandExist(%0) (GetCommandID(%0) != INVALID_COMMAND_ID)

/*******************************************************************************************************************************
	<summary>GetCommandID</summary>
	<para>Finds command id from the given commmand name</para>
	<param name="cmd">command name</param>
	<returns>
		the command id or INVALID_COMMAND_ID if the command does not exist.
	</returns>
	<remarks>
		The command name passed must have the command name not the command function name. For example, if you have a ban command
		the command name is "ban" whereas the function name is "cmd_ban". You must pass "ban" to this function to obtain ban
		command's command id.

		If you are already aware of the command name during compile time, you can obtain the command idby directly referncing
		the variable whose identifier is the command name prefixed with "cid_".

		Please do not attempt to modify the value of variables prefixed with "cid_". The variable becomes unusable if you modify
		but will not affect the command processor and other commands.
	</remarks>
	<example>
		<code>
			new id = GetCommandID("ban"); //is same as "new id = cid_ban;"
		</code>
	</example>
*******************************************************************************************************************************/
stock GetCommandID(const cmd[])
{
	static funcname[MAX_FUNC_NAME] = "cmd_";
	funcname[4] = 0;
	strcat(funcname, cmd);

	new idx = funcidx(funcname);
	if (idx == -1)
		return INVALID_COMMAND_ID;
	return __const_smartcmd_fidx_ocid_map[idx];
}

/*******************************************************************************************************************************
	<summary>GetCommandName</summary>
	<para>Gets a command name in unpacked format</para>
	<param name="cmdid">command id whose name is required</param>
	<param name="cmd">destination array</param>
	<param name="len">maximum length of cmd</param>
	<returns>
		1 on success and 0 on failure (invalid command id was passed).
	</returns>
	<example>
		<code>
			new str[MAX_COMMAND_NAME];
			GetCommandName(cmdid, str);
		</code>
	</example>
*******************************************************************************************************************************/
stock GetCommandName(cmdid, cmd[], len = sizeof(cmd))
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return 0;

	cmd[0] = 0;
	strcat(cmd, __const_smartcmd_funcname[cmdid][4], len);
	return 1;
}

/*******************************************************************************************************************************
	<summary>GetAlternateCommands</summary>
	<para>Gets a list of alternate command names associated with the given command</para>
	<param name="cmdid">command id</param>
	<param name="cmdidlist">array where the alternate command ids must be stored</param>
	<param name="cmdidlist_len">length of 'cmdidlist' array</param>
	<returns>
		the number of alternate commands or INVALID_COMMAND_ID in case cmdid passed has an invalid id.
	</returns>
	<remarks>
		The function does not do boundary checks on the cmdidlist array. It is required that the array passed is sufficiently large.
	</remarks>
	<example>
		<code>
			new acmdlist[5], cmdname[MAX_COMMAND_NAME];
			new count = GetAlternateCommands(GetCommandID("spectate"), acmdlist);

			new helpstr[200] = "Alternate commands for spectate:";
			for(new i = 0; i < count; i++)
			{
				GetCommandName(acmdlist[i], cmdname);
				strcat(helpstr, cmdname);
				strcat(helpstr, " ");
			}
		</code>
	</example>
*******************************************************************************************************************************/
stock GetAlternateCommands(cmdid, cmdidlist[], cmdidlist_len = sizeof(cmdidlist))
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;

	new count = 0;
	if (__IsCommandPropertySet(cmdid, __smartcmd_CMD_HAS_ALTERNATE))
	{
		for (new id = 0; id < __smartcmd_cmd_total_count; id++)
		{
			if (id == cmdid) continue;
			if (__smartcmd_cid_pid_map[id] == cmdid)
			{
				cmdidlist[count++] = id;
				if(count == cmdidlist_len)
				    break;
			}
		}
	}
	return count;
}

/*******************************************************************************************************************************
	<summary>IsCommandAlternate</summary>
	<para>checks if the given command is an alternate command</para>
	<param name="cmdid">command id</param>
	<returns>
		true(false) if the command is an(not an) alternate command or INVALID_COMMAND_ID if the command id passed is invalid.
	</returns>
	<remarks>
		The location where the file should be created should be relative to the scriptfiles folder
	</remarks>
	<example>
		<code>
			for(new i = 0, j = GetTotalCommandCount(); i < j; i++)
			{
				if(i == cmdid) continue;
				if(IsCommandAlternate(i)) continue;

				new cmd[28];
				GetCommandName(i, cmd);
				strcat(str, cmd);
				strcat(str, " ");
			}
		</code>
	</example>
*******************************************************************************************************************************/
stock IsCommandAlternate(cmdid)
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	return !!(__IsCommandPropertySet(cmdid, __smartcmd_CMD_IS_ALTERNATE));
}

/*******************************************************************************************************************************
	<summary>GetCommandFunctionID</summary>
	<para>returns the original function (check remarks) index of the given command. Identical to funcidx.</para>
	<param name="cmdid">command id</param>
	<returns>
		INVALID_COMMAND_ID if the given command id is invalid else returns the function index.
	</returns>
	<remarks>
		original function - the function which is associated with the command when the server starts
		pointing function - the function which is executed when the command is used
	</remarks>
	<example>
		<code>
			if(GetCommandFunctionID(GetCommandID("ban")) != funcidx("cmd_ban")) print("Logic does not work in this world");
		</code>
	</example>
*******************************************************************************************************************************/
stock GetCommandFunctionID(cmdid)
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	return __const_smartcmd_cid_ofidx_map[cmdid];
}

/*******************************************************************************************************************************
	<summary>GetPointingCommandFunctionID</summary>
	<para>returns the pointing function (check remarks) index of the given command.</para>
	<param name="cmdid">command id</param>
	<returns>
		INVALID_COMMAND_ID if the given command id is invalid else returns the function index.
	</returns>
	<remarks>
		original function - the function which is associated with the command when the server starts
		pointing function - the function which is executed when the command is used
	</remarks>
	<example>
		<code>
			//spec is an alternate command for the command spectate
			if(GetPointingCommandFunctionID(spec_cmdid) == GetCommandFunctionID(spectate_cmdid)) print("All iz well.");
		</code>
	</example>
*******************************************************************************************************************************/
stock GetPointingCommandFunctionID(cmdid)
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	return __smartcmd_cid_pfidx_map[cmdid];
}

/*******************************************************************************************************************************
	<summary>GetPointingCommandID</summary>
	<para>finds the command to which the given command points to</para>
	<param name="cmdid">command id</param>
	<returns>
		pointing command id
	</returns>
	<remarks>
		pointing command - the command which is executed when the command is used.

		Reassigning a command to a command function of another command without setting the updateCID parameter to true will NOT
		make the reassigned command to point to the new command. It only calls the command function. The command id of the
		reassigned command will be passed to the command function.

		In case of alternate commands, the command id of the original command is passed to the command function. In other words,
		your command can never know if the player used the original command directly or used an alternate command.
	</remarks>
	<example>
		<code>
			//spec is an alternate command for the command spectate
			if(GetPointingCommandID(spec_cmdid) == spectate_cmdid) print("All iz well.");
		</code>
	</example>
*******************************************************************************************************************************/
stock GetPointingCommandID(cmdid)
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	return __smartcmd_cid_pid_map[cmdid];
}

/*******************************************************************************************************************************
	<summary>GetCommandFunctionName</summary>
	<para>gets the command name in its function name form</para>
	<param name="cmdid">command id</param>
	<param name="dest">destination array where the function name has to be stored</param>
	<param name="len">size of the destination array</param>
	<returns>
		1 on success or INVALID_COMMAND_ID on if the given command id is invalid.
	</returns>
	<example>
		<code>
			//Avoid using CallLocalFunction to call commands manually. Use EmulateCommand(Ex) for the same.
			CallLocalFunction(GetCommandFunctionName("ban"), "iis", GetCommandID("ban"), playerid, "test ban");
		</code>
	</example>
*******************************************************************************************************************************/
stock GetCommandFunctionName(cmdid, dest[], len = sizeof(dest))
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	dest[0] = 0;
	strcat(dest, "cmd_", len);
	GetCommandName(cmdid, dest[4], len - 4);
	return 1;
}

/*******************************************************************************************************************************
	<summary>GetEnabledCommandCount</summary>
	<para>gets the number of enabled commands</para>
	<returns>
		number of enabled commands
	</returns>
	<example>
		<code>
			printf("Number of commands enabled: %d", GetEnabledCommandCount());
		</code>
	</example>
*******************************************************************************************************************************/
stock GetEnabledCommandCount()
{
	return __smartcmd_cmd_enabled_count;
}

/*******************************************************************************************************************************
	<summary>GetDisabledCommandCount</summary>
	<para>gets the number of disabled(deleted) commands</para>
	<returns>
		number of disabled commands
	</returns>
	<example>
		<code>
			printf("Number of commands disabled: %d", GetDisabledCommandCount());
		</code>
	</example>
*******************************************************************************************************************************/
stock GetDisabledCommandCount()
{
	return __smartcmd_cmd_total_count - __smartcmd_cmd_enabled_count;
}

/*******************************************************************************************************************************
	<summary>GetTotalCommandCount</summary>
	<para>gets total number of commands in the script</para>
	<returns>
		number of commands in the script
	</returns>
	<example>
		<code>
			printf("Total number of commands: %d", GetTotalCommandCount());
		</code>
	</example>
*******************************************************************************************************************************/
stock GetTotalCommandCount()
{
	return __smartcmd_cmd_total_count;
}

/*******************************************************************************************************************************
	<summary>EnableCommand</summary>
	<para>Enables(Undelete) a command if it was disabled(deleted)</para>
	<param name="cmd">cmdid</param>
	<returns>
		INVALID_COMMAND_ID if the given command id is invalid.
		0 if the command was already enabled.
		1 if the command was enabled.
	</returns>
	<example>
		<code>
			EnableCommand(cid_event_join);
		</code>
	</example>
*******************************************************************************************************************************/
stock EnableCommand(cmdid)
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	if (__IsCommandPropertySet(cmdid, __smartcmd_CMD_ENABLED)) return 0;

	__SetCommandProperty(cmdid, __smartcmd_CMD_ENABLED);

	__smartcmd_cmd_enabled_count++;
	return 1;
}

/*******************************************************************************************************************************
	<summary>EnableCommand</summary>
	<para>Disables(delete) a command if it was enabled</para>
	<param name="cmd">cmdid</param>
	<returns>
		INVALID_COMMAND_ID if the given command id is invalid.
		0 if the command was already disabled.
		1 if the command was disabled.
	</returns>
	<example>
		<code>
			DisableCommand(cid_event_join);
		</code>
	</example>
*******************************************************************************************************************************/
stock DisableCommand(cmdid)
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	if (!__IsCommandPropertySet(cmdid, __smartcmd_CMD_ENABLED)) return 0;

	__UnsetCommandProperty(cmdid, __smartcmd_CMD_ENABLED);

	__smartcmd_cmd_enabled_count--;
	return 1;
}

/*******************************************************************************************************************************
	<summary>IsCommandEnabled</summary>
	<para>checks if a command is enabled</para>
	<param name="cmdid">command id</param>
	<returns>
		true if the command is enabled or INVALID_COMMAND_ID If the given command id is invalid.
	</returns>
	<example>
		<code>
			if(IsCommandEnabled(cid_event_join)) return SendClientMessage(playerid, RED, "An event is in progress.");
		</code>
	</example>
*******************************************************************************************************************************/
stock IsCommandEnabled(cmdid)
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	return !!(__IsCommandPropertySet(cmdid, __smartcmd_CMD_ENABLED));
}

/*******************************************************************************************************************************
	<summary>SetCommandFlags</summary>
	<para>sets a command's flags</para>
	<param name="cmdid">command id</param>
	<param name="flags">new flags</param>
	<returns>
		1 on success and INVALID_COMMAND_ID if the given command id is invalid.
	</returns>
	<remarks>
		If you are already aware of the command's name, you can directly modify the flag variable. The flag variable identifier
		is the command name prefixed with 'flg_'. For example, the ban command's flags are stored in 'flg_ban'. Directly accessing
		the flag variable is faster than using this function.
	</remarks>
	<example>
		<code>
			SetCommandFlags(cmdid, ADMIN_COMMAND);
			SetCommandFlags(cid_ban, ADMIN_COMMAND); //is functionally equivalent to doing "flg_ban = ADMIN_COMMAND;"
		</code>
	</example>
*******************************************************************************************************************************/
stock SetCommandFlags(cmdid, flags)
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;

	__smartcmd_cflags[cmdid][0] = flags;
	return 1;
}

/*******************************************************************************************************************************
	<summary>GetCommandFlags</summary>
	<para>gets the given command's flags</para>
	<param name="cmdid">command id</param>
	<returns>
		the command's flags or INVALID_COMMAND_ID if the command id passed is invalid.
	</returns>
	<remarks>
		It is possible that your flags might evaluate to INVALID_COMMAND_ID and hence GetCommandFlags might pretend to be returning
		INVALID_COMMAND_ID for a valid command id.

		The best way to deal with it is to ensure that invalid command id would never be passed.
	</remarks>
	<example>
		<code>
			new flags = GetCommandFlags(cid_ban);
			new flags = GetCommandFlags(GetCommandID("ban"); //is functionally equivalent to doing "new flags = flg_ban;"
		</code>
	</example>
*******************************************************************************************************************************/
stock GetCommandFlags(cmdid)
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;

	#emit CONST.alt __smartcmd_cflags
	#emit LOAD.S.pri cmdid
	#emit LIDX
	#emit LOAD.I
	#emit RETN

	return  __smartcmd_cflags[cmdid][0];
}

/*******************************************************************************************************************************
	<summary>SetPointingCommandIDToSelf</summary>
	<para>Forces the command to point itself without changing other parameters</para>
	<param name="cmdid">command id</param>
	<returns>
		the command's flags or INVALID_COMMAND_ID if the command id passed is invalid.
	</returns>
	<remarks>
		This can also be used to dealternatize an alternate command or convert an alternate command to a reassigned command.
	</remarks>
	<example>
		<code>
			SetPointingCommandIDToSelf(cid_spec);
		</code>
	</example>
*******************************************************************************************************************************/
stock SetPointingCommandIDToSelf(cmdid)
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;

	if (__IsCommandPropertySet(cmdid, __smartcmd_CMD_IS_ALTERNATE))
	{
		new ocmdid = __smartcmd_cid_pfidx_map[cmdid];

		__UnsetCommandProperty(cmdid, __smartcmd_CMD_IS_ALTERNATE);
		__UnsetCommandProperty(ocmdid, __smartcmd_CMD_HAS_ALTERNATE);

		for (new id = 0; id < __smartcmd_cmd_total_count; id++)
		{
			if (id == ocmdid) continue;
			if (id == cmdid) continue;

			if (__smartcmd_cid_pfidx_map[id] == cmdid)
			{
				__SetCommandProperty(ocmdid, __smartcmd_CMD_HAS_ALTERNATE);
				break;
			}
		}
	}
	__smartcmd_cid_pid_map[cmdid] = INVALID_COMMAND_ID;
	return 1;
}

/*******************************************************************************************************************************
	<summary>ReassignCommandFunction</summary>
	<para>checks if a command exists</para>
	<param name="cmdid">command id</param>
	<param name="funcname">name of the function to which the command has to be assigned to</param>
	<param name="updateCID">if the function name is a command function, this option allows you to allow the comamnd (whose id is cmdid)
		to	pretend (alternate command) like the command whose function name was passed. (false by default)</param>
	<param name="updatePFT">Updates the public function table which makes CallLocalFunction/CallRemoteFunction execute the function
		whose name is funcname when asked to call the command (false by default)</param>
	<returns>
		INVALID_COMMAND_ID if the command id passed is invalid.
		1 on success and 0 on failure.
	</returns>
	<remarks>
		Reassigning a command will indirectly have all its alternate commands reassigned
	</remarks>
	<example>
		<code>
			//custom command disable
			forward disabled(cmdid, playerid, params[])
			public disabled(cmdid, playerid, params[])
			{
				return SendClientMessage(playerid, RED, "The command has been disabled.");
			}
			CMD:disablecmd(cmdid, playerid, params[])
			{
				switch(ReassignCommandFunction(GetCommandID(params), "disabled"))
				{
					case -1: return SendClientMessage(playerid, RED, "The entered command does not exist.");
					case 0: return SendClientMessage(playerid, RED, "The entered command is already enabled.");
					case 1: return SendClientMessage(playerid, RED, "The entered command has been enabled.");
				}
				return CMD_SUCCESS;
			}
		</code>
	</example>
*******************************************************************************************************************************/
stock ReassignCommandFunction(cmdid, const funcname[], bool:updateCID = false, bool:updatePFT = false)
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;

	new idx_funcname = funcidx(funcname);
	if (idx_funcname == -1) return 0;

	if (__IsCommandPropertySet(cmdid, __smartcmd_CMD_IS_ALTERNATE))
	{
		new ocmdid = __smartcmd_cid_pfidx_map[cmdid];

		__UnsetCommandProperty(cmdid, __smartcmd_CMD_IS_ALTERNATE);
		__UnsetCommandProperty(ocmdid, __smartcmd_CMD_HAS_ALTERNATE);

		for (new id = 0; id < __smartcmd_cmd_total_count; id++)
		{
			if (id == ocmdid) continue;
			if (id == cmdid) continue;

			if (__smartcmd_cid_pfidx_map[id] == cmdid)
			{
				__SetCommandProperty(ocmdid, __smartcmd_CMD_HAS_ALTERNATE);
				break;
			}
		}
	}

	new ocmdid = __const_smartcmd_fidx_ocid_map[idx_funcname], oaddr, caddr;

	#emit LOAD.pri __smartcmd_offsetPFT
	#emit LOAD.S.alt  idx_funcname
	#emit SHL.C.alt   3
	#emit ADD
	#emit STOR.S.pri  oaddr
	#emit LREF.S.pri  oaddr
	#emit STOR.S.pri  oaddr

	__smartcmd_cid_pfidx_map[cmdid] = idx_funcname;
	__smartcmd_cid_pid_map[cmdid] = INVALID_COMMAND_ID;
	__smartcmd_pfaddress[cmdid] = oaddr;

	if (ocmdid != INVALID_COMMAND_ID)
	{
		__smartcmd_cid_pid_map[cmdid] = ocmdid;
		if (updateCID)
		{
			__SetCommandProperty(cmdid, __smartcmd_CMD_IS_ALTERNATE);
			__SetCommandProperty(ocmdid, __smartcmd_CMD_HAS_ALTERNATE);
			__smartcmd_fidx_pcid_map[__const_smartcmd_cid_ofidx_map[cmdid]] = ocmdid;
		}
	}

	if (updatePFT)
	{
		#emit CONST.alt __const_smartcmd_cid_ofidx_map
		#emit LOAD.S.pri cmdid
		#emit LIDX

		#emit SHL.C.pri   3
		#emit LOAD.alt __smartcmd_offsetPFT
		#emit ADD
		#emit STOR.S.pri  caddr
		#emit LOAD.S.pri oaddr
		#emit SREF.S.pri  caddr
	}
	return 1;
}
/*******************************************************************************************************************************
	<summary>ExecuteCommand</summary>
	<para>Executes a command's function with the given mode/state</para>
	<param name="cmd[]">Command name</param>
	<param name="state">Mode/State</param>
	<param name="playerid">player id (sender)</param>
	<param name="success"> reference variable which will be set according to the rules give below:
		INVALID_COMMAND_ID if the command does not exist.
		0 if the command mode does not exist.
		1 if the command mode exists and was called.
		2 if the command was disabled using DisableCommand.
	</param>
	<param name="params[]">Command parameters</param>
	<returns>
		Returns value returned by the command function
	</returns>
	<example>
		<code>
			ExecuteCommand("pm",help,playerid,"");
		</code>
	</example>
*******************************************************************************************************************************/
#define ExecuteCommand(%0,%1,%2,%3,%4) state cmdMode:%1;_ExecuteCommand(%0,%2,%3,%4)

stock _ExecuteCommand(const cmd[], playerid, &success, params[] = "")
{
	new cmdid = GetCommandID(cmd), pos;
	if (!(0 <= cmdid < __smartcmd_cmd_total_count))
	{
		success = INVALID_COMMAND_ID;
		return 0;
	}

	success = 2;
	if (__IsCommandPropertySet(cmdid, __smartcmd_CMD_ENABLED))
	{
		while (params[pos] == ' ') pos++;

		success = __smartcmd_cmd_handled = 1;

		#emit LOAD.S.alt params
		#emit LOAD.S.pri pos
		#emit IDXADDR
		#emit PUSH.pri
		#emit PUSH.S playerid
		#emit PUSH.S cmdid
		#emit PUSH.C 12

		#emit LCTRL      6
		#emit ADD.C      40
		#emit PUSH.pri

		#emit CONST.alt __smartcmd_pfaddress
		#emit LOAD.S.pri cmdid
		#emit LIDX
		#emit SCTRL      6

		#emit STOR.S.pri pos

		state cmdMode:normal;
		if (__smartcmd_cmd_handled == 0) success = 0;

		#emit LOAD.S.pri pos
		#emit STACK 8
		#emit RETN
	}
	return 0;
}
/*******************************************************************************************************************************
	<summary>EmulateCommand</summary>
	<para>Emulates a command (fakes a player sending a command)</para>
	<param name="playerid">player id (sender)</param>
	<param name="cmdtext">text typed by the player (sender) into the message bar</param>
	<returns>
		the value returned by OnPlayerCommandPerformed (if it existed) or the value returned by the command function
		or the value returned by OnPlayerCommandReceived if the command was invalid and OnPlayerCommandPerformed does not exist.
	</returns>
	<remarks>
		The cmdtext must have '/' as its first character followed by the command name and the parameters in order.
	</remarks>
	<example>
		<code>
			EmulateCommand(0, "/pm 1 I am a n00b!");
		</code>
	</example>
*******************************************************************************************************************************/
stock EmulateCommand(playerid, cmdtext[])
{
	if (OnPlayerCommandText(playerid, cmdtext))
		return 1;
	else
	{
		SendClientMessage(playerid, -1, "SERVER: Unknown Command");
		return 0;
	}
}

/*******************************************************************************************************************************
	<summary>EmulateCommandEx</summary>
	<para>Emulates a command (fakes a player sending a command)</para>
	<param name="cmdid">id of the command to fake</param>
	<param name="playerid">player id (sender)</param>
	<param name="params">command params</param>
	<returns>
		the value returned by OnPlayerCommandPerformed (if it existed) or the value returned by the command function
		or the value returned by OnPlayerCommandReceived if the command was invalid and OnPlayerCommandPerformed does not exist.
	</returns>
	<remarks>
		The cmdtext must have '/' as its first character followed by the command name and the parameters in order.
	</remarks>
	<example>
		<code>
			EmulateCommand(cid_pm, ,"1 I am a n00b!");
		</code>
	</example>
*******************************************************************************************************************************/
stock EmulateCommandEx(cmdid, playerid, params[])
{
	if (!(0 <= cmdid < __smartcmd_cmd_total_count)) return INVALID_COMMAND_ID;

	new cmdtext[MAX_CLIENT_MSG_LENGTH], pos, retval;
	cmdtext[0] = '/';

	GetCommandName(cmdid, cmdtext[1], MAX_CLIENT_MSG_LENGTH);

	if (!isnull(params))
	{
		strcat(cmdtext, " ");
		strcat(cmdtext, params);
		while (params[pos] == ' ') pos++;
	}

	state cmdMode:normal;

	if (__IsCommandPropertySet(cmdid, __smartcmd_CMD_ENABLED))
	{
		#if defined OnPlayerCommandReceived
		if (!OnPlayerCommandReceived(cmdid, playerid, cmdtext)) return 1; {}
		#endif

		#emit LOAD.S.alt params
		#emit LOAD.S.pri pos
		#emit IDXADDR
		#emit PUSH.pri
		#emit PUSH.S playerid
		#emit PUSH.S cmdid
		#emit PUSH.C 12

		#emit LCTRL      6
		#emit ADD.C      40
		#emit PUSH.pri

		#emit CONST.alt __smartcmd_pfaddress
		#emit LOAD.S.pri cmdid
		#emit LIDX
		#emit SCTRL      6

		#emit STOR.S.pri retval

		#if defined OnPlayerCommandPerformed
		return OnPlayerCommandPerformed(cmdid, playerid, cmdtext, retval);
		#else
		return retval;
		#endif
	}
	#if defined OnPlayerCommandReceived
	if (!OnPlayerCommandReceived(INVALID_COMMAND_ID, playerid, cmdtext)) return 1;
	#endif

	#if defined OnPlayerCommandPerformed
	return OnPlayerCommandPerformed(INVALID_COMMAND_ID, playerid, cmdtext, 0);
	#else
	return 0;
	#endif
}
/******************************************************************************************************************************/
stock __smartcmd_register_alias(ocmdid, acmdid)
{
	new	aidx = __const_smartcmd_cid_ofidx_map[acmdid], aaddr, oaddr;

	__SetCommandProperty(ocmdid, __smartcmd_CMD_HAS_ALTERNATE);
	__SetCommandProperty(acmdid, __smartcmd_CMD_IS_ALTERNATE);

	__smartcmd_fidx_pcid_map[aidx] = ocmdid;
	__smartcmd_cid_pfidx_map[acmdid] = __smartcmd_cid_pfidx_map[ocmdid];
	__smartcmd_cid_pid_map[acmdid] = ocmdid;
	oaddr = __smartcmd_pfaddress[acmdid] = __smartcmd_pfaddress[ocmdid];
	__smartcmd_cflags[acmdid] = __smartcmd_cflags[ocmdid];

	#emit LOAD.pri __smartcmd_offsetPFT
	#emit LOAD.S.alt  aidx
	#emit SHL.C.alt   3
	#emit ADD
	#emit STOR.S.pri  aaddr
	#emit LOAD.S.pri oaddr
	#emit SREF.S.pri  aaddr
	return 0;
}
/******************************************************************************************************************************/
