#include "stdafx.h"
#include <stdio.h>
#include "md5.h"

int MD5_Process(UINT8 *pu8Data, int iLen, UINT8 * pu8Dst,int iB,int iE)
{ 
	if (pu8Data == NULL || pu8Dst == NULL)
	{
		return -1;
	}

	int ret = -1;
	MD5_CTX c;
	UINT8 md5[16]={0};
	MD5Init(&c);
	MD5Update(&c, pu8Data, iLen);  
	MD5Final(md5,&c); 
	ret = 0;

	int i = 0;
	for (int j = iB; j< iE && j<16 && j > 0; j++)
	{
		pu8Dst[i++] = md5[j];
	}

	return ret;
}

int Time_Encode( CTime t, UINT8 *pu8Dst )
{
	if (pu8Dst == NULL)
	{
		return -1;
	}

	int y = t.GetYear();
	int m = t.GetMonth();
	int d = t.GetDay();

	pu8Dst[0] = y-2001;
	pu8Dst[1] = m+7;
	pu8Dst[2] = d+9;

	return 0;
}


