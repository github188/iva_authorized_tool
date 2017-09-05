/***************************************************************
模块名	     ：base加密文件                       
文件名	     ：base64.h
相关文件	 ：
文件实现功能 ：处理智能数据加密
作者		 ：   郑靖
版本		 ：V1.0
-----------------------------------------------------------------
修改记录:
日  期		    版本		修改人		修改内容
2012/11/30		1.0      郑靖        初始版本
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


