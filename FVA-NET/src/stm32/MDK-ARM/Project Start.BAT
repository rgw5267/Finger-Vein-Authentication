SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\ProgramData\Oracle\Java\javapath;C:\Program Files\NVIDIA Corporation\PhysX\Common;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\Microsoft\Web Platform Installer\;C:\Program Files\Microsoft ASP.NET\ASP.NET Web Pages\v1.0\;C:\Program Files\Windows Kits\8.0\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;D:\Green Program\nssm\win32
SET CPU_TYPE=STM32F103VC
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Project Start
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\utils.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\led.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\speaker.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\sys_init.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\usart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\misc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_exti.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_sdio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_tim.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_usart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\core_cm3.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\system_stm32f10x.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\obj\startup_stm32f10x_hd._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\Obj\Project.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\Obj\Project.axf" --i32combined --output ".\Obj\Project.hex"
