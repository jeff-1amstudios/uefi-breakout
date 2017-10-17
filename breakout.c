#include <efi.h>
#include <efilib.h>

extern EFI_GUID GraphicsOutputProtocol;

#include "efiConsoleControl.h"

EFI_GUID gEfiConsoleControlProtocolGuid = EFI_CONSOLE_CONTROL_PROTOCOL_GUID;


EFI_STATUS
efi_main (EFI_HANDLE *image, EFI_SYSTEM_TABLE *systab)
{
	UINTN index;

	EFI_STATUS rc;
        EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixels[1];
	pixels[0].Blue = 0;
	pixels[0].Red = 1;
	pixels[0].Green = 255;
	pixels[0].Reserved = 0;

        InitializeLib(image, systab);

	

        rc = LibLocateProtocol(&GraphicsOutputProtocol, (void **)&gop);
        if (EFI_ERROR(rc))
                return rc;

	uefi_call_wrapper(systab->ConOut->Reset, 2, systab->ConOut, 0);
	uefi_call_wrapper(systab->ConOut->SetAttribute, 2, systab->ConOut, EFI_TEXT_ATTR(EFI_BLUE, EFI_YELLOW));
	uefi_call_wrapper(systab->ConOut->ClearScreen, 1, systab->ConOut);

	rc = uefi_call_wrapper(gop->Blt, 10, gop, &pixels, EfiBltVideoFill, 0, 0, 0, 0, 10, 10, 0);
	if (EFI_ERROR(rc))
        	return rc;
	Print(L"Framebuffer base is at %lx\n", gop->Mode->FrameBufferBase);

	uefi_call_wrapper(systab->ConOut->OutputString, 2, systab->ConOut, L"Hello application started4\r\n");
	uefi_call_wrapper(systab->ConOut->OutputString, 2, systab->ConOut, L"\r\n\r\n\r\nHit any key to exit\r\n");
	uefi_call_wrapper(systab->BootServices->WaitForEvent, 3, 1, &systab->ConIn->WaitForKey, &index);
	return EFI_SUCCESS;
