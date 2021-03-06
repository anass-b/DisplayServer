//
//  AppServer.cpp
//  appserverlib
//
//  Created by Anass on 2014-10-04.
//  Copyright (c) 2014 Anass Bouassaba. All rights reserved.
//

#include <iostream>
#include <memory>
#include <private/connector.h>
#include <asl/asl.h>

using namespace asl;

void* aslCreateContext()
{
    Connector* connector = new Connector();
    return connector;
}

void aslSubscribe(void* context)
{
    Connector* connector = (Connector*)context;
    connector->subscribe();
}

uint32_t aslCreateWindow(void* context, void* data, uint64_t dataSize, double x, double y, double width, double height, uint8_t rasterType)
{
    Connector* connector = (Connector*)context;
    return connector->createWindow(data, dataSize, x, y, width, height, rasterType);
}

void aslUpdateWindow(void* context, uint32_t windowId, void* data, uint64_t dataSize, double x, double y, double width, double height, bool compression)
{
    Connector* connector = (Connector*)context;
    connector->updateWindow(windowId, data, dataSize, x, y, width, height, compression);
}

void aslResizeWindow(void* context, uint32_t windowId, void* data, uint64_t dataSize, double width, double height)
{
    Connector* connector = (Connector*)context;
    connector->resizeWindow(windowId, data, dataSize, width, height);
}

void aslChangeWindowVisibility(void* context, uint32_t windowId, bool visible)
{
    Connector* connector = (Connector*)context;
    connector->changeWindowVisiblity(windowId, visible);
}

void aslBringWindowToFront(void* context, uint32_t windowId)
{
    Connector* connector = (Connector*)context;
    connector->bringWindowToFront(windowId);
}

void aslMoveWindow(void* context, uint32_t windowId, double x, double y)
{
    Connector* connector = (Connector*)context;
    connector->moveWindow(windowId, x, y);
}

void aslDestroyWindow(void* context, uint32_t windowId)
{
    Connector* connector = (Connector*)context;
    connector->destroyWindow(windowId);
}

AslEvent aslWaitEvent(void* context)
{
    Connector* connector = (Connector*)context;

    AslEvent aslEvt;

    // reset AslEvent
    aslEvt.type = -1;
    aslEvt.windowId = -1;
    aslEvt.type = -1;

    // reset AslInputEvent
    aslEvt.type = -1;

    // reset AslMouseEvent
    aslEvt.mouseEvent.buttonState = -1;
    aslEvt.mouseEvent.button = -1;
    aslEvt.mouseEvent.x = -1;
    aslEvt.mouseEvent.y = -1;
    aslEvt.mouseEvent.absX = -1;
    aslEvt.mouseEvent.absY = -1;
    aslEvt.mouseEvent.scrollX = -1;
    aslEvt.mouseEvent.scrollY = -1;

    // reset AslKeyEvent
    aslEvt.keyEvent.scancode = -1;
    aslEvt.keyEvent.keycode = -1;
    aslEvt.keyEvent.keymod = -1;
    aslEvt.keyEvent.state = -1;
    aslEvt.keyEvent.repeat = false;

    // reset AslTextEvent
    aslEvt.textEvent.text = nullptr;
    aslEvt.textEvent.textSize = 0;

    std::shared_ptr<Event> evt = connector->waitEvent();

    aslEvt.type = evt->getType();

    if (evt->getType() == AspEventTypeMouseButton) {
        std::shared_ptr<MouseButtonEvent> mouseButtonEvent = std::dynamic_pointer_cast<MouseButtonEvent>(evt);
        aslEvt.mouseEvent.buttonState = mouseButtonEvent->getState();
        aslEvt.mouseEvent.button = mouseButtonEvent->getButton();
        aslEvt.mouseEvent.x = mouseButtonEvent->getWindowX();
        aslEvt.mouseEvent.y = mouseButtonEvent->getWindowY();
        aslEvt.mouseEvent.absX = mouseButtonEvent->getX();
        aslEvt.mouseEvent.absY = mouseButtonEvent->getY();
    }
    else if (evt->getType() == AspEventTypeMouseMove) {
        std::shared_ptr<MouseMoveEvent> mouseMoveEvent = std::dynamic_pointer_cast<MouseMoveEvent>(evt);
        aslEvt.mouseEvent.x = mouseMoveEvent->getWindowX();
        aslEvt.mouseEvent.y = mouseMoveEvent->getWindowY();
        aslEvt.mouseEvent.absX = mouseMoveEvent->getX();
        aslEvt.mouseEvent.absY = mouseMoveEvent->getY();
    }
    else if (evt->getType() == AspEventTypeMouseScroll) {
        std::shared_ptr<MouseScrollEvent> mouseScrollEvent = std::dynamic_pointer_cast<MouseScrollEvent>(evt);
        aslEvt.mouseEvent.x = mouseScrollEvent->getWindowX();
        aslEvt.mouseEvent.y = mouseScrollEvent->getWindowY();
        aslEvt.mouseEvent.absX = mouseScrollEvent->getX();
        aslEvt.mouseEvent.absY = mouseScrollEvent->getY();
        aslEvt.mouseEvent.scrollX = mouseScrollEvent->getScrollX();
        aslEvt.mouseEvent.scrollY = mouseScrollEvent->getScrollY();
    }
    else if (evt->getType() == AspEventTypeKey) {
        std::shared_ptr<KeyEvent> keyEvent = std::dynamic_pointer_cast<KeyEvent>(evt);
        aslEvt.keyEvent.keycode = keyEvent->getKeycode();
        aslEvt.keyEvent.keymod = keyEvent->getKeymod();
        aslEvt.keyEvent.scancode = keyEvent->getScancode();
        aslEvt.keyEvent.repeat = keyEvent->getRepeat();
        aslEvt.keyEvent.state = keyEvent->getState();
    }
    else if (evt->getType() == AspEventTypeText) {
        std::shared_ptr<TextEvent> textEvent = std::dynamic_pointer_cast<TextEvent>(evt);
        aslEvt.textEvent.text = new char[textEvent->getText().size() + 1];
        strcpy(aslEvt.textEvent.text, textEvent->getText().c_str());
        aslEvt.textEvent.textSize = textEvent->getText().size();
    }

    return aslEvt;
}
