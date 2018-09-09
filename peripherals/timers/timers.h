#ifndef TIMERS_H_
#define TIMERS_H_

#ifdef __cplusplus
extern "C"{
#endif  /* __cplusplus */

// Functions
void usWaitTimer1(unsigned int);
void msWaitTimer1(unsigned int);
void usInterruptTimer1(unsigned int, void (*)(void));
void msInterruptTimer1(unsigned int, void (*)(void));
void usWaitTimer2(unsigned int);
void usInterruptTimer2(unsigned int, void (*)(void));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TIMERS_H_ */
