simple external cs2 base you can expand on. Has esp and read only aimbot. Hijacks discord overlay to draw without creating a sus window on top of the game.
All drawing is done with integer arithmetic. Aimbot is implemented with a syscall wrapper around NtUserSendInput to avoid potential future
userland hooks


currently config is hard coded:

config.h:

  aimbot mouse_5
  
  triggerbot mouse_4
  
  rcsScale is 1.0f(perfect)
  
  resolution 1280x720
  
main.cpp(resolution)

  w 1280
  
  h 270
  
yet to be done:

-wrapper around ntgetasynckeystate

-menu


![alt text](https://github.com/d0stoievskiii/cs2-external-base/blob/master/Untitled.png?raw=true)
