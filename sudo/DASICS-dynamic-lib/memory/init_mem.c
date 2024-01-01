/* dasics simple memory conrol block */
#include <mem.h>
#include <dasics_link.h>
#include <stdlib.h>
#include <dasics_link_manager.h>

static inline int __dasics_linker_memcpy(char *dest, const char *src, unsigned int len)
{
    for (int i = 0; i < len; i++)
    {
        dest[i] = src[i];
    }
    return len;
}

static uint64_t allocpage(mem_struct_t *mm)
{
    list_head *freePageList = &mm->mem_unused;
    if (!list_empty(freePageList))
    {
        list_node_t* new_page = freePageList->next;
        page_node_t *new = list_entry(new_page, page_node_t, list);
        /* one use */
        list_del(new_page);
        list_add_tail(new_page, &mm->mem_used);

        return new->start;        
    } else 
    {
        my_printf("[ERROR]: No more memory in lib\n");
        exit(1);   
    }
}

static uint64_t freecpage(mem_struct_t *mm, uint64_t addr)
{
    uint64_t idx = GET_MEM_NODE(addr, mm);
    if (idx > MAX_NUM)
    {
        my_printf("[ERROR] not valid addr: 0%lx\n", addr);
        exit(1);
    }
    /* alloc page */
    page_node_t *recycle_page = &mm->page_manager[GET_MEM_NODE(addr, mm)];

    list_head *freePageList = &mm->mem_unused;    
    list_del(&recycle_page->list);
    list_add_tail(&recycle_page->list, freePageList);
    
}



static void _init_mem_struct(mem_struct_t * mm)
{
    // init list
    init_list(&mm->mem_used);
    init_list(&mm->mem_unused);

    mm->page_manager = (page_node_t *)malloc(sizeof(page_node_t) * MAX_NUM);

    mm->mem_begin = malloc(MAX_MEM);

    mm->mem_end = mm->mem_begin + MAX_MEM;
    mm->high = mm->mem_end;
    mm->malloc = (uint64_t)dasics_malloc;
    mm->free = (uint64_t)dasics_free;
    mm->realloc = (uint64_t)dasics_realloc;

    if (mm->page_manager == NULL ||
        mm->mem_begin == NULL)
    {
        my_printf("[ERROR]: malloc mem_struct_t's mem error\n");
        exit(1);
    }

    uint64_t ptr = mm->mem_begin;
    for (int _i = 0; _i < MAX_NUM; _i++)
    {
        mm->page_manager[_i].start = ptr;
        ptr += PAGE_SIZE;
        list_add_tail(&mm->page_manager[_i].list, &mm->mem_unused);
    }
#ifdef DASICS_DEBUG
    debug_print_mm(mm);
#endif

}

/* dasics simple malloc */ 
uint64_t dasics_malloc(mem_struct_t * mm, uint64_t size)
{
    uint64_t ptr = NULL;
    my_printf("[dasics_malloc]: malloc: 0x%lx\n", size);
    if (size > PAGE_SIZE)
    {
        my_printf("[ERROR]: Too huge size alloc: 0x%lx\n", size);
        exit(1);
    } else 
    {
        ptr = allocpage(mm);
    }
    my_printf("[dasics_malloc]: malloc addr: 0x%lx\n", ptr);

    return ptr;
}

/* dasics simple free */ 
uint64_t dasics_free(mem_struct_t * mm, uint64_t addr)
{
    my_printf("[dasics_free]: free 0x%lx\n", addr);

    // According to glibc standard, if the free addr == 0, nothing to do   
    if (addr == 0) return;

    if (addr >= mm->mem_begin + MAX_MEM)
    {
        my_printf("[ERROR]: An illegal address: 0x%lx!\n", addr);
        // try glibc free
        free(addr);
    }
    else
    {
        freecpage(mm, addr);
    }
}

/* dasics simple realloc */ 
uint64_t dasics_realloc(mem_struct_t * mm, uint64_t addr,  uint64_t size)
{
    my_printf("[dasics_realloc]: pre addr: 0x%lx, new size: 0x%lx\n", addr, size);

    uint64_t new_addr = dasics_malloc(mm, size);


    memcpy((char *)new_addr, (char *)addr, PAGE_SIZE);


    dasics_free(mm, addr);

    my_printf("[dasics_realloc]: new addr: 0x%lx\n", new_addr);

    return new_addr;
}

/*
 * Init every lib area's mem control struct
 */
void init_mm_struct()
{
    umain_got_t * _local_got = _umain_got_table;

    while (_local_got)
    {
        if (!(_local_got->_flags & MAIN_AREA))
        {
            _local_got->mem = (mem_struct_t *)malloc(sizeof(mem_struct_t));
            if (_local_got->mem == NULL)
            {
                my_printf("[ERROR]: malloc mem_struct_t error\n");
                exit(1);
            }
            _init_mem_struct(_local_got->mem);
        }

        _local_got = _local_got->umain_got_next;
    }
}