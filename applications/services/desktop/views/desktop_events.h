#pragma once

typedef enum {
    // Events with _ are unused, kept for compatibility
    DesktopMainEventLockWithPin,
    DesktopMainEventOpenLockMenu,
    DesktopMainEventOpenArchive,
    _DesktopMainEventOpenFavoriteLeftShort,
    _DesktopMainEventOpenFavoriteLeftLong,
    _DesktopMainEventOpenFavoriteRightShort,
    _DesktopMainEventOpenFavoriteRightLong,
    DesktopMainEventOpenMenu,
    DesktopMainEventOpenDebug,
    DesktopMainEventOpenPowerOff,

    _DesktopDummyEventOpenLeft,
    _DesktopDummyEventOpenDown,
    _DesktopDummyEventOpenOk,

    DesktopLockedEventUnlocked,
    DesktopLockedEventUpdate,
    DesktopLockedEventShowPinInput,
    DesktopLockedEventCoversClosed,

    DesktopPinInputEventResetWrongPinLabel,
    DesktopPinInputEventUnlocked,
    DesktopPinInputEventUnlockFailed,
    DesktopPinInputEventBack,

    DesktopPinTimeoutExit,

    DesktopDebugEventDeed,
    DesktopDebugEventWrongDeed,
    DesktopDebugEventSaveState,
    DesktopDebugEventExit,

    DesktopLockMenuEventLockPin,
    _DesktopLockMenuEventDummyModeOn,
    _DesktopLockMenuEventDummyModeOff,
    DesktopLockMenuEventStealthModeOn,
    DesktopLockMenuEventStealthModeOff,

    DesktopAnimationEventCheckAnimation,
    DesktopAnimationEventNewIdleAnimation,
    DesktopAnimationEventInteractAnimation,

    DesktopSlideshowCompleted,
    DesktopSlideshowPoweroff,

    // Global events
    DesktopGlobalBeforeAppStarted,
    DesktopGlobalAfterAppFinished,
    DesktopGlobalAutoLock,

    DesktopMainEventLockKeypad,
    DesktopLockedEventOpenPowerOff,
    DesktopLockMenuEventSettings,
    DesktopLockMenuEventLock,
    DesktopLockMenuEventLockPinOff,
    DesktopLockMenuEventXtreme,
} DesktopEvent;
