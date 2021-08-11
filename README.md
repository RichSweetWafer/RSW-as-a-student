# RSW as a student
Okay so here is the stuff I've done as a university student and that I'm more or less proud of. Or this is just a garbage can, lol.



## 1. [kmalloc](https://elixir.bootlin.com/linux/v5.13.7/source/include/linux/slab.h#L542)
```
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
```
## 2.1. [__kmalloc\[SLUB\]](https://elixir.bootlin.com/linux/v5.13.7/source/mm/slub.c#L4037)
```
void *__kmalloc(size_t size, gfp_t flags)
{
	struct kmem_cache *s;
	void *ret;

	if (unlikely(size > KMALLOC_MAX_CACHE_SIZE))
		return kmalloc_large(size, flags);

	s = kmalloc_slab(size, flags);

	if (unlikely(ZERO_OR_NULL_PTR(s)))
		return s;

	ret = slab_alloc(s, flags, _RET_IP_, size);

	trace_kmalloc(_RET_IP_, ret, size, s->size, flags);

	ret = kasan_kmalloc(s, ret, size, flags);

	return ret;
}
```
