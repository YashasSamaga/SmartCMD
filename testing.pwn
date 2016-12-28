#include <a_samp>

//#define IZCMD_ENABLE_CASE_SENSITIVITY
#define CMD_DEFAULT_FLAG 133
#include <izcmd>
/*******************************************************************************************************/
#define ITR 100000
/*******************************************************************************************************/
enum  (*=2)
{
	ADMIN_COMMAND_FLAG = 1,
	TEST1,
	TEST2
}
CMD<ADMIN_COMMAND_FLAG>:aaaa[params](cmdid, playerid, params[])
{
	printf("[AAAA]params cmdid:%d playerid:%d params:%s", cmdid, playerid, params);
	return CMD_SUCCESS;
}
CMD<ADMIN_COMMAND_FLAG>:aaaa[help](cmdid, playerid, params[])
{
	printf("[AAAA]help cmdid:%d playerid:%d params:%s", cmdid, playerid, params);
	return CMD_SUCCESS;
}
CMD<ADMIN_COMMAND_FLAG>:aaaa(cmdid, playerid, params[])
{
	flg_aaaa = flags:0;
	//printf("[AAAA]normal cmdid:%d playerid:%d params:%s", cmdid, playerid, params);
	return 1;
}
ALT:altname = CMD:aaaa;
/*******************************************************************************************************/
main()
{
 	//---TESTING---//
	if(DoesCommandExist("aaaa") == false) print("[TEST1] DoesCommandExist failed.");
	if(DoesCommandExist("aa1234567aa") == true) print("[TEST2] DoesCommandExist failed.");
	if(GetCommandID("aaaa") != cid_aaaa) print("[TEST3] GetCommandID failed.");
	if(GetCommandID("aa1234567aa") != -1) print("[TEST4] GetCommandID failed.");

	new str[128];
	GetCommandName(3, str);
	if(strcmp(str, "aaaa")) print("[TEST5] GetCommandName failed.");

	if(GetAlternateCommands(cid_aaaa, str) != 1) print("[TEST5] GetAlternateCommands failed.");
	if(cid_altname != str[0]) print("[TEST6] GetAlternateCommands failed.");

	if(IsCommandAlternate(cid_aaaa)) print("[TEST7] IsAlternateCommand failed.");
	if(!IsCommandAlternate(cid_altname)) print("[TEST7] IsAlternateCommand failed.");

	if(GetCommandFunctionID(cid_aaaa) != funcidx("cmd_aaaa")) print("[TEST8] GetCommandFunctionID failed.");
	if(GetPointingCommandFunctionID(cid_altname) != funcidx("cmd_aaaa")) print("[TEST9] GetPointingCommandFunctionID failed.");
	if(GetPointingCommandID(cid_altname) != cid_aaaa) print("[TEST10] GetPointingCommandID failed.");

	GetCommandFunctionName(cid_aaaa, str);
	if(strcmp(str, "cmd_aaaa")) print("[TEST11] GetCommandFunctionName failed.");

	printf("Enabled Commands:%d Disabled Commands:%d Total Commands:%d", GetEnabledCommandCount(), GetDisabledCommandCount(), GetTotalCommandCount());

	if(EnableCommand(1) != 0) print("[TEST12] EnableCommand failed.");
	if(DisableCommand(1) != 1) print("[TEST13] DisableCommand failed.");
	if(IsCommandEnabled(1)) print("[TEST14] IsCommandEnabled failed.");
	if(GetDisabledCommandCount() != 1) print("[TEST15] GetDisabledCommandCount failed.");
	if(EnableCommand(1) != 1) print("[TEST16] EnableCommand failed.");

	if(GetCommandFlags(cid_aaaa) != ADMIN_COMMAND_FLAG) printf("[TEST17] GetCommandFlags failed. %d");

	new success;
	ExecuteCommand("aaaa", help, 0, success, "");

	//ReassignCommandFunction(cid_aaaa, "cmd_aaab", true);
	//EmulateCommand(10, "/aaaa testX");
	//EmulateCommandEx(cid_aaaa, 10, "this is test params");
	//SetPointingCommandIDToSelf(cid_aaaa);
	//---TESTING---//

	new Float:avg;
	for(new j = 0; j < 10; j++)
	{
		new a = GetTickCount();

		for(new i = 0; i < ITR; i++)
		{
			OnPlayerCommandText(0, "/aaaa test");
			OnPlayerCommandText(0, "/aaaz param");
			OnPlayerCommandText(0, "/altname altname");
			OnPlayerCommandText(100, "/usdfsdfasdfux param");
			OnPlayerCommandText(200, "/iNVAlidCommaNDwithCAPS");
			OnPlayerCommandText(0, "/number1 param");
			OnPlayerCommandText(300, "/abcdefghijklmnopqrsttt");
			OnPlayerCommandText(200, "/invalid this is an invalid command");
		}
		new b = GetTickCount();

		printf("%d", b-a);
		avg += float(b-a);
	}
	printf("Average: %.2f", avg/10); //1040, 930
}
/*******************************************************************************************************/
public OnPlayerCommandReceived(cmdid, playerid, cmdtext[])
{
	//printf("[OPCR] cmdid:%d playerid:%d cmdtext:%s", cmdid, playerid, cmdtext);
	return CMD_SUCCESS;
}
public OnPlayerCommandPerformed(cmdid, playerid, cmdtext[], success)
{
	//printf("[OPCP] cmdid:%d playerid:%d cmdtext:%s success:%d", cmdid, playerid, cmdtext, success);
	return success;
}
/*******************************************************************************************************/
CMD:aaab(cmdid, playerid, params[])
{
	//printf("Hi from AAAB %s", params);
	return CMD_SUCCESS;
}
CMD:aaac(cmdid, playerid, params[])
{
	return CMD_FAILURE;
}
CMD:aaad(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaae(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaaf(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaag(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaah(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaai(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaaj(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaak(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaal(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaam(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaan(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaao(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaap(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaaq(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaar(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaas(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaat(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaau(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaav(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaaw(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaax(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaay(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaaz(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:bbbb(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cccc(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:dddd(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:eeee(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ffff(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gggg(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:hhhh(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:iiii(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jjjj(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:kkkk(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:llll(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:mmmm(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:nnnn(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
//
CMD:oooo(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:pppp(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:qqqq(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:rrrr(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ssss(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:tttt(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:uuuu(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:vvvv(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:wwww(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:xxxx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:yyyy(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:zzzz(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:abcedfghijklmnopqrstuvwxyz(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:a_b_c_d_e_f(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber1(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber1(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber1(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber1(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber1(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber1(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:rnumber1(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:vnumber1(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:znumber1(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber10(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:anumber11(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber12(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber13(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber14(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber15(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber16(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber17(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber18(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber19(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber10(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:cnumber11(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber12(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber13(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber14(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber15(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber16(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber17(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber18(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber19(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber10(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:gnumber11(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber12(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber13(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber14(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber15(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber16(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber17(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber18(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber19(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber10(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:anumber_fix(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber12(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber13(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber14(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber15(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber16(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber17(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber18(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber19(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber10(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:knumber11(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber12(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber13(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber14(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber15(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber16(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber17(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber18(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber19(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber10(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:onumber11(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber12(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber13(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber14(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber15(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber16(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber17(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber18(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber19(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:rnumber10(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:rnumber11(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:rnumber12(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aaaax(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:bbbbx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ccccx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ddddx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:eeeex(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ffffx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ggggx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:hhhhx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:iiiix(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jjjjx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:kkkkx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:llllx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:mmmmx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:nnnnx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:oooox(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ppppx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:qqqqx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:rrrrx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ssssx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ttttx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:uuuux(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:vvvvx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:wwwwx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:xxxxx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:yyyyx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:zzzzx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:abcedfghijklmnopqrstuvwxyzx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:a_b_c_d_e_fx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber1x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber1x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber1x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber1x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber1x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber1x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:rnumber1x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:vnumber1x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:znumber1x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber10x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:anumber11x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber12x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber13x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber14x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber15x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber16x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber17x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber18x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:anumber19x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber10x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:cnumber11x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber12x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber13x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber14x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber15x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber16x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber17x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber18x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cnumber19x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber10x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:gnumber11x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber12x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber13x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber14x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber15x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber16x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber17x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber18x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gnumber19x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber10x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:anumber_fixx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber12x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber13x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber14x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber15x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber16x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber17x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber18x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jnumber19x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber10x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:knumber11x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber12x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber13x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber14x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber15x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber16x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber17x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber18x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:knumber19x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber10x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:onumber11x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber12x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber13x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber14x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber15x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumwqeber16x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber17x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onumber18x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:onasdumber19x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:rnuqwember10x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}CMD:rnumber11x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:rnqweumber12x(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}

CMD:aaa2(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:bqwesebbx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:cccx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ddqwqwedx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:eeex(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:awdfffx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:gggawdqx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:hhhx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:iiasdix(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:jjjAWDx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:kkkx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:lasdWllx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:mmmadasdx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:aweqweasdnnnx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ooAawdasdwox(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:ppasdaDpx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:qqqASDASDWx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:rrasdrx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:sssasdadx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:tsdfsttx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:usdfsdfasdfux(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:vvfefsafsvx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:wwasdwx(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:xxxz21(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:yyyx2ssd(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
CMD:zzzx4gss(cmdid, playerid, params[])
{
	return CMD_SUCCESS;
}
