#pragma once

#define altcostume 0x80u
#define altcharacter 0x40
#define charmask ~(altcostume|altcharacter)

extern int defaultcharacters[];
extern int defaultcharacters2p[];
extern int defaultcharacters2palt[];
extern int bosscharacters[];
extern bool customSet;
void init_Config(const char* path);
void init_SetFiles();