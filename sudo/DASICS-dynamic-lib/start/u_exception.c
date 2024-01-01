#include <utrap_handler.h>
#include <dasics_link_manager.h>
#include <dasics_link.h>
#include <udasics.h>
#include <utrap.h>
/* 
 * Judge the PLT load, if we find that a load exception, we can 
 * confirm that the programmer wants to call a lib fucntion
 */
int deal_plt(umain_got_t * entry, regs_context_t * r_regs)
{
    int plt_idx;
    umain_got_t * target_got_entry;
    if ((plt_idx = _is_plt_area(r_regs->uepc, entry)) != NOEXIST)
    {   
    #ifdef DASICS_DEBUG
        my_printf("hit the plt_idx: %lx\n", plt_idx);
    #endif
        // the plt read addr
        if (r_regs->utval == \
            (uint64_t)entry->got_begin + (plt_idx + 2) * sizeof(void *))
        {
            uint64_t target = 0;
            if (entry->got_begin[plt_idx + 2] == entry->plt_begin)
            {
                /*
                * We found that the Plt[x] wants to use dely binding to find the fucntion,
                * and we prepare all the parameters, and jump
                * 
                * dll_a0: the got[1], struct link_map of the library
                * all_a1: the thrice of the plt table offset
                * ulib_func: the addr of the ulib function 
                */
                uint64_t dll_a0 = entry->got_begin[1];
                uint64_t dll_a1 = (((reg_t)plt_idx * 0x10UL) >> 1) * 3;
                uint64_t ulib_func = dll_fixup_handler(dll_a0, dll_a1);
                
                // /*
                //  * if the trap area is untrust, the target should add offset
                //  */
                // if (!(entry->_flags & MAIN_AREA))
                // {
                //     // ulib_func += target_got_entry->offset;
                // #ifdef DASICS_DEBUG
                //     my_printf("[Warning]: the untrusted area tried to call a trusted func\n");
                //     my_printf("Turned to 0x%lx\n", ulib_func);
                // #endif
                //     entry->got_begin[plt_idx + 2] = ulib_func;
                //     target_got_entry = _get_trap_area(ulib_func);
                //     debug_print_umain_map(target_got_entry);
                // } else 
                // {
                // #ifdef DASICS_DEBUG
                //     my_printf("[Warning]: the trusted area tried to call a trusted func\n");
                //     my_printf("Turned to 0x%lx\n", ulib_func);
                // #endif                    
                // }
                ulib_func = _call_reloc(entry, ulib_func);
                /* now, we get the lib func, we will switch to it */
                target = r_regs->uepc = ulib_func;
                entry->got_begin[plt_idx + 2] = target;
                target_got_entry = _get_trap_area(ulib_func);
            } else
            {
                /**
                 * Now, the got has been filled with the lib function address in the memory
                 * we will check it.
                 */
                target = r_regs->t3 = r_regs->uepc = entry->got_begin[plt_idx + 2];
                target_got_entry = _get_trap_area(target);
                #ifdef DASICS_DEBUG
                    my_printf("[Warning]: the trusted area tried to call a trusted func\n");
                    my_printf("Turned to 0x%lx\n", target);
                #endif     
            }    


            if (r_regs->dasicsReturnPC != r_regs->ra && (entry->_flags & MAIN_AREA))
            {
            #ifdef DASICS_DEBUG
                my_printf("[Warning]: maybe used instruction j or jr to ulib func\n");
            #endif
                r_regs->dasicsReturnPC = r_regs->ra;
            }

            
            // TMP alloc a free zone for the lib func
            if (!(target_got_entry->_flags & MAIN_AREA))
            {
                trap_libcfg_alloc(r_regs, DASICS_LIBCFG_X, target_got_entry->_text_end, target_got_entry->_plt_start);
                // debug_print_umain_map(target_got_entry);
            }
            
            return HIT;    
        } else
        {
        #ifdef DASICS_DEBUG
            my_printf("[error]: the plt read badaddr not equal pointed got\n");
        #endif            
            return HIT_ERROR;
        }
    } else 
        return NO_HIT;

    
}


int deal_jump_to_plt(umain_got_t * entry, regs_context_t * r_regs)
{
    int plt_idx;
    if ((plt_idx = _is_plt_area(r_regs->utval, entry)) != NOEXIST)
    {   
    #ifdef DASICS_DEBUG
        my_printf("hit the plt_idx: %lx\n", plt_idx);
    #endif
        // the plt read addr
        // if (r_regs->utval == \
        //     (uint64_t)entry->got_begin + (plt_idx + 2) * sizeof(void *))
        // {
            uint64_t target = 0;
            if (entry->got_begin[plt_idx + 2] == entry->plt_begin)
            {
                /*
                * We found that the Plt[x] wants to use dely binding to find the fucntion,
                * and we prepare all the parameters, and jump
                * 
                * dll_a0: the got[1], struct link_map of the library
                * all_a1: the thrice of the plt table offset
                * ulib_func: the addr of the ulib function 
                */
                uint64_t dll_a0 = entry->got_begin[1];
                uint64_t dll_a1 = (((reg_t)plt_idx * 0x10UL) >> 1) * 3;
                uint64_t ulib_func = dll_fixup_handler(dll_a0, dll_a1);
                
                /* now, we get the lib func, we will switch to it */
                target = r_regs->uepc = ulib_func;

            } else
            {
                /**
                 * Now, the got has been filled with the lib function address in the memory
                 * we will check it.
                 */
                target = r_regs->t3 = r_regs->uepc = entry->got_begin[plt_idx + 2];
            }    
            // if (r_regs->dasicsReturnPC != r_regs->ra && (entry->_flags & MAIN_AREA))
            // {
            // #ifdef DASICS_DEBUG
            //     my_printf("[Warning]: maybe used instruction j or jr to ulib func\n");
            // #endif
            //     r_regs->dasicsReturnPC = r_regs->ra;
            // }

            umain_got_t * target_got_entry = _get_trap_area(target);
            // TMP alloc a free zone for the lib func
            if (!(target_got_entry->_flags & MAIN_AREA))
            {
                trap_libcfg_alloc(r_regs, DASICS_LIBCFG_X, target_got_entry->_text_end, target_got_entry->_plt_end);
                // debug_print_umain_map(target_got_entry);
            }
            
            return HIT;    
        // } else
        // {
        // #ifdef DASICS_DEBUG
        //     my_printf("[error]: the plt read badaddr not equal pointed got\n");
        // #endif            
        //     return HIT_ERROR;
        // }
    } else 
        return NO_HIT;
}