# RSW as a student
Okay so here is the stuff I've done as a university student and that I'm more or less proud of. Or this is just a garbage can, lol.



## 1. [kmalloc]
(https://elixir.bootlin.com/linux/v5.13.7/source/include/linux/slab.h#L542)

/**
 \* kmalloc - allocate memory
 \* @size: how many bytes of memory are required.
 \* @flags: the type of memory to allocate.
 \*
 \* kmalloc is the normal method of allocating memory
 \* for objects smaller than page size in the kernel.
 ...
 \*/
static __always_inline void *kmalloc(size_t size, gfp_t flags)
{
	if (__builtin_constant_p(size)) {
#ifndef CONFIG_SLOB
		unsigned int index;
#endif
		if (size > KMALLOC_MAX_CACHE_SIZE)
			return kmalloc_large(size, flags);
#ifndef CONFIG_SLOB
		index = kmalloc_index(size);

		if (!index)
			return ZERO_SIZE_PTR;

		return kmem_cache_alloc_trace(
				kmalloc_caches[kmalloc_type(flags)][index],
				flags, size);
#endif
	}
	return __kmalloc(size, flags);
}

## 1.1. [zram_bvec_read](https://elixir.bootlin.com/linux/v4.2/source/drivers/block/zram/zram_drv.c#L595)   
```  
static int zram_bvec_read(struct zram *zram, struct bio_vec *bvec,
			  u32 index, int offset)
{
	...
	page = bvec->bv_page;
	...
	if (is_partial_io(bvec))
		/* Use  a temporary buffer to decompress the page */
		uncmem = kmalloc(PAGE_SIZE, GFP_NOIO);
​
	user_mem = kmap_atomic(page);
	...
	ret = zram_decompress_page(zram, uncmem, index);
​
	if (is_partial_io(bvec))
		memcpy(user_mem + bvec->bv_offset, uncmem + offset,
				bvec->bv_len);
​
	flush_dcache_page(page);
...
}
```

