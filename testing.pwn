#include <a_samp>

#include <izcmd>
/*******************************************************************************************************/
#define ITR 100000
#define ADMIN_COMMAND_FLAG 1234
/*******************************************************************************************************/
COMMAND<ADMIN_COMMAND_FLAG>:aaaa(cmdid, playerid, params[])
{
//	printf("[AAAA]cmdid:%d playerid:%d params:%s", cmdid, playerid, params);
	return 1;
}
ALT:aaaa:altname;
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
	if(str[0] != cid_altname) print("[TEST6] GetAlternateCommands failed.");

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

	ReassignCommandFunction(cid_aaaa, "cmd_aaab", true, true);
	EmulateCommand(10, "/aaaa testX");
	EmulateCommandEx(cid_aaaa, 10, "this is test params");

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
	return 1;
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
	return 1;
}
CMD:aaac(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaad(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaae(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaaf(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaag(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaah(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaai(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaaj(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaak(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaal(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaam(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaan(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaao(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaap(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaaq(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaar(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaas(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaat(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaau(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaav(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaaw(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaax(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaay(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaaz(cmdid, playerid, params[])
{
	return 1;
}
CMD:bbbb(cmdid, playerid, params[])
{
	return 1;
}
CMD:cccc(cmdid, playerid, params[])
{
	return 1;
}
CMD:dddd(cmdid, playerid, params[])
{
	return 1;
}
CMD:eeee(cmdid, playerid, params[])
{
	return 1;
}
CMD:ffff(cmdid, playerid, params[])
{
	return 1;
}
CMD:gggg(cmdid, playerid, params[])
{
	return 1;
}
CMD:hhhh(cmdid, playerid, params[])
{
	return 1;
}
CMD:iiii(cmdid, playerid, params[])
{
	return 1;
}
CMD:jjjj(cmdid, playerid, params[])
{
	return 1;
}
CMD:kkkk(cmdid, playerid, params[])
{
	return 1;
}
CMD:llll(cmdid, playerid, params[])
{
	return 1;
}
CMD:mmmm(cmdid, playerid, params[])
{
	return 1;
}
CMD:nnnn(cmdid, playerid, params[])
{
	return 1;
}
//
CMD:oooo(cmdid, playerid, params[])
{
	return 1;
}
CMD:pppp(cmdid, playerid, params[])
{
	return 1;
}
CMD:qqqq(cmdid, playerid, params[])
{
	return 1;
}
CMD:rrrr(cmdid, playerid, params[])
{
	return 1;
}
CMD:ssss(cmdid, playerid, params[])
{
	return 1;
}
CMD:tttt(cmdid, playerid, params[])
{
	return 1;
}
CMD:uuuu(cmdid, playerid, params[])
{
	return 1;
}
CMD:vvvv(cmdid, playerid, params[])
{
	return 1;
}
CMD:wwww(cmdid, playerid, params[])
{
	return 1;
}
CMD:xxxx(cmdid, playerid, params[])
{
	return 1;
}
CMD:yyyy(cmdid, playerid, params[])
{
	return 1;
}
CMD:zzzz(cmdid, playerid, params[])
{
	return 1;
}
CMD:abcedfghijklmnopqrstuvwxyz(cmdid, playerid, params[])
{
	return 1;
}
CMD:a_b_c_d_e_f(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber1(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber1(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber1(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber1(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber1(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber1(cmdid, playerid, params[])
{
	return 1;
}
CMD:rnumber1(cmdid, playerid, params[])
{
	return 1;
}
CMD:vnumber1(cmdid, playerid, params[])
{
	return 1;
}
CMD:znumber1(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber10(cmdid, playerid, params[])
{
	return 1;
}CMD:anumber11(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber12(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber13(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber14(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber15(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber16(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber17(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber18(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber19(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber10(cmdid, playerid, params[])
{
	return 1;
}CMD:cnumber11(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber12(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber13(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber14(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber15(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber16(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber17(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber18(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber19(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber10(cmdid, playerid, params[])
{
	return 1;
}CMD:gnumber11(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber12(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber13(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber14(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber15(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber16(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber17(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber18(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber19(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber10(cmdid, playerid, params[])
{
	return 1;
}CMD:anumber_fix(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber12(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber13(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber14(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber15(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber16(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber17(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber18(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber19(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber10(cmdid, playerid, params[])
{
	return 1;
}CMD:knumber11(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber12(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber13(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber14(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber15(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber16(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber17(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber18(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber19(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber10(cmdid, playerid, params[])
{
	return 1;
}CMD:onumber11(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber12(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber13(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber14(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber15(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber16(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber17(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber18(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber19(cmdid, playerid, params[])
{
	return 1;
}
CMD:rnumber10(cmdid, playerid, params[])
{
	return 1;
}CMD:rnumber11(cmdid, playerid, params[])
{
	return 1;
}
CMD:rnumber12(cmdid, playerid, params[])
{
	return 1;
}
CMD:aaaax(cmdid, playerid, params[])
{
	return 1;
}
CMD:bbbbx(cmdid, playerid, params[])
{
	return 1;
}
CMD:ccccx(cmdid, playerid, params[])
{
	return 1;
}
CMD:ddddx(cmdid, playerid, params[])
{
	return 1;
}
CMD:eeeex(cmdid, playerid, params[])
{
	return 1;
}
CMD:ffffx(cmdid, playerid, params[])
{
	return 1;
}
CMD:ggggx(cmdid, playerid, params[])
{
	return 1;
}
CMD:hhhhx(cmdid, playerid, params[])
{
	return 1;
}
CMD:iiiix(cmdid, playerid, params[])
{
	return 1;
}
CMD:jjjjx(cmdid, playerid, params[])
{
	return 1;
}
CMD:kkkkx(cmdid, playerid, params[])
{
	return 1;
}
CMD:llllx(cmdid, playerid, params[])
{
	return 1;
}
CMD:mmmmx(cmdid, playerid, params[])
{
	return 1;
}
CMD:nnnnx(cmdid, playerid, params[])
{
	return 1;
}
CMD:oooox(cmdid, playerid, params[])
{
	return 1;
}
CMD:ppppx(cmdid, playerid, params[])
{
	return 1;
}
CMD:qqqqx(cmdid, playerid, params[])
{
	return 1;
}
CMD:rrrrx(cmdid, playerid, params[])
{
	return 1;
}
CMD:ssssx(cmdid, playerid, params[])
{
	return 1;
}
CMD:ttttx(cmdid, playerid, params[])
{
	return 1;
}
CMD:uuuux(cmdid, playerid, params[])
{
	return 1;
}
CMD:vvvvx(cmdid, playerid, params[])
{
	return 1;
}
CMD:wwwwx(cmdid, playerid, params[])
{
	return 1;
}
CMD:xxxxx(cmdid, playerid, params[])
{
	return 1;
}
CMD:yyyyx(cmdid, playerid, params[])
{
	return 1;
}
CMD:zzzzx(cmdid, playerid, params[])
{
	return 1;
}
CMD:abcedfghijklmnopqrstuvwxyzx(cmdid, playerid, params[])
{
	return 1;
}
CMD:a_b_c_d_e_fx(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber1x(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber1x(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber1x(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber1x(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber1x(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber1x(cmdid, playerid, params[])
{
	return 1;
}
CMD:rnumber1x(cmdid, playerid, params[])
{
	return 1;
}
CMD:vnumber1x(cmdid, playerid, params[])
{
	return 1;
}
CMD:znumber1x(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber10x(cmdid, playerid, params[])
{
	return 1;
}CMD:anumber11x(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber12x(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber13x(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber14x(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber15x(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber16x(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber17x(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber18x(cmdid, playerid, params[])
{
	return 1;
}
CMD:anumber19x(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber10x(cmdid, playerid, params[])
{
	return 1;
}CMD:cnumber11x(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber12x(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber13x(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber14x(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber15x(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber16x(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber17x(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber18x(cmdid, playerid, params[])
{
	return 1;
}
CMD:cnumber19x(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber10x(cmdid, playerid, params[])
{
	return 1;
}CMD:gnumber11x(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber12x(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber13x(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber14x(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber15x(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber16x(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber17x(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber18x(cmdid, playerid, params[])
{
	return 1;
}
CMD:gnumber19x(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber10x(cmdid, playerid, params[])
{
	return 1;
}CMD:anumber_fixx(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber12x(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber13x(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber14x(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber15x(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber16x(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber17x(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber18x(cmdid, playerid, params[])
{
	return 1;
}
CMD:jnumber19x(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber10x(cmdid, playerid, params[])
{
	return 1;
}CMD:knumber11x(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber12x(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber13x(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber14x(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber15x(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber16x(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber17x(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber18x(cmdid, playerid, params[])
{
	return 1;
}
CMD:knumber19x(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber10x(cmdid, playerid, params[])
{
	return 1;
}CMD:onumber11x(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber12x(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber13x(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber14x(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber15x(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumwqeber16x(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber17x(cmdid, playerid, params[])
{
	return 1;
}
CMD:onumber18x(cmdid, playerid, params[])
{
	return 1;
}
CMD:onasdumber19x(cmdid, playerid, params[])
{
	return 1;
}
CMD:rnuqwember10x(cmdid, playerid, params[])
{
	return 1;
}CMD:rnumber11x(cmdid, playerid, params[])
{
	return 1;
}
CMD:rnqweumber12x(cmdid, playerid, params[])
{
	return 1;
}

CMD:aaa2(cmdid, playerid, params[])
{
	return 1;
}
CMD:bqwesebbx(cmdid, playerid, params[])
{
	return 1;
}
CMD:cccx(cmdid, playerid, params[])
{
	return 1;
}
CMD:ddqwqwedx(cmdid, playerid, params[])
{
	return 1;
}
CMD:eeex(cmdid, playerid, params[])
{
	return 1;
}
CMD:awdfffx(cmdid, playerid, params[])
{
	return 1;
}
CMD:gggawdqx(cmdid, playerid, params[])
{
	return 1;
}
CMD:hhhx(cmdid, playerid, params[])
{
	return 1;
}
CMD:iiasdix(cmdid, playerid, params[])
{
	return 1;
}
CMD:jjjAWDx(cmdid, playerid, params[])
{
	return 1;
}
CMD:kkkx(cmdid, playerid, params[])
{
	return 1;
}
CMD:lasdWllx(cmdid, playerid, params[])
{
	return 1;
}
CMD:mmmadasdx(cmdid, playerid, params[])
{
	return 1;
}
CMD:aweqweasdnnnx(cmdid, playerid, params[])
{
	return 1;
}
CMD:ooAawdasdwox(cmdid, playerid, params[])
{
	return 1;
}
CMD:ppasdaDpx(cmdid, playerid, params[])
{
	return 1;
}
CMD:qqqASDASDWx(cmdid, playerid, params[])
{
	return 1;
}
CMD:rrasdrx(cmdid, playerid, params[])
{
	return 1;
}
CMD:sssasdadx(cmdid, playerid, params[])
{
	return 1;
}
CMD:tsdfsttx(cmdid, playerid, params[])
{
	return 1;
}
CMD:usdfsdfasdfux(cmdid, playerid, params[])
{
	return 1;
}
CMD:vvfefsafsvx(cmdid, playerid, params[])
{
	return 1;
}
CMD:wwasdwx(cmdid, playerid, params[])
{
	return 1;
}
CMD:xxxz21(cmdid, playerid, params[])
{
	return 1;
}
CMD:yyyx2ssd(cmdid, playerid, params[])
{
	return 1;
}
CMD:zzzx4gss(cmdid, playerid, params[])
{
	return 1;
}
