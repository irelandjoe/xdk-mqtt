#include "xdkbutton.h"
#include "button.h"
#include "XdkBoardHandle.h"

#include "retcode.h"
#include "logging.h"

XDK_Retcode_E EnableButton(BUTTON_handle_tp* buttonHandle);
XDK_Retcode_E SetButtonCallback(BUTTON_handle_tp* buttonHandle, buttonHandler handler);

static BUTTON_handle_tp ButtonOneHandle = (BUTTON_handle_tp) NULL;

XDK_Retcode_E ButtonOneInit(buttonHandler handler)
{
    XDK_Retcode_E ret = XDK_RETCODE_FAILURE;
    ButtonOneHandle = BUTTON_create(gpioButton1_Handle, GPIO_STATE_OFF);

    DEBUG_PRINT("Button handle %x", (int)ButtonOneHandle);

    if(NULL != ButtonOneHandle)
    {
        if(XDK_RETCODE_SUCCESS == EnableButton(&ButtonOneHandle))
        {
            ret = SetButtonCallback(&ButtonOneHandle, handler);
        }
    }

    if(XDK_RETCODE_FAILURE == ret)
    {
        WARN_PRINT("Button creation FAILED!");
    }

    return ret;
}

XDK_Retcode_E EnableButton(BUTTON_handle_tp* buttonHandle)
{
    XDK_Retcode_E ret = XDK_RETCODE_SUCCESS;

    BUTTON_errorTypes_t buttonReturn = BUTTON_ERROR_INVALID_PARAMETER;
    buttonReturn = BUTTON_enable(*buttonHandle);
    if(BUTTON_ERROR_OK != buttonReturn)
    {
        WARN_PRINT("Enabling Button failed with status = %d!", (int)buttonReturn);
        ret = XDK_RETCODE_FAILURE;
    }
    return ret;
}

XDK_Retcode_E SetButtonCallback(BUTTON_handle_tp* buttonHandle, buttonHandler handler)
{
    XDK_Retcode_E ret = XDK_RETCODE_SUCCESS;

    BUTTON_errorTypes_t buttonReturn = BUTTON_ERROR_INVALID_PARAMETER;
    buttonReturn = BUTTON_setCallback(*buttonHandle, handler, BUTTONONE);
    if(BUTTON_ERROR_OK != buttonReturn)
    {
        WARN_PRINT("Setting callback for button failed with status = %d!", (int)buttonReturn);
        ret = XDK_RETCODE_FAILURE;
    }
    return ret;
}

int IsButtonPressed(void* handle)
{
    return BUTTON_isPressed(handle);
}

int IsButtonReleased(void* handle)
{
    return BUTTON_isReleased(handle);
}
