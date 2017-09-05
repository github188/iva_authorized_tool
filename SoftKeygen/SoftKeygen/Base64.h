/***************************************************************
ģ����	     ��base�����ļ�                       
�ļ���	     ��base64.h
����ļ�	 ��
�ļ�ʵ�ֹ��� �������������ݼ���
����		 ��   ֣��
�汾		 ��V1.0
-----------------------------------------------------------------
�޸ļ�¼:
��  ��		    �汾		�޸���		�޸�����
2012/11/30		1.0      ֣��        ��ʼ�汾
*****************************************************************/

#if !defined( __FHNMEDIASTREAM_H_BASE64_H__)
#define __FHNMEDIASTREAM_H_BASE64_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

int Base64encode_len(int len);
int Base64encode(char * coded_dst, const char *plain_src,int len_plain_src);

int Base64decode_len(const char * coded_src);
int Base64decode(char * plain_dst, const char *coded_src);



#endif //_BASE64_H_


