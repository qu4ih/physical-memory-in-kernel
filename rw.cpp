// WOW memory

NTSTATUS read(PVOID address, PVOID buffer, size_t size)
{
    // MM_COPY_ADDRESS is a union to specficy a physical or virtual address (ie just tells mmcopymemory what the address is); for you to get the physical address you would actually have to translate the address from the virtual address using cr3 and a bunch of math
    MM_COPY_ADDRESS  maddy = { 0 };
    // here we actually set the address
    maddy.PhysicalAddress.QuadPart = (ULONG64)address;
    SIZE_T bytesCopied = 0;
    // copies memory from the physical address to the buffer (what the call would output)
    // we tell windows its a physical address and to treat it like so by using MM_COPY_MEMORY_PHYSICAL
    return MmCopyMemory(buffer, maddy, size, MM_COPY_MEMORY_PHYSICAL, &bytesCopied);
}
