## Smart Command Processor

SmartCMD, also known as iZCMD+, is a fast feature rich command processor which simplifies the creation and handling of commands. SmartCMD is iZCMD loaded with new features but ironically, it is faster than iZCMD. SmartCMD works on the same principle which iZCMD/ZCMD works on, i.e: create commands as public functions and call the public function when the command is used. However, there are few subtle changes in the core algorithm to make allowances for the features.

SAMP Forum Topic: http://forum.sa-mp.com/showthread.php?p=3786819


###What's new?

* **Command ID System**

    SmartCMD assigns a unique ID number to each command to enhance the overall performance. By using command ids instead of command names, costly string comparisons can be avoided. The system has been designed with extensibility in mind. If you'd ever need per-player command permissions or need to store per-command information say help descriptions, it'll take you less than 5 minutes to set things up using the command id system.


* **Delete/Undelete commands**

    Delete and undelete commands in runtime.


* **Command Flags**

    A command flag is just a variable associated with each command. You can store crucial information about each command in its flag. Flags are optional. Unlike most command processors which support flags, flags for commands can be assigned during compile time. SmartCMD does not require SetCommandFlags calls during initialization.


* **Command Modes/States**
    
    You can have more than one command function associated with each command.


* **Alternate Command Names**

   Unlike iZCMD/ZCMD, creating alternate command names does not take any extra CPU. Using the alternate command is as fast as using the parent command. However, each alternate command gets a unique command id using which its properties can be modified (disabled/reassigned/etc).


* **Reassign Command Functions**
    
    Change a command's command function in run-time.


* **Scripter-friendly include**
    
    SmartCMD allows you to set command properties such as flags, alternate names during compile time while declaring/defining the command itself so that you don't have to fill your OnGameModeInit/OnFilterScriptInit with trash. You can also access command ids and command flags by a simple variable fetch; no need to bloat your code with GetCommandID or Get/SetCommandFlags function calls.


### Who made it?
- Yashas
- Y_Less for defines tricks (Issue No 2)
- Zeex for ZCMD
- Crayder for constructive talks
