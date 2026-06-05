

  for (i = 0; i < n_nodes; i++)
    {
      if (symbol_partitioned_p ((symtab_node) order[i]))
        continue;

      current_order = order[i]->symbol.order;

      if (!flag_toplevel_reorder)
        while (varpool_pos < n_varpool_nodes
               && varpool_order[varpool_pos]->symbol.order < current_order)
          {
            if (!symbol_partitioned_p ((symtab_node) varpool_order[varpool_pos]))
              add_symbol_to_partition (partition, (symtab_node) varpool_order[varpool_pos]);
            varpool_pos++;
          }

      add_symbol_to_partition (partition, (symtab_node) order[i]);
      total_size -= inline_summary (order[i])->size;
