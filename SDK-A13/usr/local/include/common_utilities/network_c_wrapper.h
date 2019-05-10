#ifndef NETWORK_C_WRAPPER_H
#define NETWORK_C_WRAPPER_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void NetworkCWrapper_SwitchNetworkManagerAsync(int status, void (*cb)());

int NetworkCWrapper_IsNetworkManagerTurnedOn();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // NETWORK_C_WRAPPER_H
