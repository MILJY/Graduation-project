#include "protocol.h"
PDU *mkPDU(uint msg_len)
{
    uint pdu_len = sizeof(PDU) + msg_len;
    PDU *pdu = (PDU *)malloc(pdu_len);
    if(pdu == NULL)
    {
        exit(EXIT_FAILURE);
    }
    memset(pdu, 0 , pdu_len);
    pdu->msg_len = msg_len;
    pdu->pdu_len = pdu_len;
    return pdu;
}
