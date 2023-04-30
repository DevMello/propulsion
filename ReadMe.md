<h1 align="center">XFW - <code>Xtreme Firmware</code> for the Flipper Zero</h1>
<h3 align="center">Feature-rich, stable, customizable</h3>

<p align="center">
  <img src="https://user-images.githubusercontent.com/55334727/215170306-051eeb8f-8f72-437f-8c2d-0e4be009bdad.png">
</p>

[Intro](https://github.com/ClaraCrazy/Flipper-Xtreme#What-makes-it-special) | [Animations](https://github.com/ClaraCrazy/Flipper-Xtreme#Animations--Asset-Packs) | [Wiki](https://github.com/ClaraCrazy/Flipper-Xtreme/wiki) | [Changelog](https://github.com/ClaraCrazy/Flipper-Xtreme#list-of-changes) | [Known bugs](https://github.com/ClaraCrazy/Flipper-Xtreme/issues?q=is%3Aissue+is%3Aopen+label%3Arelease-pending) | [Install](https://github.com/ClaraCrazy/Flipper-Xtreme#Install) | [Build](https://github.com/ClaraCrazy/Flipper-Xtreme#build-it-yourself) | [Discord](https://discord.gg/flipper-xtreme) | [Donate](https://github.com/ClaraCrazy/Flipper-Xtreme#%EF%B8%8F-support)
-----

This firmware is a complete overhaul of the [Official Firmware](https://github.com/flipperdevices/flipperzero-firmware), and also features lots of awesome code-bits from [Unleashed](https://github.com/DarkFlippers/unleashed-firmware).

-----
<br>
<h2 align="center">What makes it special?</h2>

We have spent many hours perfecting this code even further, and getting the most out of it.

The goal of this Firmware is to regularly bring out amazing updates based on what the community wants, with an actual understanding of whats going on. Fixing bugs that are regularly talked about, removing unstable / broken applications (.FAP) and actually using the level system that just sits abandoned everywhere else.
<br><br>
- <h4>Feature-rich: We include all commonly found apps in the firmware, as long as they work.</h4>

- <h4>Stable: Many hours have been spent rewriting core parts of the Flippers firmware as well as some of its apps to ensure stability. A task that was long needed on all Firmware, so we tackled it right away.</h4>

- <h4>Customizable: Dont like the Animations, want to turn on/off the Home screen icons (battery, SD card etc), change the flippers name or anything like that? You absolutely can. No need to mess with code or deal with weird manifest files. Its all done with an App.</h4>
<br><br>
Note, the below mentioned changes are only a few things we did. For a full list click [here](https://github.com/ClaraCrazy/Flipper-Xtreme/wiki/Customization)

-----
<br>
<h2 align="center">Xtreme Settings:</h2>

We wrote a powerful yet easy-to-use application specifically for our Firmware, that gives you easy-access to all the fancy things we implemented:

<!--

This image needs to be updated!
Also, perhaps a bigger height, with set width (yes distrotion issues ik) so it fits all our bulletpoints without issues

-->

<img src="https://user-images.githubusercontent.com/49810075/228392945-1e68b996-4e2c-46c6-8aae-d0aadd8ea001.gif" align="left" height="150vh"/>
<img align="left" height="160vh" width="20" src="https://upload.wikimedia.org/wikipedia/commons/3/3d/1_120_transparent.png">

- <ins><b>Interface:</b></ins> Customize every bit of your Flipper, from the desktop animations, to the main menu apps, lockscreen style etc.

- <ins><b>Protocols:</b></ins> Here you can toggle between USB & Bluetooth mode for <a href="https://github.com/ClaraCrazy/Flipper-Xtreme/wiki/Generic-Guides#badbt--kb">BadKB</a>, and manage custom Subghz frequencies.

- <ins><b>Misc:</b></ins> All the other options that don't fit elsewhere. Change your Flipper's name, xp level, and manage settings for RGB backlight.

<br>

-----
<br>
<h2 align="center">Animations / Asset Packs:</h2>

We created our own, new & improved Animation / Asset system, that we can finally reveal. It lets you to create and cycle through your own `Asset Packs` with only a few button presses, allowing you to easily load custom Animations and Icons like never before.

<img src="https://user-images.githubusercontent.com/55334727/214010675-9eddb8f5-1dd6-4cf4-a0ee-e37af8b6c933.PNG" align="left" width="200px"/>
You can easily create your own pack, or find some user made ones in the discord channel. Check <a href="https://github.com/ClaraCrazy/Flipper-Xtreme/wiki/Asset-Packs">here</a> for a tutorial on creating your own. Essentially, we got our own <code>Anims</code> & <code>Icons</code> folders, inside each <code>Asset Pack</code>.

<br clear="left"/>

<br>

<img src="https://user-images.githubusercontent.com/55334727/214016338-95a619c7-88d2-4db5-bb7a-75282d9082b8.png" align="left" width="200px"/>
Once you have some packs, upload them to your Flipper in <code>SD/dolphin_custom</code> (if you did this right you should see <code>SD/dolphin_custom/PackName/Anims</code> and/or <code>SD/dolphin_custom/PackName/Icons</code>).


<br clear="left"/>

<br>

<img src="https://user-images.githubusercontent.com/55334727/214013624-25dad48e-72ea-4a90-9060-66e137e0d61a.png" align="left" width="200px"/>
After installing the packs to Flipper, hit the <code>Arrow UP</code> button on the main menu and go to <code>Xtreme Settings</code>. Here choose which pack you want and tweak the other settings how you prefer, then press back to reboot and enjoy your new assets for all apps (e.g. Subghz scanning asset) & animations!

<br clear="left"/>

-----
<br>
<h2 align="center">Bad Keyboard:</h2>

<img src="https://user-images.githubusercontent.com/49810075/223855940-b8ee6770-4520-4bcc-a4cc-089196cf904b.png" align="left" width="250px"/>
<! -- This fuckshit needs a captured image, but bc of blockage, i cant get one. someone do some magic plz -- !>
BadUSB is a wonderful app, but it lacks bluetooth capabilities. Now some might argue that its useless as you will always need authentication from both sides, but what if I told you that we found a solution to this problem?
<br><br>
Bad-KB allows you to toggle between USB and Bluetooth mode for your attacks. In Bluetooth mode it allows you to spoof the name & MAC of the device to whatever you want. Being a JBL speaker or a wireless razer keyboard is easily doable, allowing you to trick people so you can run your payloads without needing a cable at hand.

-----
<br>
<h2 align="center">Levels:</h2>

This Firmware has 30 levels, not just the basic 3 the official one has.

With this new system in place, it allows for some cool stuff like locking animations behind a certain level. This can be done fairly easy: The idle_animations are tied to the level system. Specifically, the `Min level` variable of your manifest file is used here. Each level you reach, unlocks a new animation. The higher your level, the more animations people can see.

<details>
<summary>Our example</summary>

In our example case, this is used with the NSFW animation pack you can select in the Xtreme app. Dont worry, this is not used by default because I know not everyone likes to see my / anime tits and thats fine. Anyways.. each level gives a brand new background animation, they also become more and more lewd over time.

| Level  | Animations |
| ------------- | ------------- |
| 1-10  | Try harder. Just sexy clothes |
| 11-20 | Some tits, maybe an ass |
| 21-30 | Fully NSFW, graphic scenes |

</details>

-----
<br>
<h2 align="center">List of changes:</h2>

Note: This repo is always updated with OFW & Unleashed. No need to mention all those here. We will only mention **our** changes that we can actually be credited for.

```txt
[Added]

- Xtreme App
- Asset Packs
- More UI options
- Bad-Keyboard App
- A new battery display-type
- Scrolling view for long file names in browser
- NSFW Animations tied to the level system. Read more above
- Folder handling for empty ones (Now indicate they are empty)

- Custom subghz presets
- Multiple NFC protocols
- Multiple Sub-Ghz protocols | Merged from Unleashed, thanks @xMasterX
- Subghz and IR signal replication via gpio | Credits to @xMasterX

- New API Routes for Locale settings
```
```txt
[Updated]

- All Assets

- Tons of apps
- File browser
- Massive compiler re-do
- About 4k files to speed things up a lot
- Applications to now use the new Locale setting
```
```txt
[Fixed]

- Keyboard issues on first char
- Passport crash on high level
- SFW / Dummy_mode getting you XP
- Leveling system
- Mood system
```
```txt
[REMOVED]

- Unused Dummy Mode
- Broken apps (bad apple, chess, etc.)
- Tons of unused code from FAPs and system calls
```

----
<br>
<h2 align="center">Install:</h2>
<br>

There are 3 methods to install Xtreme, we recommend you use the **Web Updater**, but choose whichever one you prefer:

<br>

> <details><summary><code>Web Updater (Chrome)</code></summary><ul>
>   <li>Open the <a href="https://github.com/ClaraCrazy/Flipper-Xtreme/releases/latest">latest release page</a> and click on the <code>Web Updater</code> link</li>
>   <li>Make sure qFlipper is closed</li>
>   <li>Click <code>Connect</code> and select your Flipper from the list</li>
>   <li>Click <code>Install</code> and wait for the update to complete</li>
> </ul></details>

> <details><summary><code>qFlipper Package (.tgz)</code></summary><ul>
>   <li>Download the qFlipper package (.tgz) from the <a href="https://github.com/ClaraCrazy/Flipper-Xtreme/releases/latest">latest release page</a></li>
>   <li>Open <a href="https://flipperzero.one/update">qFlipper</a> and connect your Flipper</li>
>   <li>Click <code>Install from file</code></li>
>   <li>Select the .tgz you downloaded and wait for the update to complete</li>
> </ul></details>

> <details><summary><code>Zipped Archive (.zip)</code></summary><ul>
>   <li>Download the zipped archive (.zip) from the <a href="https://github.com/ClaraCrazy/Flipper-Xtreme/releases/latest">latest release page</a></li>
>   <li>Extract the archive. This is now your new Firmware folder</li>
>   <li>Open <a href="https://flipperzero.one/update">qFlipper</a>, head to <code>SD/Update</code> and simply move the firmware folder there</li>
>   <li>On the Flipper, hit the <code>Arrow Down</code> button, this will get you to the file menu. In there simply search for your updates folder</li>
>   <li>Inside that folder, select the Firmware you just moved onto it, and run the file thats simply called <code>Update</code></li>
> </ul></details>

<br>

**If you have issues or crashes with the install process, you can try to use `Settings > Storage > Factory Reset` then retry the install.**
**Doing that will NOT remove your saved files, it will only forget some settings and paired devices.**

----
<br>
<h2 align="center">Build it yourself:</h2>

```bash
To download the needed tools:
$ git clone --recursive https://github.com/ClaraCrazy/Flipper-Xtreme.git
$ cd Flipper-Xtreme/

To flash directly to the Flipper (Needs to be connected via USB, qFlipper closed)
$ ./fbt flash_usb

To just compile firmware
$ ./fbt updater_package

If building FAPS:
$ ./fbt fap_dist

If building image assets:
$ ./fbt resources icons dolphin_ext
```

----
<h2 align="center">Stargazers over time</h2>

[![Stargazers over time](https://starchart.cc/ClaraCrazy/Flipper-Xtreme.svg)](https://starchart.cc/ClaraCrazy/Flipper-Xtreme)

----
<h2 align="center">Contributors</h2>

[![Contributors](https://user-images.githubusercontent.com/49810075/228231815-8f0a267d-ac1a-494c-9cd0-1cd57568fc79.svg)](https://github.com/ClaraCrazy/Flipper-Xtreme/graphs/contributors)


## ❤️ Support
If you like what you're seeing, **please consider donating to us**. We won't ever put this behind a paywall, but we'd still appreciate a few bucks!

- **[Direct Wire-transfer](https://bunq.me/ClaraK)**: No account needed, just specify amount and hit send
- **[Patreon](https://patreon.com/CynthiaLabs)**
- **[Paypal](https://paypal.me/RdX2020)**
- **Monero**: `41kyWeeoVdK4quzQ4M9ikVGs6tCQCLfdx8jLExTNsAu2SF1QAyDqRdjfGM6EL8L9NpXwt89HJeAoGf1aoArk7nDr4AMMV4T`

**Thanks for all your support <3**

----
<p align="center"> "What we do for ourselves dies with us. What we do for others and the world remains and is immortal.” ― Albert Pine </p>
