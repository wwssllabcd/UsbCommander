# UsbCommander
UsbCommander，可以讓你使用自訂的 cdb cmd 到 usb device 的程式，也有一些可以針對儲存裝置做的一些簡單的測試功能，如 sequence random read/write lba 並且做 data compare 的功能

How to compile
* Compile 時，會需要 include EricCore Project[1] 此時基本上可以完成 compile (為了高內聚, 所有我的 CPP Project 都需要 include EricCoreCpp Project )
* UsbCommander 本身只當作一些功能的介面，而底層的 scsi io 是可以切開來的，而預設是一個 fake 的 lba read/write 的功能，在 cmdif.cpp( commmand interface) 可以讓你自由的切換後端的功能，例如你可以 include DriveCtrl project[2] 並且在 cmdif.cpp 中做出切換，這樣 UsbCommander 就可以進行實際上的 scsi cmd 操作


------------

UsbCommander, can let you free to issue any cdb cmd you want, here also have some function for device testing 
for ex: sequence read/write & random read/write lba, and compare data 

How to compile
* at first, you need to include EricCore Project[1] for compile( actually, all of my cpp project need to include that for cohesion issue)
* UsbCommander just a interface for input. backend is separated from usbcommander. defalut of backend is a fake lba read/write, so if you want to issue real scsi cmd, you need to include DriveCtrl project[2] and enable real scsi cmd function(by deviceioctrl) in CmdIf.cpp mmander 

-----------

Reference  
[1]: EricCoreCpp Project https://github.com/wwssllabcd/EricCoreCpp   
[2]: DriveCtrl project https://github.com/wwssllabcd/DeviceCtrl    
