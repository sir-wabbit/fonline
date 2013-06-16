#ifndef _SOCIALS_H_
#define _SOCIALS_H_

#include <mysql/mysql.h>

#include "netproto.h"

struct soc_def
{
	char cmd[30];
	char NoVictimSelf[256];
	char NoVictimAll[256];
	char VictimSelf[256];
	char VictimAll[256];
	char VictimVictim[256];
	char VictimErr[256];
	char SelfSelf[256];
	char SelfAll[256];
};

#define	SOC_NOPARAMS	0
#define	SOC_NOSELF		1
#define	SOC_PARAMSOK	2

void LoadSocials(MYSQL* mysql);
void UnloadSocials();
void ParseSymbolStr(char* symstr,char* resstr, crit_info* self, crit_info* victim);

int GetSocialId(char* cmd);

void GetSocNoStr(int socid, char* SelfStr, char* AllStr, crit_info* self);
void GetSocSelfStr(int socid, char* SelfStr, char* AllStr, crit_info* self);
void GetSocVicStr(int socid, char* SelfStr, char* VicStr, char* AllStr, crit_info* self, crit_info* victim);
void GetSocVicErrStr(int socid, char* SelfStr, crit_info* self);

int GetPossParams(int socid);

#endif
