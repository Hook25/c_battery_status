# c_battery_status

A tiny battery status blocklet for i3bar written in C. Prints the percentage of each battery present in the system and its status. This blocklet depends on `acpi` for the battery status and `libnotify` when compiled with the notification feature.

This is how the blocklet looks with two batteries under different conditions:

![Blocklet Preview](./preview.png)

## Status

The blocklet reports the status of each battery with its percentage. The status codes are:
- `C`: Charging
- `D`: Discharging
- `N`: Not charging
- `F`: Full
- `U`: Unknown

Refer to the `acpi` manual for the meaning of each.

## Building

To build the project do the following
``` 
> git clone git@github.com:Hook25/c_battery_status.git
> cd c_battery_status
> make build
```
Alternatively to exclude the notification feature from the blocklet
``` 
[...]
> make nonotify
```
To debug the blocklet if it does not report the full status of the battery try building a debug version and call it via console.
```
[...]
> make debug
```
