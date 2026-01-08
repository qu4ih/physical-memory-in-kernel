// this essentially just holds the processes cr3. I use it so I don't continuesly get the cr3 each IO call

typedef struct cr3_cache
{
	ULONG64 cr3 = 0;
};

// retrive the cr3
std::uintptr_t get_cr3(HANDLE pid, cr3_cache* cache)
{
  // make sure the process id is valid to avoid invalidation
	if (!pid)
		return 0;

  // validates cache pointer before we write to it
	if (!cache)
		return 0;

  // PEPROCESS is an object that represents a process it holds memory context, threads, etc
  PEPROCESS proc = nullptr;
	if (!NT_SUCCESS(PsLookupProcessByProcessId(pid, &proc)))
		return 0;

  // just a safe check
	if (!proc)
		return 0;

  // KAPC_STATE is used to temporarily attach to the current thread to a process to access its memory space
	KAPC_STATE apc;
  // once we attach it switches the memory context to that processes context so then we can read its cr3
	KeStackAttachProcess(proc, &apc);
	std::uintptr_t cr3 = __readcr3();
  // after we detach to go back to our current context
	KeUnstackDetachProcess(&apc);
  
  // then we cache the cr3
	cache->cr3 = cr3;

  // kernel doesn't free this by default so we have to do it manually
	ObDereferenceObject(proc);
	return cr3;
}
