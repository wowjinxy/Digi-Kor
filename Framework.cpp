//Kernel32
// 
//GetModuleHandleA
//GetStartupInfoA

//MFC42
//
//fwWinMain
//fwGetModuleThreadState
//fwGetModuleState

//MSVCRT.DLL
//
//__p__commode
//_exit
//_XcptFilter
//_acmdln
//__getmainargs
//_initterm
//_except_handler3
//_controlfp
//__set_app_type
//__p__fmode
//_adjust_fdiv

//DM.EXE
//
//STUB_004a2195
//DAT_004f4c7c
//DAT_004F4C78
//VTBL_004B0000
//DAT_004b01e4
//__argc_flag
//_newmode
//_controlfp(0x10000,0x30000)
//GUID_ffffffff-20c8-004a-dc20-4a0000000000
//WinMain
//DAT_004f4c3c
//DAT_004f4c38
//__start

//Call Order
// 
//GetModuleHandleA
//STUB_004a2195
//__p__commode
//DAT_004f4c7c
//DAT_004F4C78
//_exit
//_XcptFilter
//_exit
//GetStartupInfoA
//_acmdln
//VTBL_004B0000
//DAT_004b01e4
//__getmainargs
//__argc_flag
//_newmode
//_initterm
//_except_handler3
//_controlfp(0x10000, 0x30000)
//GUID_ffffffff-20c8-004a-dc20-4a0000000000
//WinMain
//__set_app_type
//__p__fmode
//DAT_004f4c3c
//DAT_004f4c38
//_adjust_fdiv
//__start