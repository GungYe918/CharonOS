#@range_begin(defines)
[Defines]
  PLATFORM_NAME                     = CharonLoaderPkg
  PLATFORM_GUID                     = db94a504-c5cd-4647-8662-9adb3be41455
  PLATFORM_VERSION                  = 0.1
  DSC_SPECIFICATION                 = 0x00010005
  OUTPUT_DIRECTORY                  = Build/CharonLoader$(ARCH)
  SUPPORTED_ARCHITECTURES           = X64
  BUILD_TARGETS                     = DEBUG|RELEASE|NOOPT
#@range_end(defines)
#@range_end(defines)

#@range_begin(library_classes)
[LibraryClasses]
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
#@range_end(library_classes)

  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf

#@range_begin(components)
[Components]
  CharonLoaderPkg/Loader.inf
#@range_end(components)