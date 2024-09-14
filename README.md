simple external cs2 base. Has esp and read only aimbot. Hijacks discord overlay to draw without creating a sus window on top of the game.
All drawing is done with integer arithmetic. Aimbot is implemented with a syscall wrapper around NtUserSendInput to avoid potential future
userland hooks

yet to be done:
-full aimbot implementation
-config implementation

