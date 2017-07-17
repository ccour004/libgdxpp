/** Multiplexing capability for raw inputs.
 * 
 * @author ccourtney */
 
#include <vector>
 
#pragma once
class RawInputMultiplexer: public InputProcessor{
    std::vector<InputProcessor> processors = std::vector<InputProcessor>(4);
public:
	InputMultiplexer () {}

	InputMultiplexer (const std::vector<InputProcessor>& processors) {
		for (int i = 0; i < processors.size(); i++)
			this->processors.push_back(processors[i]);
	}
    
    std::vector<InputProcessor> getProcessors(){return processors;}

    bool controllerAxisEvent(const SDL_ControllerAxisEvent& event){
        for(auto& InputProcessor processor:processors)
            if(processor->controllerAxisEvent(event)) return true;
        return false;
    }
    
    bool controllerButtonEvent(const SDL_ControllerButtonEvent& event){
        for(auto& InputProcessor processor:processors)
            if(processor->controllerButtonEvent(event)) return true;
        return false;
    }
    
    bool controllerDeviceEvent(const SDL_ControllerDeviceEvent& event){
        for(auto& InputProcessor processor:processors)
            if(processor->controllerDeviceEvent(event)) return true;
        return false;
    }

    bool touchFingerEvent(const SDL_TouchFingerEvent& event){
        for(auto& InputProcessor processor:processors)
            if(processor->touchFingerEvent(event)) return true;
        return false;
    }
    bool keyboardEvent(const SDL_KeyboardEvent& event){
        for(auto& InputProcessor processor:processors)
            if(processor->keyboardEvent(event)) return true;
        return false;
    }

    bool mouseMotionEvent(const SDL_MouseMotionEvent& event){
        for(auto& InputProcessor processor:processors)
            if(processor->mouseMotionEvent(event)) return true;
        return false;
    }
    
    bool mouseButtonEvent(const SDL_MouseButtonEvent& event){
        for(auto& InputProcessor processor:processors)
            if(processor->mouseButtonEvent(event)) return true;
        return false;
    }
    
    bool mouseWheelEvent(const SDL_MouseWheelEvent& event){
        for(auto& InputProcessor processor:processors)
            if(processor->mouseWheelEvent(event)) return true;
        return false;
    }
    
    bool multiGestureEvent(const SDL_MultiGestureEvent& event){
        for(auto& InputProcessor processor:processors)
            if(processor->multiGestureEvent(event)) return true;
        return false;
    }  
};