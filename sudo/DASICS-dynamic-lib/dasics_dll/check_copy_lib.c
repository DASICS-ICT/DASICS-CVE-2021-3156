#include <dasics_link.h>
#include <dasics_link_manager.h>
#include <utrap.h>
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>


/* 
 * Check lib's init by DT_INIT and DT_INIT_ARRAY, if they are
 * zero which means that the reloc of the dynamic link is failed !
 * the init of the dynamic has failed!
 */
static int _check_lib(umain_got_t * untrust_got)
{
    if (untrust_got->l_info[DT_INIT] != NULL)
    {
        /* copy addr from trusted lib */
        
        if (*(uint64_t *)(untrust_got->l_addr + untrust_got->l_info[DT_INIT]->d_un.d_ptr) == NULL)
        {
        #ifdef DASICS_DEBUG
            my_printf("[ERROR]: the untrusted DT_INIT is zero!\n");
        #endif
            exit(1);
        }
    }

    
    ElfW(Dyn) *untrusted_init_array = untrust_got->l_info[DT_INIT_ARRAY];

    if (untrusted_init_array != NULL)
    {
        unsigned int j;
        unsigned int jm;
        ElfW(Addr) *addrs;

        jm = untrust_got->l_info[DT_INIT_ARRAYSZ]->d_un.d_val / sizeof (ElfW(Addr));


        addrs = (ElfW(Addr) *) (untrusted_init_array->d_un.d_ptr + untrust_got->l_addr);
        for (j = 0; j < jm; ++j)
	        if (addrs[j] == NULL)
            {
            #ifdef DASICS_DEBUG
                my_printf("[ERROR]: the untrusted DT_INIT_ARRAY is zero!\n");
            #endif
                exit(1);                
            } else 
            {
            #ifdef DASICS_DEBUG
                my_printf("[DEBUG]: the init func: 0x%lx\n", addrs[j]);
            #endif
            }
    }
}

/*
 * if the library in trusted area has one copy in untrusted area
 * init it
 */
int check_copy_library()
{
    umain_got_t * _local_umain = _umain_got_table;
    /* a head used to fill the new list, we will list it to _umain_got_table finally */
    while (_local_umain)
    {
        /* if this is a trusted lib */
        if ((_local_umain->_flags & (LIB_AREA | MAIN_AREA)) == (LIB_AREA | MAIN_AREA))
        {
        #ifdef DASICS_DEBUG
            my_printf("[DEBUG]: found trust lib: %s, path: %s\n", _local_umain->real_name, _local_umain->l_name);
        #endif
            umain_got_t * _untrusted_umain = _find_got_untrust(_local_umain, _local_umain->real_name);

            if (_untrusted_umain)
            {
            #ifdef DASICS_DEBUG
                my_printf("[DEBUG]: Found one unstrusted lib, check it!\n");
            #endif  
                _check_lib(_untrusted_umain);
                _local_umain->_point_got = _untrusted_umain;
                _untrusted_umain->_point_got = _local_umain;
            }
        }
        _local_umain = _local_umain->umain_got_next;
    }
    
    _local_umain = _umain_got_table;

    return 0; 
}


