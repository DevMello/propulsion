targets_help = """Configuration variables:
"""

tail_help = """

TASKS:
    (* - not supported yet)

    launch:
        Upload and start application over USB
    vscode_dist:
        Configure application in current directory for development in VSCode.
    create:
        Copy application template to current directory. Set APPID=myapp to create an app with id 'myapp'.

Building:
    faps:
        Build all FAP apps
    fap_{APPID}, launch APPSRC={APPID}:
        Build FAP app with appid={APPID}; upload & start it over USB
    cdb:
        regenerate "compile_commands.json" file (for IDE integration)

Flashing & debugging:
    flash, *jflash:
        Flash firmware to target using SWD probe. See also SWD_TRANSPORT, SWD_TRANSPORT_SERIAL
    flash_usb, flash_usb_full:
        Install firmware using self-update package
    debug, debug_other, blackmagic:
        Start GDB
    devboard_flash:
        Update WiFi dev board. 
        Supports ARGS="..." to pass extra arguments to the update script, e.g. ARGS="-c dev"

Other:
    cli:
        Open a Flipper CLI session over USB
    lint:
        run linter for C code
    format:
        reformat C code

How to create a new application:
    1. Create a new directory for your application and cd into it.
    2. Run `ufbt vscode_dist create APPID=myapp`
    3. In VSCode, open the folder and start editing.
    4. Run `ufbt launch` to build and upload your application.

How to open a shell with toolchain environment and other build tools:
    In your shell, type "eval `ufbt -s env`".

How to update uFBT SDK:
    Run "ufbt update" to fetch latest SDK.
    You can also specify branch, target and/or channel options. See "ufbt update -h" for details.
"""


def generate(env, **kw):
    vars = kw["vars"]
    basic_help = vars.GenerateHelpText(env)
    env.Help(targets_help + basic_help + tail_help)


def exists(env):
    return True
