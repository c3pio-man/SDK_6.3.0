namespace pocketbook
{
struct WrongFlag
{
    int value;
    inline explicit WrongFlag(int value) : value(value) {}
    inline operator int() const { return value; }
};
/**
 * @brief Flags class is used to make a typesafe structure
 * for storing bitmask
 * the way it should be used:
 *
 * enum SomeFlags {
 *     ALIGN_RIGHT = 1,
 *     ALIGN_BOTTOM = 2,
 *     ALIGN_TOP = 4,
 * };
 * PB_DECLARE_FLAGS(BitMask, SomeFlags);
 *
 * Then you may write:
 *
 * BitMask mask; // or like this Flags<SomeFlags> mask;
 * mask = ALIGN_RIGHT | ALIGN_TOP;
 * mask |= 2; // will cause a compiler error
 *
 * if (mask & ALIGN_TOP) {  \\ is valid
 * }
 * if (mask & 2) { \\ is not valid
 * }
 */
template<typename Enum>
class Flags
{
    int m_mask;
public:
    typedef Enum enum_type;
    constexpr inline Flags(const Flags<Enum> &f) : m_mask(f.m_mask) {}
    constexpr inline Flags(Enum f) : m_mask(f) {}
    constexpr inline Flags() : m_mask(0) {}

    inline Flags<Enum> &operator=(const Flags<Enum> &f) { m_mask = f.m_mask; return *this; }
    //inline Flags<Enum> &operator&=(int mask) { m_mask &= mask; return *this; }
    //inline Flags<Enum> &operator&=(uint mask) { m_mask &= mask; return *this; }
    inline Flags<Enum> &operator|=(Flags<Enum> f) { m_mask |= f.m_mask; return *this; }
    inline Flags<Enum> &operator|=(Enum f) { m_mask |= f; return *this; }
    inline Flags<Enum> &operator^=(Flags<Enum> f) { m_mask ^= f.m_mask; return *this; }
    inline Flags<Enum> &operator^=(Enum f) { m_mask ^= f; return *this; }

    constexpr  inline operator int() const { return m_mask; }

    constexpr inline Flags<Enum> operator|(Flags<Enum> f) const { return Flags<Enum>(Enum(m_mask | f.m_mask)); }
    constexpr inline Flags<Enum> operator|(Enum f) const { return Flags<Enum>(Enum(m_mask | f)); }
    constexpr inline Flags<Enum> operator^(Flags<Enum> f) const { return Flags<Enum>(Enum(m_mask ^ f.m_mask)); }
    constexpr inline Flags<Enum> operator^(Enum f) const { return Flags<Enum>(Enum(m_mask ^ f)); }
    //constexpr inline Flags<Enum> operator&(int mask) const { return Flags<Enum>(Enum(m_mask & mask)); }
    //constexpr inline Flags<Enum> operator&(unsigned int mask) const { return Flags<Enum>(Enum(m_mask & mask)); }
    constexpr inline Flags<Enum> operator&(Flags<Enum> flags) const { return Flags<Enum>(Enum(m_mask & flags.m_mask)); }
    constexpr inline Flags<Enum> operator&(Enum f) const { return Flags<Enum>(Enum(m_mask & f)); }
    constexpr inline Flags<Enum> operator~() const { return Flags<Enum>(Enum(~m_mask)); }

    constexpr inline bool operator!() const { return !m_mask; }

    inline bool testFlag(Enum f) const { return (m_mask & f) == f && (f != 0 || m_mask == int(f) ); }
    inline static Flags<Enum> fromInt(int value) {Flags<Enum> result; result.m_mask = value; return result;}
};

#define PB_DECLARE_OPERATORS_FOR_FLAGS(Name) \
constexpr inline Flags<Name::enum_type> operator|(Name::enum_type f1, Name::enum_type f2) \
{ return Flags<Name::enum_type>(f1) | f2; } \
constexpr inline Flags<Name::enum_type> operator|(Name::enum_type f1, Flags<Name::enum_type> f2) \
{ return f2 | f1; } \
constexpr inline Flags<Name::enum_type> operator^(Name::enum_type f1, Name::enum_type f2) \
{ return Flags<Name::enum_type>(f1) ^ f2; } \
constexpr inline Flags<Name::enum_type> operator^(Name::enum_type f1, Flags<Name::enum_type> f2) \
{ return f2 ^ f1; } \
inline Flags<Name::enum_type> operator~(Name::enum_type f) \
{ Flags<Name::enum_type> result(f); result = ~result; return result; } \
PB_DECLARE_BAD_FLAGS(Name)

#define PB_DECLARE_BAD_FLAGS(Name) \
inline WrongFlag operator|(Name::enum_type f1, int f2) \
{ return WrongFlag(int(f1) | f2); }

#define PB_DECLARE_FLAGS(Name, Enum)\
typedef Flags<Enum> Name; \
PB_DECLARE_OPERATORS_FOR_FLAGS(Name)

}
