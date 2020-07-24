# UsbCommander
UsbCommander, 可以讓你使用自訂的 cdb cmd 到 usb device 的程式
it need to incldue EricCore Project https://github.com/wwssllabcd/EricCoreCpp when you compile

cmdif.cpp provide an interface to switch to fake scsi cmd(fake_lba_read /write) / write and scsi cmd

if you want to endable real scsi cmd, you need to include DriveCtrl project https://github.com/wwssllabcd/DeviceCtrl
