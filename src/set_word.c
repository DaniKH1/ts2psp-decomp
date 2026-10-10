/* Store a word in the first slot of a small value object.
 * Retail code: jr ra; sw a1, 0(a0).
 */
typedef struct WordSlot {
    unsigned value;
} WordSlot;

void set_word(WordSlot *slot, unsigned value)
{
    slot->value = value;
}
