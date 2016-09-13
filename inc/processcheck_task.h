/*
 * processcheck_task.h
 *
 *  Created on: Aug 12, 2016
 *      Author: krcka
 */

#ifndef INC_PROCESSCHECK_TASK_H_
#define INC_PROCESSCHECK_TASK_H_

#define TASK_CREATE_WITHCHECK(task, name, stack, priority, handle) \
                xTaskCreate(task, (const char *)name, stack, NULL, priority, &handle); \
                ProcessCheck_RegisterProc(handle, name)

void ProcessCheck_Init(void);
void ProcessCheck_RegisterProc(xTaskHandle pvProc, const char * const name);
void ProcessCheck_SetMaxState(xTaskHandle pvProc, int nMaxState);
void ProcessCheck_ControlFlag(xTaskHandle pvProc);


#endif /* INC_PROCESSCHECK_TASK_H_ */
