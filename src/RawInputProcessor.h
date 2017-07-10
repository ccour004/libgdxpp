/** Since SDL provides different (and sometimes more) info that an InputProcessor wants, capture it here.
 * 
 * @author ccourtney */
 
#pragma once
class RawInputProcessor {
public:
    //SDL_AudioDeviceEvent(SDL_AUDIODEVICEADDED,SDL_AUDIODEVICEREMOVED)
    virtual bool controllerAxisEvent(const SDL_ControllerAxisEvent& event) = 0;
    virtual bool controllerButtonEvent(const SDL_ControllerButtonEvent& event) = 0;
    virtual bool controllerDeviceEvent(const SDL_ControllerDeviceEvent& event) = 0;
    //SDL_DollarGestureEvent(SDL_DOLLARGESTURE,SDL_DOLLARRECORD)
    //SDL_DropEvent(SDL_DROPFILE,SDL_DROPTEXT,SDL_DROPBEGIN,SDL_DROPCOMPLETE)
    virtual bool touchFingerEvent(const SDL_TouchFingerEvent& event) = 0;
    virtual bool keyboardEvent(const SDL_KeyboardEvent& event) = 0;
    //SDL_JoyAxisEvent(SDL_JOYAXISMOTION)
    //SDL_JoyBallEvent(SDL_JOYBALLMOTION)
    //SDL_JoyHatEvent(SDL_JOYHATMOTION)
    //SDL_JoyButtonEvent(SDL_JOYBUTTONDOWN,SDL_JOYBUTTONUP)
    //SDL_JoyDeviceEvent(SDL_JOYDEVICEADDED,SDL_JOYDEVICEREMOVED)
    virtual bool mouseMotionEvent(const SDL_MouseMotionEvent& event) = 0;
    virtual bool mouseButtonEvent(const SDL_MouseButtonEvent& event) = 0;
    virtual bool mouseWheelEvent(const SDL_MouseWheelEvent& event) = 0;
    virtual bool multiGestureEvent(const SDL_MultiGestureEvent& event) = 0;
    //SDL_TextEditingEvent(SDL_TEXTEDITING)
    //SDL_TextInputEvent(SDL_TEXTINPUT)
    //SDL_UserEvent(SDL_USEREVENT)
};