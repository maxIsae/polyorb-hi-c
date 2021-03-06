/*
 * This is a part of PolyORB-HI-C distribution, a minimal
 * middleware written for generated code from AADL models.
 * You should use it with the Ocarina toolsuite.
 *
 * For more informations, please visit http://taste.tuxfamily.org/wiki
 *
 * Copyright (C) 2014 ESA & ISAE.
 */

#if (defined (__PO_HI_NEED_DRIVER_1553_RASTA))\n
/*
Code automatically generated by asn1scc tool
Date was: 2/7/2012
Time was: 15:35:34
*/
#include <string.h>
#include <math.h>
#include "1553.h"


#ifndef _MSC_VER
#endif

void __po_hi_c_Node_Addr_T_Initialize(__po_hi_c_Node_Addr_T* pVal)
{
	*pVal = 0;
}


flag __po_hi_c_Node_Addr_T_IsConstraintValid(const __po_hi_c_Node_Addr_T* pVal, int* pErrCode) 
{
	if ( !(((*pVal>=0) && (*pVal<=31))) ) {
		*pErrCode = ERR___po_hi_c_Node_Addr_T;
		return FALSE;
	}
	(void)pVal; /*Dummy statement, just to hide potential warning*/
	(void)pErrCode; /*Dummy statement, just to hide potential warning*/
	return TRUE;
}




void __po_hi_c_Standard_T_Initialize(__po_hi_c_Standard_T* pVal)
{
	*pVal = __po_hi_c_mil1553a;
}


flag __po_hi_c_Standard_T_IsConstraintValid(const __po_hi_c_Standard_T* pVal, int* pErrCode) 
{
	(void)pVal; /*Dummy statement, just to hide potential warning*/
	(void)pErrCode; /*Dummy statement, just to hide potential warning*/
	return TRUE;
}




void __po_hi_c_Mode_T_Initialize(__po_hi_c_Mode_T* pVal)
{
	*pVal = __po_hi_c_controller;
}


flag __po_hi_c_Mode_T_IsConstraintValid(const __po_hi_c_Mode_T* pVal, int* pErrCode) 
{
	(void)pVal; /*Dummy statement, just to hide potential warning*/
	(void)pErrCode; /*Dummy statement, just to hide potential warning*/
	return TRUE;
}




void __po_hi_c_Bus_T_Initialize(__po_hi_c_Bus_T* pVal)
{
	*pVal = __po_hi_c_none;
}


flag __po_hi_c_Bus_T_IsConstraintValid(const __po_hi_c_Bus_T* pVal, int* pErrCode) 
{
	(void)pVal; /*Dummy statement, just to hide potential warning*/
	(void)pErrCode; /*Dummy statement, just to hide potential warning*/
	return TRUE;
}




void __po_hi_c_mil_1553_conf_t_Initialize(__po_hi_c_mil_1553_conf_t* pVal)
{
	memset(pVal->devname, 0x0, 21);
	__po_hi_c_Standard_T_Initialize(&pVal->standard);
	__po_hi_c_Mode_T_Initialize(&pVal->mode);
	__po_hi_c_Bus_T_Initialize(&pVal->bus);
	pVal->termaddr = 0;
	pVal->broadcast = FALSE;
	pVal->rxblock = FALSE;
	pVal->txblock = FALSE;
}


flag __po_hi_c_mil_1553_conf_t_IsConstraintValid(const __po_hi_c_mil_1553_conf_t* pVal, int* pErrCode) 
{
	if ( !(((strlen(pVal->devname)>=1) && (strlen(pVal->devname)<=20))) ) {
		*pErrCode = ERR___po_hi_c_mil_1553_conf_t_devname;
		return FALSE;
	}
	if ( !__po_hi_c_Standard_T_IsConstraintValid(&pVal->standard, pErrCode)) 
		return FALSE;

	if ( !__po_hi_c_Mode_T_IsConstraintValid(&pVal->mode, pErrCode)) 
		return FALSE;

	if ( !__po_hi_c_Bus_T_IsConstraintValid(&pVal->bus, pErrCode)) 
		return FALSE;

	if ( !__po_hi_c_Node_Addr_T_IsConstraintValid(&pVal->termaddr, pErrCode)) 
		return FALSE;


	if (pVal->exist.rxblock) {
	}
	if (pVal->exist.txblock) {
	}
	(void)pVal; /*Dummy statement, just to hide potential warning*/
	(void)pErrCode; /*Dummy statement, just to hide potential warning*/
	return TRUE;
}





\n#endif
