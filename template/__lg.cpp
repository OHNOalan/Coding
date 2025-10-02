template<typename _Tp>
_Tp __lg(_Tp __n)
{
    return (sizeof(+__n) * __CHAR_BIT__ - 1)
           - (sizeof(+__n) == sizeof(long long)
            ? __builtin_clzll(+__n)
            : (sizeof(+__n) == sizeof(long)
             ? __builtin_clzl(+__n)
             : __builtin_clz(+__n)));
}