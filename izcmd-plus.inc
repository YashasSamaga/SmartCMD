/*******************************************************************************************************************************
 Improved ZCMD Plus (iZCMD+)
 This include is a feature rich version of iZCMD.
 Version: 0.3.0 (October 11th 2016)

 GitHub Link: https://github.com/YashasSamaga/iZCMD-Plus
*******************************************************************************************************************************/
#if defined _I_ZCMDP_INCLUDED
	#endinput
#endif
#define _I_ZCMDP_INCLUDED
#define _I_ZCMDP_INCLUDED_V0_3_0_0_
/******************************************************************************************************************************/
const CMD_DEFAULT_FLAG = 0;

#define CMD_DEFAULT_FLAG%9_@REMOVE@_<%1> %1
#define _@REMOVE@_

#define CMD%0:%1(%2) \
            _CreateCommandFlags(%1,%0) \
            _CreateCommandIDVar(%1) \
			forward cmd_%1(%2); \
			public cmd_%1(%2)

#define COMMAND%0:%1(%2) \
			CMD%0:%1(%2)

#define command%0(%1,%2,%3,%4) \
		COMMAND%0:%1(%2, %3, %4)

#define cmd%0(%1,%2,%3,%4) \
		COMMAND%0:%1(%2, %3, %4)

#define ALT:%0:%1; \
        _CreateCommandFlags(%1,)\
        _CreateCommandIDVar(%1) \
        _CreateCommandAltvar(%0,%1) \
		forward cmd_%1(cmdid, playerid, params[]);\
		public cmd_%1(cmdid, playerid, params[])\
			return _izcmdp_register_alias(GetCommandID(#%0), cmdid);

#define _CreateCommandIDVar(%0) \
            public cid_%0=-1; \
			stock cid@%0=cid_%0;

#define _CreateCommandFlags(%0,%1) \
            public flg_%0=CMD_DEFAULT_FLAG _@REMOVE@_%1 ; \
			stock flg@%0=flg_%0;
			
#define _CreateCommandAltvar(%0,%1) \
    		public alt_%1= 0; \
			stock alt@%1=alt_%1;

#define CMD_SUCCESS 1
#define CMD_FAILURE 0

#define INVALID_COMMAND_ID			(-1)

#define MAX_CLIENT_MSG_LENGTH		(144)
#define MAX_FUNC_NAME				(32)

#define MAX_COMMAND_NAME            (28)

#if !defined MAX_PUBLIC_FUNCTIONS
	#define MAX_PUBLIC_FUNCTIONS	(2048)
#endif

#if !defined MAX_COMMANDS
	#define MAX_COMMANDS			(500)
#endif

#if !defined isnull
	#define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif
/******************************************************************************************************************************/
#if defined OnPlayerCommandReceived
	forward OnPlayerCommandReceived(cmdid, playerid, cmdtext[]);
#endif
#if defined OnPlayerCommandPerformed
	forward OnPlayerCommandPerformed(cmdid, playerid, cmdtext[], success);
#endif
/******************************************************************************************************************************/
enum (<<=1)
{
	COMMAND_ENABLED = 1,
	COMMAND_HAS_ALTERNATE,
	COMMAND_IS_ALTERNATE
}
static izcmd_cmd_properties[MAX_COMMANDS];

#define _IsCommandPropertySet(%0,%1)            ((izcmd_cmd_properties[%0]) & (%1))
#define _SetCommandProperty(%0,%1)             ((izcmd_cmd_properties[%0]) |= (%1))
#define _UnsetCommandProperty(%0,%1)            ((izcmd_cmd_properties[%0]) &= ~(%1))
/******************************************************************************************************************************/
//Internal Arrays
static izcmd_cmd_OPTC_idx_id[MAX_PUBLIC_FUNCTIONS] = {INVALID_COMMAND_ID, INVALID_COMMAND_ID, INVALID_COMMAND_ID, ...}; //cmdid to be used for the given funcidx in OnPlayerCommandText
static izcmd_cmd_idx_id_map[MAX_PUBLIC_FUNCTIONS] = {INVALID_COMMAND_ID, INVALID_COMMAND_ID, INVALID_COMMAND_ID, ...}; //MUST NEVER BE CHANGED

static izcmd_cmd_id_oidx_map[MAX_COMMANDS] = {-1, -1, -1, ...}; //original command function - MUST NEVER BE CHANGED
static izcmd_cmd_id_pidx_map[MAX_COMMANDS] = {-1, -1, -1, ...}; //pointing command function
static izcmd_cmd_id_pid_map[MAX_COMMANDS] = {INVALID_COMMAND_ID, INVALID_COMMAND_ID, INVALID_COMMAND_ID, ...}; //pointing cmdid

//Per-Command Properties
static izcmd_cmd_funcname[MAX_COMMANDS][MAX_FUNC_NAME];
static izcmd_cmd_oaddress[MAX_COMMANDS] = {-1, -1, -1, ...};
static izcmd_cmd_paddress[MAX_COMMANDS] = {-1, -1, -1, ...};
static izcmd_cmd_flags[MAX_COMMANDS][1];

//General Command Statstics
static izcmd_cmd_total_count = 0;
static izcmd_cmd_enabled_count = 0;

//Internal Variables
static offsetDAT, offsetPFT, offsetNFT, offsetPVT, offsetTAGS, number_of_publics, number_of_pubvars;
/******************************************************************************************************************************/
static LoadString(addr, str[])
{
	new chr, j;
    do
   	{
		#emit LREF.S.pri addr
		#emit STOR.S.pri chr
		
        str[j++] = chr = (chr & 0xFF);
        
		#emit INC.S addr
	}while(chr != EOS);
}
static izcmd_Init()
{
	new addr, tmp, cmd_name[MAX_FUNC_NAME] = "cmd_", bool:crash = false;

	#emit LCTRL 1
	#emit NEG
	#emit STOR.pri offsetDAT

	#emit ADD.C 32
	#emit STOR.S.pri addr
	#emit LREF.S.pri addr
	#emit LOAD.S.alt addr
	#emit ADD
	#emit CONST.alt 32
	#emit SUB
	#emit STOR.pri offsetPFT

	#emit LOAD.pri offsetDAT
	#emit ADD.C 36
	#emit STOR.S.pri addr
	#emit LREF.S.pri addr
	#emit LOAD.S.alt addr
	#emit ADD
	#emit CONST.alt 36
	#emit SUB
	#emit STOR.pri offsetNFT

	#emit LOAD.pri offsetDAT
	#emit ADD.C 44
	#emit STOR.S.pri addr
	#emit LREF.S.pri addr
	#emit LOAD.S.alt addr
	#emit ADD
	#emit CONST.alt 44
	#emit SUB
	#emit STOR.pri offsetPVT

	#emit LOAD.pri offsetDAT
	#emit ADD.C 48
	#emit STOR.S.pri addr
	#emit LREF.S.pri addr
	#emit LOAD.S.alt addr
	#emit ADD
	#emit CONST.alt 48
	#emit SUB
	#emit STOR.pri offsetTAGS

	number_of_publics = ((offsetNFT - offsetPFT) >>> 3);
	number_of_pubvars = ((offsetTAGS - offsetPVT) >>> 3);

	for(new pubfunc_idx = 0; pubfunc_idx < number_of_publics; pubfunc_idx++)
	{
		if(pubfunc_idx == MAX_PUBLIC_FUNCTIONS)
		{
			printf("[ERROR] MAX_PUBLIC_FUNCTIONS (Current Value: %d) must be set to %d or higher.", MAX_PUBLIC_FUNCTIONS, number_of_publics);
			print("[WARNING] All commands may not work as expected.");
			crash = true;
			break;
		}

		addr = (pubfunc_idx << 3) + offsetPFT + 4;

		#emit LREF.S.pri addr
		#emit LOAD.alt offsetDAT
		#emit ADD
		#emit STOR.S.pri addr

		#emit LREF.S.pri addr
		#emit STOR.S.pri tmp

		if(tmp != ((('_')<<24) | (('d')<<16) | (('m')<<8) | ('c')))	continue; 	{}

        strcat(izcmd_cmd_funcname[izcmd_cmd_total_count], "cmd_", MAX_FUNC_NAME);

        #emit LOAD.S.pri addr
		#emit ADD.C 4
		#emit STOR.S.pri addr

        LoadString(addr, izcmd_cmd_funcname[izcmd_cmd_total_count][4]);

		#emit LOAD.pri offsetPFT
		#emit LOAD.S.alt  pubfunc_idx
		#emit SHL.C.alt   3
		#emit ADD
		#emit STOR.S.pri  addr
		#emit LREF.S.pri  addr
		#emit STOR.S.pri  addr

		izcmd_cmd_idx_id_map[pubfunc_idx] = izcmd_cmd_OPTC_idx_id[pubfunc_idx] = izcmd_cmd_total_count;
		izcmd_cmd_id_oidx_map[izcmd_cmd_total_count] = izcmd_cmd_id_pidx_map[izcmd_cmd_total_count] = pubfunc_idx;

        _SetCommandProperty(izcmd_cmd_total_count, COMMAND_ENABLED);
		izcmd_cmd_paddress[izcmd_cmd_total_count] = izcmd_cmd_oaddress[izcmd_cmd_total_count] = addr;

		izcmd_cmd_total_count++;
		izcmd_cmd_enabled_count++;

		if(izcmd_cmd_total_count == MAX_COMMANDS)
		{
			printf("[ERROR] MAX_COMMANDS (Current Value: %d) must be set to larger number.", izcmd_cmd_total_count);
			print("[WARNING] All commands may not work as expected.");
			crash = true;
			break;
		}
	}
	for(new pubvar_idx = 0; pubvar_idx < number_of_pubvars; pubvar_idx++)
	{
		addr = (pubvar_idx << 3) + offsetPVT + 4;

        #emit LREF.S.pri addr
        #emit LOAD.alt offsetDAT
        #emit ADD
        #emit STOR.S.pri addr

        #emit LREF.S.pri addr
        #emit STOR.S.pri tmp

		if(tmp == ((('_')<<24) | (('g')<<16) | (('l')<<8) | ('f')))
		{
		    #emit LOAD.S.pri addr
		    #emit ADD.C 4
		    #emit STOR.S.pri addr
		    
			LoadString(addr,cmd_name[4]);
			
		    new cmd_idx = funcidx(cmd_name);
			if(cmd_idx == -1) break;
			if(cmd_idx >= MAX_PUBLIC_FUNCTIONS) break; {}

            #emit LOAD.pri offsetPVT
			#emit LOAD.S.alt  pubvar_idx
			#emit SHL.C.alt   3
			#emit ADD
			#emit STOR.S.pri  addr
			#emit LREF.S.pri  addr
			#emit STOR.S.pri  addr
			
			#emit LOAD.I
			#emit STOR.S.pri  tmp
            
            new x, n;
            
            #emit CONST.alt izcmd_cmd_idx_id_map
            #emit LOAD.S.pri cmd_idx
            #emit LIDX
            #emit CONST.alt izcmd_cmd_flags 
			#emit IDXADDR 

			#emit STOR.S.pri x
			
			n = addr - x;

			#emit LOAD.S.alt x
			#emit LOAD.S.pri n
			#emit STOR.I
		}
	    else if(tmp == ((('_')<<24) + (('t')<<16) + (('l')<<8) + ('a')))
		{
		    #emit LOAD.S.pri addr
		    #emit ADD.C 4
		    #emit STOR.S.pri addr

		   	LoadString(addr,cmd_name[4]);

 		    new cmd_idx = funcidx(cmd_name);
			if(cmd_idx == -1) break;
			if(cmd_idx >= MAX_PUBLIC_FUNCTIONS) break;

			new cmdid = izcmd_cmd_idx_id_map[cmd_idx];
			
		    #emit PUSH.S cmd_name //doesn't matter what is pushed
			#emit PUSH.C 0
			#emit PUSH.S cmdid
			#emit PUSH.C 12

			#emit LCTRL      6
			#emit ADD.C      40
			#emit PUSH.pri
    		#emit CONST.alt izcmd_cmd_paddress
    		#emit LOAD.S.pri cmdid
			#emit LIDX
			#emit SCTRL      6
		}
		else if(tmp == ((('_')<<24) + (('d')<<16) + (('i')<<8) + ('c')))
		{
		    #emit LOAD.S.pri addr
		    #emit ADD.C 4
		    #emit STOR.S.pri addr

		   	LoadString(addr,cmd_name[4]);
		   	
		   	new cmd_idx = funcidx(cmd_name);
			if(cmd_idx == -1) break;
			if(cmd_idx >= MAX_PUBLIC_FUNCTIONS) break; {}
			
			#emit LOAD.pri offsetPVT
			#emit LOAD.S.alt  pubvar_idx
			#emit SHL.C.alt   3
			#emit ADD
			#emit STOR.S.pri  addr
			#emit LREF.S.pri  addr
			#emit STOR.S.pri  addr

			#emit CONST.alt izcmd_cmd_idx_id_map
			#emit LOAD.S.pri cmd_idx
			#emit LIDX
			#emit LOAD.S.alt addr
			#emit STOR.I
		}
	}
	if(crash)
	{
	    new x = MAX_COMMANDS;
		return izcmd_cmd_paddress[x];
	}
	return 1;
}
/******************************************************************************************************************************/
#if defined FILTERSCRIPT
	public OnFilterScriptInit()
	{
	   	izcmd_Init();
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
	    izcmd_Init();
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
	static prefix[3] = {'c', 'm', 'd'}, funcname[MAX_CLIENT_MSG_LENGTH] = "_";
	new pos = 1, tmp, cmdid, retval;

	while((tmp = cmdtext[pos]) > ' ')
	{
		#if !defined IZCMD_ENABLE_CASE_SENSITIVITY
			if('A' <= tmp <= 'Z') funcname[pos++] = tmp | 0x20;
			else
		#endif
				funcname[pos++] = tmp;
	}
	funcname[pos] = 0;

	while (cmdtext[pos] == ' ') pos++;

	if((tmp = funcidx(prefix)) != -1)
	{
		if((cmdid = izcmd_cmd_OPTC_idx_id[tmp]) != INVALID_COMMAND_ID && _IsCommandPropertySet(cmdid, COMMAND_ENABLED))
		{
		    #if defined OnPlayerCommandReceived
	    		if(!OnPlayerCommandReceived(cmdid, playerid, cmdtext)) return 1; {}
			#endif

		    #emit LOAD.S.alt cmdtext
			#emit LOAD.S.pri pos
			#emit IDXADDR
			#emit PUSH.pri
			#emit PUSH.S playerid
			#emit PUSH.S cmdid
			#emit PUSH.C 12

			#emit LCTRL      6
			#emit ADD.C      40
			#emit PUSH.pri

		    #emit CONST.alt izcmd_cmd_paddress
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
	}
    #if defined OnPlayerCommandReceived
    	if(!OnPlayerCommandReceived(INVALID_COMMAND_ID, playerid, cmdtext)) return 1;
	#endif

	#if defined OnPlayerCommandPerformed
		return OnPlayerCommandPerformed(INVALID_COMMAND_ID, playerid, cmdtext, 0);
	#else
    	return 0;
	#endif
}
/******************************************************************************************************************************/
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
		Returns the command id or INVALID_COMMAND_ID if the command does not exist.
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

	if(idx == -1) return INVALID_COMMAND_ID;
	return izcmd_cmd_idx_id_map[idx];
}

/*******************************************************************************************************************************
	<summary>GetCommandName</summary>
	<para>Gets a command name in unpacked format</para>
	<param name="cmdid">command id whose name is required</param>
	<param name="cmd">destination array</param>
	<param name="len">maximum length of cmd</param>
	<returns>
		Returns 1 on success and 0 on failure (invalid command id was passed).
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
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return 0;

	cmd[0] = 0;
	strcat(cmd, izcmd_cmd_funcname[cmdid][4], len);
	return 1;
}

/*******************************************************************************************************************************
	<summary>GetAlternateCommands</summary>
	<para>Gets a list of alternate command names associated with the given command</para>
	<param name="cmdid">command id</param>
	<param name="cmdidlist">array where the alternate command ids must be stored</param>
	<returns>
		Returns the number of alternate commands or INVALUID_COMMAND_IN in case cmdid passed has an invalid id.
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
stock GetAlternateCommands(cmdid, cmdidlist[])
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;

	new count = 0;
	if(iz_IsCommandPropertySet(cmdid, COMMAND_HAS_ALTERNATE))
	{
	    for(new id = 0; id < izcmd_cmd_total_count; id++)
	    {
	        if(id == cmdid) continue;
            if(izcmd_cmd_id_pidx_map[id] == cmdid)
                cmdidlist[count++] = id;
	    }
	}
	return count;
}

/*******************************************************************************************************************************
	<summary>IsCommandAlternate</summary>
	<para>checks if the given command is an alternate command</para>
	<param name="cmdid">command id</param>
	<returns>
		Returns true(false) if the command is an(not an) alternate command or INVALID_COMMAND_ID if the command id passed is invalid.
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
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
    return !!(_IsCommandPropertySet(cmdid, COMMAND_IS_ALTERNATE));
}

/*******************************************************************************************************************************
	<summary>GetCommandFunctionID</summary>
	<para>returns the original function (check remarks) index of the given command. Identical to funcidx.</para>
	<param name="cmdid">command id</param>
	<returns>
		Returns INVALID_COMMAND_ID if the given command id is invalid else returns the function index.
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
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	return izcmd_cmd_id_oidx_map[cmdid];
}

/*******************************************************************************************************************************
	<summary>GetPointingCommandFunctionID</summary>
	<para>returns the pointing function (check remarks) index of the given command.</para>
	<param name="cmdid">command id</param>
	<returns>
		Returns INVALID_COMMAND_ID if the given command id is invalid else returns the function index.
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
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	return izcmd_cmd_id_pidx_map[cmdid];
}

/*******************************************************************************************************************************
	<summary>GetPointingCommandID</summary>
	<para>finds the command to which the given command points to</para>
	<param name="cmdid">command id</param>
	<returns>
		Returns true if the command exists
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
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	return izcmd_cmd_id_pid_map[cmdid];
}

/*******************************************************************************************************************************
	<summary>GetCommandFunctionName</summary>
	<para>gets the command name in its function name form</para>
	<param name="dest">destination array where the function name has to be stored</param>
	<param name="cmdid">command id</param>
	<param name="len">size of the destination array</param>
	<returns>
		Returns 1 on success or INVALID_COMMAND_ID on if the given command id is invalid.
	</returns>
	<example>
	    <code>
	        //Avoid using CallLocalFunction to call commands manually. Use EmulateCommand(Ex) for the same.
			CallLocalFunction(GetCommandFunctionName("ban"), "iis", GetCommandID("ban"), playerid, "test ban");
	    </code>
	</example>
*******************************************************************************************************************************/
stock GetCommandFunctionName(dest[], cmdid, len = sizeof(dest))
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	dest[0] = 0;
	strcat(dest, "cmd_", len);
	GetCommandName(cmdid, dest[4], len-4);
	return 1;
}

/*******************************************************************************************************************************
	<summary>GetEnabledCommandCount</summary>
	<para>gets the number of enabled commands</para>
	<returns>
		Returns number of enabled commands
	</returns>
	<example>
	    <code>
	    	printf("Number of commands enabled: %d", GetEnabledCommandCount());
	    </code>
	</example>
*******************************************************************************************************************************/
stock GetEnabledCommandCount()
{
	return izcmd_cmd_enabled_count;
}

/*******************************************************************************************************************************
	<summary>GetDisabledCommandCount</summary>
	<para>gets the number of disabled(deleted) commands</para>
	<returns>
		Returns number of disabled commands
	</returns>
	<example>
	    <code>
	    	printf("Number of commands disabled: %d", GetDisabledCommandCount());
	    </code>
	</example>
*******************************************************************************************************************************/
stock GetDisabledCommandCount()
{
	return izcmd_cmd_total_count - izcmd_cmd_enabled_count;
}

/*******************************************************************************************************************************
	<summary>GetTotalCommandCount</summary>
	<para>gets total number of commands in the script</para>
	<returns>
		Returns number of commands in the script
	</returns>
	<example>
	    <code>
	    	printf("Total number of commands: %d", GetTotalCommandCount());
	    </code>
	</example>
*******************************************************************************************************************************/
stock GetTotalCommandCount()
{
	return izcmd_cmd_total_count;
}

/*******************************************************************************************************************************
	<summary>EnableCommand</summary>
	<para>Enables(Undelete) a command if it was disabled(deleted)</para>
	<param name="cmd">cmdid</param>
	<returns>
		Returns INVALID_COMMAND_ID if the given command id is invalid.
		Returns 0 if the command was already enabled.
		Returns 1 if the command was enabled.
	</returns>
	<example>
	    <code>
	    	EnableCommand(cid_event_join);
	    </code>
	</example>
*******************************************************************************************************************************/
stock EnableCommand(cmdid)
{
	if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	if(_IsCommandPropertySet(cmdid, COMMAND_ENABLED)) return 0;

    _SetCommandProperty(cmdid, COMMAND_ENABLED);

    izcmd_cmd_enabled_count++;
	return 1;
}

/*******************************************************************************************************************************
	<summary>EnableCommand</summary>
	<para>Disables(delete) a command if it was enabled</para>
	<param name="cmd">cmdid</param>
	<returns>
		Returns INVALID_COMMAND_ID if the given command id is invalid.
		Returns 0 if the command was already disabled.
		Returns 1 if the command was disabled.
	</returns>
	<example>
	    <code>
	    	DisableCommand(cid_event_join);
	    </code>
	</example>
*******************************************************************************************************************************/
stock DisableCommand(cmdid)
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	if(!_IsCommandPropertySet(cmdid, COMMAND_ENABLED)) return 0;

	_UnsetCommandProperty(cmdid, COMMAND_ENABLED);

    izcmd_cmd_enabled_count--;
	return 1;
}

/*******************************************************************************************************************************
	<summary>IsCommandEnabled</summary>
	<para>checks if a command is enabled</para>
	<param name="cmdid">command id</param>
	<returns>
		Returns true if the command is enabled or INVALID_COMMAND_ID If the given command id is invalid.
	</returns>
	<example>
	    <code>
	    	if(IsCommandEnabled(cid_event_join)) return SendClientMessage(playerid, RED, "An event is in progress.");
	    </code>
	</example>
*******************************************************************************************************************************/
stock IsCommandEnabled(cmdid)
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	return !!(_IsCommandPropertySet(cmdid, COMMAND_ENABLED));
}

/*******************************************************************************************************************************
	<summary>SetCommandFlags</summary>
	<para>sets a command's flags</para>
	<param name="cmdid">command id</param>
	<param name="flags">new flags</param>
	<returns>
		Returns 1 on success and INVALID_COMMAND_ID if the given command id is invalid.
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
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;

    izcmd_cmd_flags[cmdid][0] = flags;
    return 1;
}

/*******************************************************************************************************************************
	<summary>GetCommandID</summary>
	<para>gets the given command's flags</para>
	<param name="cmdid">command id</param>
	<returns>
		Returns the command's flags or INVALID_COMMAND_ID if the command id passed is invalid.
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
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
    return izcmd_cmd_flags[cmdid][0];
}

/*******************************************************************************************************************************
	<summary>ReassignCommandFunction</summary>
	<para>checks if a command exists</para>
	<param name="cmdid">command id</param>
	<param name="funcname">name of the function to which the command has to be assigned to</param>
	<param name="updateCID">if the function name is a command function, this option allows you to allow the comamnd (whose id is cmdid)
							to	pretend (alternate command) like the command whose function name was passed. (true by default)</param>
	<param name="updatePFT">Updates the public function table which makes CallLocalFunction/CallRemoteFunction execute the function
							whose name is funcname when asked to call the command (false by default)</param>
	<returns>
		Returns INVALID_COMMAND_ID if the command id passed is invalid.
		Returns 1 on success and 0 on failure.
	</returns>
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
stock ReassignCommandFunction(cmdid, const funcname[], bool:updateCID = true, bool:updatePFT = false)
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
    
    new idx_funcname = funcidx(funcname);
	if(idx_funcname == -1) return 0;
    
    if(_IsCommandPropertySet(cmdid, COMMAND_IS_ALTERNATE))
	{
		new ocmdid = izcmd_cmd_id_pidx_map[cmdid];
		
		_UnsetCommandProperty(cmdid, COMMAND_IS_ALTERNATE);
		_UnsetCommandProperty(ocmdid, COMMAND_HAS_ALTERNATE);
		
		for(new id = 0; id < izcmd_cmd_total_count; id++)
	    {
	        if(id == ocmdid) continue;
			if(id == cmdid) continue;
			
			if(izcmd_cmd_id_pidx_map[id] == cmdid)
            {
                _SetCommandProperty(ocmdid, COMMAND_HAS_ALTERNATE);
                break;
			}
		}
	}

	new ocmdid = izcmd_cmd_idx_id_map[idx_funcname], oaddr, caddr;
	if(ocmdid == INVALID_COMMAND_ID)
	{
    	#emit LOAD.pri offsetPFT
		#emit LOAD.S.alt  idx_funcname
		#emit SHL.C.alt   3
		#emit ADD
		#emit STOR.S.pri  oaddr
		#emit LREF.S.pri  oaddr
		#emit STOR.S.pri  oaddr

        izcmd_cmd_id_pidx_map[cmdid] = idx_funcname;
        izcmd_cmd_id_pid_map[cmdid] = cmdid;
	   	izcmd_cmd_paddress[cmdid] = oaddr;
	}
	else
	{
	    if(updateCID)
	        izcmd_cmd_OPTC_idx_id[izcmd_cmd_id_oidx_map[cmdid]] = ocmdid;

	    izcmd_cmd_id_pidx_map[cmdid] = idx_funcname;
    	izcmd_cmd_id_pid_map[cmdid] = ocmdid;
    	oaddr = izcmd_cmd_paddress[cmdid] = izcmd_cmd_paddress[ocmdid];
	}
 
	if(updatePFT)
	{
		#emit CONST.alt izcmd_cmd_id_oidx_map
		#emit LOAD.S.pri cmdid
		#emit LIDX

		#emit SHL.C.pri   3
		#emit LOAD.alt offsetPFT
		#emit ADD
		#emit STOR.S.pri  caddr
		#emit LOAD.S.pri oaddr
		#emit SREF.S.pri  caddr
	}
	return 1;
}

/*******************************************************************************************************************************
	<summary>EmulateCommand</summary>
	<para>Emulates a command (fakes a player sending a command)</para>
	<param name="playerid">player id (sender)</param>
	<param name="cmdtext">text typed by the player (sender) into the message bar</param>
	<returns>
		Returns the value returned by OnPlayerCommandPerformed (if it existed) or the value returned by the command function
		or the value returned by OnPlayerCommandRecieved if the command was invalid and OnPlayerCommandPerformed does not exist.
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
	if(OnPlayerCommandText(playerid, cmdtext))
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
		Returns the value returned by OnPlayerCommandPerformed (if it existed) or the value returned by the command function
		or the value returned by OnPlayerCommandRecieved if the command was invalid and OnPlayerCommandPerformed does not exist.
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
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;

	new cmdtext[MAX_CLIENT_MSG_LENGTH], pos, retval;
	cmdtext[0] = '/';

	GetCommandName(cmdid, cmdtext[1], MAX_CLIENT_MSG_LENGTH);

	if(!isnull(params))
	{
	    strcat(cmdtext, " ");
	    strcat(cmdtext, params);
	    while (params[pos] == ' ') pos++;
	}

	if(_IsCommandPropertySet(cmdid, COMMAND_ENABLED))
	{
	    #if defined OnPlayerCommandReceived
	  		if(!OnPlayerCommandReceived(cmdid, playerid, cmdtext)) return 1; {}
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

	    #emit CONST.alt izcmd_cmd_paddress
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
    	if(!OnPlayerCommandReceived(INVALID_COMMAND_ID, playerid, cmdtext)) return 1;
	#endif

	#if defined OnPlayerCommandPerformed
		return OnPlayerCommandPerformed(INVALID_COMMAND_ID, playerid, cmdtext, 0);
	#else
    	return 0;
	#endif
}
/******************************************************************************************************************************/
stock _izcmdp_register_alias(ocmdid, acmdid)
{
 	new	aidx = izcmd_cmd_id_oidx_map[acmdid], aaddr, oaddr;

    _SetCommandProperty(ocmdid, COMMAND_HAS_ALTERNATE);
	_SetCommandProperty(acmdid, COMMAND_IS_ALTERNATE);

    izcmd_cmd_OPTC_idx_id[aidx] = ocmdid;
    izcmd_cmd_id_pidx_map[acmdid] = izcmd_cmd_idx_id_map[ocmdid];
    izcmd_cmd_id_pid_map[acmdid] = ocmdid;

	oaddr = izcmd_cmd_paddress[acmdid] = izcmd_cmd_paddress[ocmdid];

 	#emit LOAD.pri offsetPFT
	#emit LOAD.S.alt  aidx
	#emit SHL.C.alt   3
	#emit ADD
	#emit STOR.S.pri  aaddr
	#emit LOAD.S.pri oaddr
	#emit SREF.S.pri  aaddr
    return 0;
}
/******************************************************************************************************************************/
