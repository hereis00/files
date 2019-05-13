#include <cstdio>
#include <string>
using std::string;

#define FILE_NAME_FF 01
#define VALUE_STRING_LEN 60
#define LEN 8
struct A
{
	int iFileSeqNbr;
	int iSmallFileSeqNbr;
};

int iProcessIndex = 33, iCombineSeq = 0;
void AllTrim(char *sBuffer);
int ParseString(char *, A, string &);
/**/
int main(void)
{
	int iFileSeqNbr = 55, iSmallFileSeqNbr = 44, iProcessIndex = 33, iCombineSeq = 1;
	A a{ 55, 44 };
	string strValue;
	char value_string[LEN][VALUE_STRING_LEN + 1] =
	{ 
		"MAX.BBBBB.FFF.NNNN",
		"MAX.BBBBB.FFF.NNNN",
		"MAX.NNNN.FFFF",
		"MAX.BBBBB.MAX.BBB.MAX.BBB.MAX",
		"12MAX3!@#BBB*FFFFNNNMAX",
		"FFFF.NN.XAM.BBBB",
		"-BBBB-MAX-FFFFF-NNN-"
	};
	for (int i = 0; i < LEN; i++)
	{
		ParseString(value_string[i], a, strValue);
		printf("%s\n", strValue.c_str());
		strValue.clear();
	}
	getchar();
	return 0;
}

void AllTrim(char *sBuffer)
{
	long    lIndex = 0;
	long    lFirstCharFlag = 0;
	long    lFirstPos = 0;
	long    lEndPos = 0;

	for (lIndex = 0; sBuffer[lIndex] != '\0'; lIndex++)
	{
		if ((sBuffer[lIndex] == ' ')
			|| (sBuffer[lIndex] == '\t')
			)
		{
			if (lFirstCharFlag == 0)
			{
				lFirstPos++;
			}
		}
		else
		{
			lFirstCharFlag = 1;
			lEndPos = lIndex;
		}
	}/*  abcde\0*/
	if (lFirstPos == lIndex)
		sBuffer[0] = '\0';
	else
	{
		memmove(sBuffer, sBuffer + lFirstPos, lEndPos - lFirstPos + 1);
		sBuffer[lEndPos - lFirstPos + 1] = '\0';
	}

	return;
}

int ParseString(char *value_string, A t, string &strValue)
{
	static char sLastValueString[VALUE_STRING_LEN + 1] = { 0 };	// 保存上次解析的字符串
	static char st_asSeqMode[VALUE_STRING_LEN * 2] = { 0 };		// 保存上次解析后的模式
	static int  iBCount = 0;									// 字符B的个数，用来设定MAX的值，默认999

	char sValueString[VALUE_STRING_LEN + 1] = { 0 };	// 拷贝value_string
	char sSubSeqNbr[VALUE_STRING_LEN + 1] = { 0 }; 	// 每一子串拼接的序号

	AllTrim(value_string);

	// value_string为0，取4位大序号
	if (0 == strlen(value_string))
	{
		sprintf(sSubSeqNbr, "%04d", t.iFileSeqNbr);
		strValue.append(sSubSeqNbr);
		return 0;
	}

	if (strcmp(value_string, sLastValueString) != 0)	// 与上次不同，需要解析
	{
		sprintf(sLastValueString, value_string);	// 保存
		sprintf(sValueString, value_string);	// 拷贝
		int iStringCount = 0;	// 有多少子串
		bool bValid = true;		// 解析是否成功
		iBCount = 0;

		for (int i = 0; sValueString[i] != '\0'; i++)
		{
			st_asSeqMode[iStringCount * 2] = sValueString[i];
			int iCharCount = 1;
			while (sValueString[i] == sValueString[i + 1])
			{
				i++;
				iCharCount++;
			}
			if (('A' <= sValueString[i] && sValueString[i] <= 'Z') || ('a' <= sValueString[i] && sValueString[i] <= 'z'))
			{
				if (!(sValueString[i] == 'B' || sValueString[i] == 'F' || sValueString[i] == 'N' || sValueString[i] == 'M'))
				{
					bValid = false;
					break;
				}
			}
			switch (sValueString[i])
			{
			case 'B':
			{
				iBCount = iBCount > iCharCount ? iBCount : iCharCount;
			}
			break;
			case 'M':
			{
				if ('A' == sValueString[i + 1] && 'X' == sValueString[i + 2])
				{
					i += 2;
					iCharCount = 3;
				}
				else
				{
					bValid = false;
				}
			}
			break;
			case 'N':	// 这里可以没有break
			case 'F':	// 这里可以没有break
			default:	// 其他字符都当作分隔符
				break;
			}
			st_asSeqMode[iStringCount * 2 + 1] = iCharCount;
			iStringCount++;
		}
		if (!bValid)	// 解析失败，使用4位大序号
		{
			memset(st_asSeqMode, 0, VALUE_STRING_LEN + 1);
			st_asSeqMode[0] = 'B';
			st_asSeqMode[1] = 4;
			st_asSeqMode[2] = '\0';
		}
		st_asSeqMode[iStringCount * 2] = '\0';
	}

	iBCount = iBCount == 0 ? 3 : iBCount;
	for (int i = 0; st_asSeqMode[i * 2] != '\0'; i++)
	{
		switch (st_asSeqMode[i * 2])
		{
		case 'B':
		{
			sprintf(sSubSeqNbr, "%0*d", st_asSeqMode[i * 2 + 1], t.iFileSeqNbr);
		}
		break;
		case 'N':
		{
			sprintf(sSubSeqNbr, "%0*d", st_asSeqMode[i * 2 + 1], t.iSmallFileSeqNbr);
		}
		break;
		case 'F':
		{
			if (iCombineSeq == 1)
			{
				sprintf(sSubSeqNbr, "%0*d", st_asSeqMode[i * 2 + 1], FILE_NAME_FF);
			}
			else
			{
				sprintf(sSubSeqNbr, "%0*d", st_asSeqMode[i * 2 + 1], iProcessIndex);
			}
		}
		break;
		case 'M':
		{
			memset(sSubSeqNbr, '9', iBCount);
			sSubSeqNbr[iBCount] = 0;
		}
		break;
		default:	// 其他字符都算成分隔符
		{
			memset(sSubSeqNbr, st_asSeqMode[i * 2], st_asSeqMode[i * 2 + 1]);
			sSubSeqNbr[st_asSeqMode[i * 2 + 1]] = '\0';
		}
		break;
		}
		strValue.append(sSubSeqNbr);
	}

}