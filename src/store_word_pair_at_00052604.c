/* The pair fields are known as words; their higher-level meaning is not. */
typedef struct {
    unsigned first;
    unsigned second;
} WordPair;

WordPair *store_word_pair_at_00052604(WordPair *result, unsigned first, unsigned second)
{
    result->first = first;
    result->second = second;
    /* Keep the return value live through both stores; this emits no code. */
    __asm__ volatile ("" : "+r" (result) : : "memory");
    return result;
}
