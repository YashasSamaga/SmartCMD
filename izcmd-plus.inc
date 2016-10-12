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

#define CMD_DEFAULT_FLAG%9_@REMOVE@_(%1) %1
#define _@REMOVE@_

#define CMD%0:%1(%2) \
			public flg_%1=CMD_DEFAULT_FLAG _@REMOVE@_%0 ; \
			stock flg@%1=flg_%1; \
			forward cmd_%1(%2); \
			public cmd_%1(%2)

#define COMMAND%0:%1(%2) \
			CMD%0:%1(%2)

#define command%0(%1,%2,%3,%4) \
			COMMAND%0:%1(%2, %3, %4)

#define cmd%0(%1,%2,%3,%4) \
			COMMAND%0:%1(%2, %3, %4)

#define ALT:%0:%1; \
  		public alt_%1= 0; \
		stock alt@%1=alt_%1; \
	 	forward cmd_%1(cmdid, playerid, params[]);\
		public cmd_%1(cmdid, playerid, params[])\
			return _izcmdp_register_alias(#%0, cmdid);

#define CMD_SUCCESS 1
#define CMD_FAILURE 0

#define INVALID_COMMAND_ID 			(-1)

#define MAX_CLIENT_MSG_LENGTH 		(144)
#define MAX_FUNC_NAME 				(32)

#if !defined MAX_PUBLIC_FUNCTIONS
	#define MAX_PUBLIC_FUNCTIONS 	(2048)
#endif

#if !defined MAX_COMMANDS
	#define MAX_COMMANDS 			(500)
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
static izcmd_cmd_idx_id_map[MAX_PUBLIC_FUNCTIONS] = {INVALID_COMMAND_ID, INVALID_COMMAND_ID, INVALID_COMMAND_ID, ...};
static izcmd_cmd_id_idx_map[MAX_COMMANDS] = {-1, -1, -1, ...};

static bool:izcmd_cmd_enabled[MAX_COMMANDS] = {false, false, false, ...};
static izcmd_cmd_address[MAX_COMMANDS] = {-1, -1, -1, ...};
static izcmd_cmd_flags[MAX_COMMANDS] = {0, 0, 0, ...};

static izcmd_cmd_total_count = 0;
static izcmd_cmd_enabled_count = 0;

new offsetDAT, offsetPFT, offsetNFT, offsetPVT, offsetTAGS, number_of_publics, number_of_pubvars;
/******************************************************************************************************************************/
static izcmd_Init()
{
	new addr, chr, cmd_name[MAX_FUNC_NAME] = "cmd_", bool:crash = false;

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
        #emit STOR.S.pri chr

		if((chr & 0xFF) != 'c')	continue; 	{}

		#emit INC.S addr
		#emit LREF.S.pri addr
        #emit STOR.S.pri chr

		if((chr & 0xFF) != 'm') continue;	{}
		
        #emit INC.S addr
		#emit LREF.S.pri addr
        #emit STOR.S.pri chr

		if((chr & 0xFF) != 'd') continue;	{}
		
  		#emit INC.S addr
		#emit LREF.S.pri addr
        #emit STOR.S.pri chr

		if((chr & 0xFF) != '_') continue;	{}
		
  		#emit LOAD.pri offsetPFT
		#emit LOAD.S.alt  pubfunc_idx
		#emit SHL.C.alt   3
		#emit ADD
		#emit STOR.S.pri  addr
		#emit LREF.S.pri  addr
		#emit STOR.S.pri  addr

		izcmd_cmd_idx_id_map[pubfunc_idx] = izcmd_cmd_total_count;
		izcmd_cmd_id_idx_map[izcmd_cmd_total_count] = pubfunc_idx;
		
		izcmd_cmd_enabled[izcmd_cmd_total_count] = true;
		izcmd_cmd_address[izcmd_cmd_total_count] = addr;
				
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
        #emit STOR.S.pri chr

		if((chr & 0xFF) == 'f')
		{
            #emit INC.S addr
			#emit LREF.S.pri addr
	        #emit STOR.S.pri chr

			if((chr & 0xFF) != 'l') continue;	{}

	        #emit INC.S addr
			#emit LREF.S.pri addr
	        #emit STOR.S.pri chr

			if((chr & 0xFF) != 'g') continue;	{}

			#emit INC.S addr
			#emit LREF.S.pri addr
	        #emit STOR.S.pri chr

			if((chr & 0xFF) != '_') continue;	{}

		    for(new j = 4; j < MAX_FUNC_NAME; j++)
		    {
		        #emit INC.S addr
				#emit LREF.S.pri addr
		        #emit STOR.S.pri chr
		        
		        cmd_name[j] = chr = (chr & 0xFF);

				if(chr == EOS)
				{
				    new cmd_idx = funcidx(cmd_name), flgs;
					if(cmd_idx == -1) break;
					if(cmd_idx >= MAX_PUBLIC_FUNCTIONS) break; {}

       				#emit LOAD.pri offsetPVT
					#emit LOAD.S.alt  pubvar_idx
					#emit SHL.C.alt   3
					#emit ADD
					#emit STOR.S.pri  flgs
					#emit LREF.S.pri  flgs
					#emit LOAD.I
					#emit STOR.S.pri  flgs
					
				    izcmd_cmd_flags[izcmd_cmd_idx_id_map[cmd_idx]] = flgs;
				    break;
				}
			}
	    }
	    else if((chr & 0xFF) == 'a')
		{
            #emit INC.S addr
			#emit LREF.S.pri addr
	        #emit STOR.S.pri chr

			if((chr & 0xFF) != 'l') continue;	{}

	       	#emit INC.S addr
			#emit LREF.S.pri addr
	        #emit STOR.S.pri chr

			if((chr & 0xFF) != 't') continue;	{}

			#emit INC.S addr
			#emit LREF.S.pri addr
	        #emit STOR.S.pri chr

			if((chr & 0xFF) != '_') continue;	{}

		    for(new j = 4; j < MAX_FUNC_NAME; j++)
		    {
                #emit INC.S addr
				#emit LREF.S.pri addr
		        #emit STOR.S.pri chr

		        cmd_name[j] = chr = (chr & 0xFF);

				if(chr == EOS)
				{
				    new cmd_idx = funcidx(cmd_name);
					if(cmd_idx == -1) break;
					if(cmd_idx >= MAX_PUBLIC_FUNCTIONS) break; {}

					new cmdid = izcmd_cmd_idx_id_map[cmd_idx];
					
				    #emit PUSH.S cmd_name //doesn't matter what is pushed
					#emit PUSH.C 0
					#emit PUSH.S cmdid
					#emit PUSH.C 12

					#emit LCTRL      6
					#emit ADD.C      40
					#emit PUSH.pri
		    		#emit CONST.alt izcmd_cmd_address
		    		#emit LOAD.S.pri cmdid
					#emit LIDX
					#emit SCTRL      6

				    break;
				}
			}
		}
	}
	if(crash)
	{
	    new x = MAX_COMMANDS;
		return izcmd_cmd_address[x];
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
		if(((cmdid = izcmd_cmd_idx_id_map[tmp]) != INVALID_COMMAND_ID) && izcmd_cmd_enabled[cmdid])
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

		    #emit CONST.alt izcmd_cmd_address
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
/*
    native DoesCommandExist(cmd[]);
	native GetCommandID(const cmd[]);
	native GetCommandName(cmdid, cmd[]);
	native GetFuncidxFromCommandID(cmdid);
	native GetCommandIDFromFuncidx(idx);
	native GetCommandFuncname(dest[], cmdid, len = sizeof(dest))
	native GetEnabledCommandCount();
	native GetDisabledCommandCount();
	native GetTotalCommandCount();
	native EnableCommand(cmdid);
	native DisableCommand(cmdid);
	native IsCommandEnabled(cmdid);
	native SetCommandFlags(cmdid, flags);
	native GetCommandFlags(cmdid);
	native ReassignCommandFunction(cmdid, const funcname[], bool:updateCID = true, bool:updatePFT = false)
	native EmulateCommand(playerid, cmdtext[]);
	native EmulateCommandEx(cmdid, playerid, params[]);
*/
#define DoesCommandExist(%0) (GetCommandID(%0) != INVALID_COMMAND_ID)
stock GetCommandID(const cmd[])
{
    static funcname[MAX_FUNC_NAME] = "cmd_";
    funcname[4] = 0;
	strcat(funcname, cmd);

	new idx = funcidx(funcname);

	if(idx == -1) return INVALID_COMMAND_ID;
	return izcmd_cmd_idx_id_map[idx];
}
stock GetCommandName(cmdid, cmd[], len = sizeof(cmd))
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return 0;

	new addr, chr, idx = izcmd_cmd_id_idx_map[cmdid];

    addr = (idx << 3) + offsetPFT + 4;

	#emit LREF.S.pri addr
    #emit LOAD.alt offsetDAT
    #emit ADD
	#emit ADD.C 4
    #emit STOR.S.pri addr

	for(new i = 0;i < len;i++)
	{
       	#emit LREF.S.pri addr
        #emit STOR.S.pri chr
		#emit INC.S addr

        cmd[i] = chr = (chr & 0xFF);
        if(chr == EOS)
			return 1;
	}
	return 1;
}
stock GetFuncidxFromCommandID(cmdid)
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	return izcmd_cmd_id_idx_map[cmdid];
}
stock GetCommandIDFromFuncidx(idx)
{
    if(!(0 <= idx < number_of_publics)) return INVALID_COMMAND_ID;
	return izcmd_cmd_idx_id_map[idx];
}
stock GetEnabledCommandCount()
{
	return izcmd_cmd_enabled_count;
}
stock GetDisabledCommandCount() 
{
	return izcmd_cmd_total_count - izcmd_cmd_enabled_count;
}
stock GetCommandFuncname(dest[], cmdid, len = sizeof(dest))
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	dest[0] = 0;
	strcat(dest, "cmd_", len);
	GetCommandName(cmdid, dest[4], len-4);
	return 1;
}
stock GetTotalCommandCount()
{
	return izcmd_cmd_total_count;
}
stock EnableCommand(cmdid)
{
	if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	if(izcmd_cmd_enabled[cmdid]) return 0;

    izcmd_cmd_enabled_count++;
	izcmd_cmd_enabled[cmdid] = true;
	return 1;
}
stock DisableCommand(cmdid)
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
	if(!izcmd_cmd_enabled[cmdid]) return 0;

    izcmd_cmd_enabled_count--;
	izcmd_cmd_enabled[cmdid] = false;
	return 1;
}
stock bool:IsCommandEnabled(cmdid)
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return false;
	return izcmd_cmd_enabled[cmdid];
}
stock SetCommandFlags(cmdid, flags)
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return 0;

    izcmd_cmd_flags[cmdid] = flags;
    return 1;
}
stock GetCommandFlags(cmdid)
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return 0;
    return izcmd_cmd_flags[cmdid];
}

stock ReassignCommandFunction(cmdid, const funcname[], bool:updateCID = true, bool:updatePFT = false)
{
    if(!(0 <= cmdid < izcmd_cmd_total_count)) return INVALID_COMMAND_ID;
    
	new idx_funcname = funcidx(funcname), addr_funcname, addr_cmd;
	new idx_cmd = izcmd_cmd_id_idx_map[cmdid];
	
	if(idx_funcname == -1) return 0;

    if(izcmd_cmd_idx_id[idx_funcname] != INVALID_COMMAND_ID)
    {
        addr_funcname = izcmd_cmd_address[idx_funcname];
       	if(updateCID)
	    	izcmd_cmd_idx_id_map[idx_cmd] = izcmd_cmd_idx_id_map[idx_funcname];
    }
    else
    {
        #emit LOAD.pri offsetPFT
		#emit LOAD.S.alt  idx
		#emit SHL.C.alt   3
		#emit ADD
		#emit STOR.S.pri  addr_funcname
		#emit LREF.S.pri  addr_funcname
		#emit STOR.S.pri  addr_funcname
    }

	izcmd_cmd_address[cmdid] = addr_funcname;

	if(updatePFT)
	{
	 	#emit LOAD.pri offsetPFT
		#emit LOAD.S.alt  idx_cmd
		#emit SHL.C.alt   3
		#emit ADD
		#emit STOR.S.pri  addr_cmd
		#emit LOAD.S.pri addr_funcname
		#emit SREF.S.pri  addr_cmd
	}
	return 1;
}
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

	if(izcmd_cmd_enabled[cmdid])
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

	    #emit CONST.alt izcmd_cmd_address
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
stock _izcmdp_register_alias(const cmd[], cmdid_alias)
{
 	new cmdid_cmd = GetCommandID(cmd), addr_cmd, addr_alias;
	new idx_alias = izcmd_cmd_id_idx_map[cmdid_alias];
	
	addr_cmd = izcmd_cmd_address[cmdid_cmd];
	
 	#emit LOAD.pri offsetPFT
	#emit LOAD.S.alt  idx_alias
	#emit SHL.C.alt   3
	#emit ADD
	#emit STOR.S.pri  addr_alias
	#emit LOAD.S.pri addr_cmd
	#emit SREF.S.pri  addr_alias

    izcmd_cmd_idx_id_map[idx_alias] = cmdid_cmd;
    return 0;
}
/******************************************************************************************************************************/
