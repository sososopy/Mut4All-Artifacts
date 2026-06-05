
#pragma pack(1)
struct CPacked
{
	unsigned char m_EvilEpsilon;
	unsigned m_Data;
};

void testaligned(CPacked *pPacked)
{
	pPacked->m_Data = 0;
}
