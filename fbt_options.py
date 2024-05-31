from pathlib import Path
import posixpath
import re
import os

# For more details on these options, run 'fbt -h'

FIRMWARE_ORIGIN = "Momentum"

# Default hardware target
TARGET_HW = 7

# Optimization flags
## Optimize for size
COMPACT = 1
## Optimize for debugging experience
DEBUG = 0

# Suffix to add to files when building distribution
# If OS environment has DIST_SUFFIX set, it will be used instead

if not os.environ.get("DIST_SUFFIX"):
    # Check scripts/get_env.py to mirror CI naming
    def git(*args):
        import subprocess

        return (
            subprocess.check_output(["git", *args], stderr=subprocess.DEVNULL)
            .decode()
            .strip()
        )

    try:
        local_branch = git("symbolic-ref", "HEAD", "--short")
        ref = git("config", "--get", f"branch.{local_branch}.merge")
    except Exception:
        ref = "refs/heads/detached"
    branch_name = re.sub("refs/\w+/", "", ref)
    commit_sha = git("rev-parse", "HEAD")[:8]
    DIST_SUFFIX = "mntm-" + branch_name.replace("/", "_") + "-" + commit_sha

# Skip external apps by default
SKIP_EXTERNAL = False

# Appid's to include even when skipping externals
EXTRA_EXT_APPS = []

# Coprocessor firmware
COPRO_OB_DATA = "scripts/ob.data"

# Must match lib/stm32wb_copro version
COPRO_CUBE_VERSION = "1.19.0"

COPRO_CUBE_DIR = "lib/stm32wb_copro"

# Default radio stack
COPRO_STACK_BIN = "stm32wb5x_BLE_Stack_light_fw.bin"
# Firmware also supports "ble_full", but it might not fit into debug builds
COPRO_STACK_TYPE = "ble_light"

# Leave 0 to let scripts automatically calculate it
COPRO_STACK_ADDR = "0x0"

# If you override COPRO_CUBE_DIR on commandline, override this as well
COPRO_STACK_BIN_DIR = posixpath.join(COPRO_CUBE_DIR, "firmware")

# Supported toolchain versions
# Also specify in scripts/ufbt/SConstruct
FBT_TOOLCHAIN_VERSIONS = (" 12.3.", " 13.2.")

OPENOCD_OPTS = [
    "-f",
    "interface/stlink.cfg",
    "-c",
    "transport select hla_swd",
    "-f",
    "${FBT_DEBUG_DIR}/stm32wbx.cfg",
    "-c",
    "stm32wbx.cpu configure -rtos auto",
]

SVD_FILE = "${FBT_DEBUG_DIR}/STM32WB55_CM4.svd"

# Look for blackmagic probe on serial ports and local network
BLACKMAGIC = "auto"

# Application to start on boot
LOADER_AUTOSTART = ""

FIRMWARE_APPS = {
    "default": [
        # Svc
        "basic_services",
        # Apps
        "main_apps",
        "system_apps",
        # Settings
        "settings_apps",
    ],
    "unit_tests": [
        "basic_services",
        "updater_app",
        "radio_device_cc1101_ext",
        "unit_tests",
    ],
}

FIRMWARE_APP_SET = "default"

custom_options_fn = "fbt_options_local.py"

if Path(custom_options_fn).exists():
    exec(compile(Path(custom_options_fn).read_text(), custom_options_fn, "exec"))
