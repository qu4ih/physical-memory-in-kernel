std::uintptr_t get_base_address(HANDLE pid)
{
  // validate that the pid isnt null to avoid some issues
	if (!pid)
		return 0;

  // this makes sure this is running at a safe time since we're touching memory related things
	if (KeGetCurrentIrql() != PASSIVE_LEVEL)
		return 0;

  // refrenced this in cr3 but we find a process by its process id and then assign it to the process object
	PEPROCESS proc = nullptr;
	if (!NT_SUCCESS(PsLookupProcessByProcessId(pid, &proc)))
		return 0;

  // just for safety again
	if (!proc)
		return 0;

  // assign the base address of the processes main exe image (this is where the exe is in memory)
	std::uintptr_t image = reinterpret_cast<std::uintptr_t>(PsGetProcessSectionBaseAddress(proc));

  // just make sure we actually retrived it
  if (!image)
  {
       ObDereferenceObject(proc);
      return 0;
  }

  // free the process object
	ObDereferenceObject(proc);

  // return the base address
	return image;
}
