/* Subtract a value from the word at offset 0x30.
 *
 *   lw   a2, 0x30(a0)     ; previous value
 *   subu a1, a2, a1       ; subtract the argument
 *   jr   ra
 *   sw   a1, 0x30(a0)     ; store in the delay slot
 *
 * The surrounding object and the field's game meaning are still unknown.
 */
typedef struct WordAt30 {
    unsigned char padding[0x30];
    unsigned value;
} WordAt30;

void subtract_word_at_30(WordAt30 *object, unsigned amount)
{
    register unsigned previous asm("$6") = object->value;
    amount = previous - amount;
    object->value = amount;
}
