#ifndef _INCLUDE_UTRAP_HANDLER_H
#define _INCLUDE_UTRAP_HANDLER_H
#include <asm/dasics_ucontext.h>
#include <dasics_link.h>

// judge whether fine exception hit   
#define NO_HIT 0
#define HIT 1
#define HIT_ERROR 2

#define DasicsUFetchFault 0x18UL
#define DasicsULoadFault 0x1aUL
#define DasicsUStoreFault 0x1cUL
#define DasicsEcallFault 0x1eUL


/*
 * Three types of U-exception types
 */
int handle_DasicsUFetchFault(regs_context_t * r_regs);
int handle_DasicsULoadFault(regs_context_t * r_regs);
int handle_DasicsUStoreFault(regs_context_t * r_regs);
int handle_DasicsUEcallFault(regs_context_t * r_regs);

// Fine grit deal
int deal_plt(umain_got_t * entry, regs_context_t * r_regs);
int deal_jump_to_plt(umain_got_t * entry, regs_context_t * r_regs);

#endif