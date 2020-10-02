## Configure Razer Naga Hex V2 (without running Synapse)

### Features

* Designed for Windows (unlike most other projects)
* Configure mouse without having to use Razer Synapse
* Settings stay changed after program is terminated

### Notes

* **Button remapping is not supported**. The program would have to keep actively monitoring key presses to be able to support that.
* **Nothing is shown when the program runs**. This is so that the program can seamlessly run on startup, ensuring the mouse settings are always correct.

### Options

| Method Signature        | Parameter  | Description                     |
|-------------------------|------------|---------------------------------|
| `SET_SENSITIVITY(WORD)` | 0 - 16000  | -                               |
| `SET_POLLRATE(BYTE)`    | 1 / 2 / 8  | Divides polling rate by value   |
| `SET_BRIGHTNESS(BYTE)`  | 0 - 255    | -                               |
| `SET_COLOR(DWORD)`      | 0 - FFFFFF | Displays single static color    |
| `SET_SPECTRUM(DWORD)`   | (ignored)  | Cycles through color spectrum   |
| `SET_REACTIVE(DWORD)`   | 0 - FFFFFF | Displays color when clicking    |
| `SET_BREATH(DWORD)`     | 0 - FFFFFF | Cycles from dim to bright       |

## Implementation

Sends messages in a proprietary protocol, which has been only partially reverse engineered. Packets are delivered over USB, directly altering the state of the device.

### Packet structure

```C
char _0; //unused
char _1; //unused
char transaction;
char _3; //unused
char _4; //unused
char _5; //unused
char size;
char category;
char type;
char data[80];
char crc;
char _90; //unused
```
