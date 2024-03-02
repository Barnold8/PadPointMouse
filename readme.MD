# PadPointMouse

This program aims to achieve perfect comfort with ones PC. Imagine it, you're sat there laying in bed and then youtube says "Are you still watching?". And then you have to get up from that illustrious comfort you had. Or you finished a netflix movie and you dont want to get up to put another one on. Okay, I admit, this is just facilitating laziness but who doesn't want that.

PadPointMouse takes your Xbox controller and turns it into your go to place for all your basic PC needs to keep you tucked in bed or sat on the couch to chill out and enjoy your day. 

## PadPointMouse can

- Left click
- Right click
- Bring up the windows virtual keyboard,so you can type without having to get up
- Open up explorer.exe, easily get into your files with no worries!
- Open up the start menu, perfect for finding the program that will ehance your 
- lazy day
- Go backward in the browser and forward in the browser, so you dont need to fiddle about with UI 
- Screenshot, we have to let press both sticks in and get you a cool screenshot, cmon now!
- Change volume, you dont need to get up for nothing!


# How to install

Ensure you have:
- Windows
- An xbox controller (other may work but Xinput was used so xbox controllers were in mind here)

All you need to do is download the binary from the releases page or copy the repo (or download source code from the release) and open up the solution and build for release mode! Release mode will be found in the usual place that visual studio puts that stuff when it is compiled. 

# We now support settings!

The settings file looks like the following, provided with helpful comments to help clear up their purpose

```ini
[settings]

mouse_acceleration = 1  ; how quickly the mouse moves by
dead_zone_x = 90		; left stick dead zone for x axis  - Control stick must move further than dead zone to work
dead_zone_y = 90		; left stick dead zone for y axis  - Control stick must move further than dead zone to work
dead_zone_z = 90		; right stick dead zone for y axis - Control stick must move further than dead zone to work
volume_change_by = 10	; how much the volume goes up/down by when using volume controls
screen_width = 1920     ; config to stop mouse moving outside of screen. Change to 0 to allow system to get screen metrics
screen_height = 1080	; config to stop mouse moving outside of screen. Change to 0 to allow system to get screen metrics
```

# Have an issue with the software?

Feel free to make an issue for this repository, I will look at it at my earliest convenience since I am busy with University work right now (02/03/2024). Im putting the date there because this may be viewed at another date and become outdated.

# Have a suggestion to add to the software?

Feel free to make an issue with some text or tag with the word "Suggestion" within it and I will consider implementing it.


# My current ideas to add to the software

## Extra modes

As of right now, the **Y** button only changes between controller and pc mode. This means that when in controller mode, the controller takes full control of the cursor and related functions, bar clicking, this can still happen with the mouse will in controller mode. And pc mode, this is the same as not having the software running at all. 

So what is my idea? Well to incorporate multiple modes. Since the controller only has a select few buttons to set up functions with, it could be a good idea to iterate over different modes with the **Y** button. You could have 

- Mouse mode
    - Only control mouse button and scroll wheel, with virtual keyboard capabilities
- Media mode
    - Buttons have functionality like, skip media, pause media, volume up, volume down
- Other modes with specific functionality    

## Button binds

In the event that I don't decide on the [Extra modes](#extra-modes) functionality, it could be a cool idea to allow for what is essentially keybinds for the many buttons on the controller. Allowing for specific functionality on whatever buttons the user chooses, this allows for modularity later on in the code base since we can just add new functions later on for the user to choose from. For example, the user could have the left trigger open the file explorer and the right trigger make the volume go up higher.  

I imagine the settings.ini to look like 

```ini

[settings]

mouse_acceleration = 1  ; how quickly the mouse moves by
dead_zone_x = 90		; left stick dead zone for x axis  - Control stick must move further than dead zone to work
dead_zone_y = 90		; left stick dead zone for y axis  - Control stick must move further than dead zone to work
dead_zone_z = 90		; right stick dead zone for y axis - Control stick must move further than dead zone to work
volume_change_by = 10	; how much the volume goes up/down by when using volume controls
screen_width = 1920     ; config to stop mouse moving outside of screen. Change to 0 to allow system to get screen metrics
screen_height = 1080	; config to stop mouse moving outside of screen. Change to 0 to allow system to get screen metrics

custom_webpage = "https://www.google.com"

; ~~ Button function IDs ~~ 
    ; 0 = File explorer
    ; 1 = Virtual keyboard
    ; 2 = Launch custom webpage 
;
x_button = 0
y_button = 1
```

Again, these are just some ideas of what could be done and if you have any cool ideas please let me know!



