# USBWormScanner
A small solution to a college computer lab problem.

## History and Synopsis
This application, originally named "AntiWorm", was created sometime in 2013
while I was in college. I had heard there was an epidemic in the computer lab
of a worm spreading from computer to computer through student's USB drives.
The malware was a VB script file which replaced files on the root of the USB
drive inserted with links which executed the VB script worm, infecting the
computer the USB device was inserted in to if it was not already infected.
The worm also tried to open the original file to hide itself, but this rarely
worked. This application was written to help quickly eliminate the worm from
inserted USB flash drives while the college waited for a definition update from
their antivirus software vendor.

The application first kills all running `wscript` instances on the system when
it first executes. This stops the worm execution if it is already running on
the machine. The application then scans newly inserted flash drives for link
files which have a corresponding file on the flash drive which is hidden. This
is an indicator of infection. The application will remove the link files and
unhide the original documents. The worm only affects files in the root of the
drive so there is no need to traverse the file system on the drive and no need
to check the status of folders. The applciation then deletes all `.vbs` files
on the root of the flash drive to ensure the removal of the worm.

