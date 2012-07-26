/********************************************************************
	author:	Anton Tsvetinsky
*********************************************************************/

#include "stdafx.h"
#include "FOserv.h"

int CServer::InitScriptSystem()
{
	LogExecStr("Script System Init...");

	if(!SS_StartLog())
	{
		LogExecStr("Инициализация лога скриптовой системы провалилась\n");
		return 0;
	}

//INIT
	ss=asCreateScriptEngine(ANGELSCRIPT_VERSION);

	if(!ss)
	{
		LogExecStr("asCreateScriptEngine FALSE\n");
		return 0;
	}

	RegisterScriptString(ss);

	SS_WriteLog(string("Start Logging Script System\n==================================="));

	ss->SetMessageCallback(asFUNCTION(CServer::SS_Callback),0,asCALL_CDECL);

//CONFIGURE =====================================================================================

	if(ss->RegisterGlobalFunction("void Log(string &in)",asFUNCTION(CServer::SS_WriteLog),asCALL_CDECL)<0) return 0;

	if(ss->RegisterGlobalFunction("int FindTarget(int crit, uint8 flags)",asFUNCTION(CServer::Crit_FindTarget),asCALL_CDECL)<0) return 0;
	if(ss->RegisterGlobalFunction("int FreeTarget(int crit)",asFUNCTION(CServer::Crit_FreeTarget),asCALL_CDECL)<0) return 0;
	if(ss->RegisterGlobalFunction("int IsTarget(int crit)",asFUNCTION(CServer::Crit_IsTarget),asCALL_CDECL)<0) return 0;
	if(ss->RegisterGlobalFunction("int IsSelfTarget(int crit)",asFUNCTION(CServer::Crit_IsSelfTarget),asCALL_CDECL)<0) return 0;
	if(ss->RegisterGlobalFunction("int MoveToTarget(int crit)",asFUNCTION(CServer::Crit_MoveToTarget),asCALL_CDECL)<0) return 0;
	if(ss->RegisterGlobalFunction("int PossibleToAttack(int crit)",asFUNCTION(CServer::Crit_PossibleToAttack),asCALL_CDECL)<0) return 0;
	if(ss->RegisterGlobalFunction("int AttackTarget(int crit)",asFUNCTION(CServer::Crit_AttackTarget),asCALL_CDECL)<0) return 0;
	if(ss->RegisterGlobalFunction("int IsBusy(int crit)",asFUNCTION(CServer::Crit_IsBusy),asCALL_CDECL)<0) return 0;
	if(ss->RegisterGlobalFunction("int Wait(int crit, int ms)",asFUNCTION(CServer::Crit_Wait),asCALL_CDECL)<0) return 0;
	if(ss->RegisterGlobalFunction("int MoveRandom(int crit)",asFUNCTION(CServer::Crit_MoveRandom),asCALL_CDECL)<0) return 0;

	if(ss->RegisterGlobalFunction("int GetStat(int crit, uint8 stat)",asFUNCTION(CServer::Crit_GetStat),asCALL_CDECL)<0) return 0;
	if(ss->RegisterGlobalFunction("int GetSkill(int crit, uint8 skill)",asFUNCTION(CServer::Crit_GetSkill),asCALL_CDECL)<0) return 0;
	if(ss->RegisterGlobalFunction("int GetPerk(int crit, uint8 perk)",asFUNCTION(CServer::Crit_GetPerk),asCALL_CDECL)<0) return 0;

/*
	if(ss->RegisterGlobalFunction("int FindTarget(int crit, uint8 flags)",asMETHOD(CServer,Crit_FindTarget),asCALL_STDCALL)<0) return 0;
	if(ss->RegisterGlobalFunction("int FreeTarget(int crit)",asMETHOD(CServer,Crit_FreeTarget),asCALL_STDCALL)<0) return 0;
	if(ss->RegisterGlobalFunction("int IsTarget(int crit)",asMETHOD(CServer,Crit_IsTarget),asCALL_STDCALL)<0) return 0;
	if(ss->RegisterGlobalFunction("int IsSelfTarget(int crit)",asMETHOD(CServer,Crit_IsSelfTarget),asCALL_STDCALL)<0) return 0;
	if(ss->RegisterGlobalFunction("int MoveToTarget(int crit)",asMETHOD(CServer,Crit_MoveToTarget),asCALL_STDCALL)<0) return 0;
	if(ss->RegisterGlobalFunction("int PossibleToAttack(int crit)",asMETHOD(CServer,Crit_PossibleToAttack),asCALL_STDCALL)<0) return 0;
	if(ss->RegisterGlobalFunction("int AttackTarget(int crit)",asMETHOD(CServer,Crit_AttackTarget),asCALL_STDCALL)<0) return 0;
	if(ss->RegisterGlobalFunction("int IsBusy(int crit)",asMETHOD(CServer,Crit_IsBusy),asCALL_STDCALL)<0) return 0;
	if(ss->RegisterGlobalFunction("int Wait(int crit, int ms)",asMETHOD(CServer,Crit_Wait),asCALL_STDCALL)<0) return 0;
	if(ss->RegisterGlobalFunction("int MoveRandom(int crit)",asMETHOD(CServer,Crit_MoveRandom),asCALL_STDCALL)<0) return 0;

	if(ss->RegisterGlobalFunction("int GetStat(int crit, uint8 stat)",asMETHOD(CServer,Crit_GetStat),asCALL_STDCALL)<0) return 0;
	if(ss->RegisterGlobalFunction("int GetStat(int crit, uint8 skill)",asMETHOD(CServer,Crit_GetSkill),asCALL_STDCALL)<0) return 0;
	if(ss->RegisterGlobalFunction("int GetStat(int crit, uint8 perk)",asMETHOD(CServer,Crit_GetPerk),asCALL_STDCALL)<0) return 0;
*/
/*
	//critter
	if(ss->RegisterObjectType("Critter",sizeof(crit_info),asOBJ_CLASS)<0)
	{
		LogExecStr("RegisterObjectType Critter FALSE\n");
		return 0;
	}

	if(ss->RegisterObjectProperty("Critter","uint16 x",offsetof(crit_info,x))<0)
	{
		LogExecStr("RegisterObjectProperty x FALSE\n");
		return 0;
	}

	if(ss->RegisterObjectProperty("Critter","uint16 y",offsetof(crit_info,y))<0)
	{
		LogExecStr("RegisterObjectProperty y FALSE\n");
		return 0;
	}

	if(ss->RegisterObjectMethod("Critter","int FreeTarget()",asMETHOD(CServer,Crit_FreeTarget),asCALL_THISCALL)<0)
	{
		LogExecStr("RegisterObjectMethod FreeTarget FALSE\n");
		return 0;
	}
*/
//LOAD SCRIPTS ==================================================================================

	FILE* fs=NULL;
	char str_path[256];

	sprintf(str_path,"%sscripts.lst",PATH_SCRIPTS);
	if(!(fs=fopen(str_path,"rb")))
	{
		LogExecStr("Не удалось открыть файл со списком скриптов\n");
		ss->Release();
		return 0;
	}

	LogExecStr("Считываем скрипты:\n");

	char ch=0;
	char str_name[256];
	while(!feof(fs))
	{
		fscanf(fs,"%c",&ch);
		if(ch!='$') continue;

		LogExecStr("\t");

		fscanf(fs,"%s",&str_name[0]);

		if(!str_name)
		{
			LogExecStr("Не удалось считать имя скрипта\n");
			return 0;
		}

		LogExecStr("%s...",str_name);

		if(!SS_LoadScript(str_name))
		{
			LogExecStr("Не удалось загрузить скрипт |%s|\n",str_name);
			ss->Release();
			return 0;
		}

		LogExecStr("OK\n");
	}

	if(ss->Build(NULL)<0)
	{
		LogExecStr("Can't build script module. See FOss.log\n");
		return 0;
	}

	LogExecStr("Script System Init OK\n");


	asIScriptContext *ctx=NULL;
	
	if(!(ctx=ss->CreateContext()))
	{
		LogExecStr("e1\n");
		return 0;
	}

	int funcId=ss->GetFunctionIDByName(NULL,"main");

	if(!funcId)
	{
		LogExecStr("e2\n");
		return 0;
	}

	if(ctx->Prepare(funcId)<0)
	{
		LogExecStr("e3\n");
		return 0;
	}

	if(ctx->Execute()<0)
	{
		LogExecStr("e4\n");
		return 0;
	}

	ctx->Release();


	return 1;
}

void CServer::FinishScriptSystem()
{
	SS_EndLog();
}

int CServer::SS_LoadScript(char* sname)
{
	FILE* fs=NULL;
	char str_path[256];

	sprintf(str_path,"%s%s.fos",PATH_SCRIPTS,sname);
	if(!(fs=fopen(str_path,"rb"))) return 0;

	fseek(fs,0,SEEK_END);
	int len=ftell(fs);
	fseek(fs,0,SEEK_SET);

	string script;
	script.resize(len);
	if(!fread(&script[0],len,1,fs))
	{
		fclose(fs);
		return 0;
	}
	fclose(fs);

	if(ss->AddScriptSection(NULL,sname,&script[0],len,0,true)<0) return 0;

//	if(ss->Build(NULL)<0) return 0;

	return 1;
}

HANDLE CServer::hSSLogFile=NULL;
int CServer::SS_StartLog()
{
	if(hSSLogFile) return 1;

	hSSLogFile=CreateFile(".\\FOss.log",GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_FLAG_WRITE_THROUGH,NULL);

	if(hSSLogFile==INVALID_HANDLE_VALUE) return 0;

	return 1;
}

void CServer::SS_EndLog()
{
	if(!hSSLogFile) return;

	CloseHandle(hSSLogFile);
	hSSLogFile=NULL;
}

void CServer::SS_WriteLog(string& str)
{
//	LogExecStr("=== From Script System ===>|%s|<===\n",str.c_str());

	if(!hSSLogFile) return;

//	if(!logging) return;
//
//	switch(logging)
//	{
//	case 1:

	str+="\n";

	DWORD br;
	WriteFile(hSSLogFile,str.c_str(),str.length(),&br,NULL);

//		break;
//	case 2:
//		sql.WriteLog(str);
//		break;
//	}*/
}

void CServer::SS_Callback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";

	char str[2048];
	sprintf(str,"%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message);

	SS_WriteLog(string(str));

//	LogExecStr("handle=%d\n",hSSLogFile);
//	LogExecStr("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}
