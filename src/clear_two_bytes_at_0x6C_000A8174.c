/* Clear the two adjacent flag bytes at offsets 0x6C and 0x6D. */
typedef struct TwoFlagBytesAt6C {
    unsigned char padding[0x6C];
    unsigned char first;
    unsigned char second;
} TwoFlagBytesAt6C;

void clear_two_bytes_at_0x6C_000A8174(
    int unused0, int unused1, TwoFlagBytesAt6C *object)
{
    object->second = 0;
    object->first = 0;
}
