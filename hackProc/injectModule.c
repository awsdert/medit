#include "hackProc.h"
/**
 * Inject a PE module in the target process memory
 * @param proc Handle to target process
 * @param module PE we want to inject
 * @return Handle to injected module in target process
 */
HMODULE injectModule(HANDLE proc, LPVOID module)
{
    DWORD i = 0;
    DWORD_PTR delta = 0;
    DWORD_PTR olddelta = 0;
    /* Get module PE headers */
    PIMAGE_NT_HEADERS headers = (PIMAGE_NT_HEADERS)((LPBYTE)module + ((PIMAGE_DOS_HEADER)module)->e_lfanew);
    PIMAGE_DATA_DIRECTORY datadir;

    /* Get the size of the code we want to inject */
    DWORD moduleSize = headers->OptionalHeader.SizeOfImage;
    LPVOID distantModuleMemorySpace = NULL;
    LPBYTE tmpBuffer = NULL;
    BOOL ok = FALSE;
    if (headers->Signature != IMAGE_NT_SIGNATURE)
        return NULL;

    /* Check if calculated size really corresponds to module size */
    if (IsBadReadPtr(module, moduleSize))
        return NULL;

        /* Allocate memory in the target process to contain the injected module image */
    distantModuleMemorySpace = VirtualAllocEx(proc, NULL, moduleSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (distantModuleMemorySpace != NULL)
    {
        /* Now we need to modify the current module before we inject it */
        /* Allocate some space to process the current PE image in an temporary buffer */
        tmpBuffer = (LPBYTE)VirtualAlloc(NULL, moduleSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (tmpBuffer != NULL)
        {
            RtlCopyMemory(tmpBuffer, module, moduleSize);
                        /* Get data of .reloc section */
            datadir = &headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
            if (datadir->Size > 0 && datadir->VirtualAddress > 0)
            {
                /* delta is offset of allocated memory in target process */
                delta = (DWORD_PTR)((LPBYTE)distantModuleMemorySpace - headers->OptionalHeader.ImageBase);

                /* olddelta is offset of image in current process */
                olddelta = (DWORD_PTR)((LPBYTE)module - headers->OptionalHeader.ImageBase);

                /* Point to first relocation block copied in temporary buffer */
                PIMAGE_BASE_RELOCATION reloc = (PIMAGE_BASE_RELOCATION)(tmpBuffer + datadir->VirtualAddress);

                /* Browse all relocation blocks */
                while(reloc->VirtualAddress != 0)
                {
                        /* We check if the current block contains relocation descriptors, if not we skip to the next block */
                        if (reloc->SizeOfBlock >= sizeof(IMAGE_BASE_RELOCATION))
                        {
                                /* We count the number of relocation descriptors */
                                DWORD relocDescNb = (reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
                                /* relocDescList is a pointer to first relocation descriptor */
                                LPWORD relocDescList = (LPWORD)((LPBYTE)reloc + sizeof(IMAGE_BASE_RELOCATION));

                                /* For each descriptor */
                                for (i = 0; i < relocDescNb; i++)
                                {
                                        if (relocDescList[i] > 0)
                                        {
                                                /* Locate data that must be reallocated in buffer (data being an address we use pointer of pointer) */
                                                /* reloc->VirtualAddress + (0x0FFF & (list[i])) -> add botom 12 bit to block virtual address */
                                                DWORD_PTR *p = (DWORD_PTR *)(tmpBuffer + (reloc->VirtualAddress + (0x0FFF & (relocDescList[i]))));
                                                /* Change the offset to adapt to injected module base address */
                                                *p -= olddelta;
                                                *p += delta;
                                        }
                                }
                        }
                        /* Set reloc pointer to the next relocation block */
                        reloc = (PIMAGE_BASE_RELOCATION)((LPBYTE)reloc + reloc->SizeOfBlock);
                }

                /* Remove wild breakpoint at begining of main function */
                tmpBuffer[(DWORD)main - (DWORD)module] = 0x55;// put the normal push ebp instruction

                /* Write processed module image in target process memory */
                ok = WriteProcessMemory(proc, distantModuleMemorySpace, tmpBuffer, moduleSize, NULL);
            }
            VirtualFree(tmpBuffer, 0, MEM_RELEASE);
        }

        if (!ok)
        {
            VirtualFreeEx(proc, distantModuleMemorySpace, 0, MEM_RELEASE);
            distantModuleMemorySpace = NULL;
        }
    }
        /* Return base address of copied image in target process */
    return (HMODULE)distantModuleMemorySpace;
}
