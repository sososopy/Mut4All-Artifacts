struct ScriptObjectFixture
{
    static const int code_size;
    static const char code[];
    static wchar_t uc_code[];

    ScriptObjectFixture()
    {
        for (int i = 0; i < code_size; i++)
            uc_code[i] = 0;
    }

};

const char ScriptObjectFixture::code[] = "x";
const int ScriptObjectFixture::code_size = sizeof(ScriptObjectFixture::code) - 1;
wchar_t ScriptObjectFixture::uc_code[ScriptObjectFixture::code_size];


