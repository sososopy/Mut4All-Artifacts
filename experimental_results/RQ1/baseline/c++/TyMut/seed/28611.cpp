
void remove_memory_blocks(pointer back_element_in_final_block)
{
   if constexpr(!std::is_trivially_destructible<element_type>::value)
   {
      // destroy each element in each memory block until the back_element is reached
   }

   // remove memory blocks
}

