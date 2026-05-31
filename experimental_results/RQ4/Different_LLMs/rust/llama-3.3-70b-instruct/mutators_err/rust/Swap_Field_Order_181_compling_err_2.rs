impl Mutator for Swap_Field_Order_181 {
    fn name(&self) -> &str {
        "Swap_Field_Order_181"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if struct_item.fields.len() >= 2 {
                    let mut rng = thread_rng();
                    let fields: Vec<_> = struct_item.fields.iter().collect();
                    let idx1 = rng.gen_range(0..fields.len());
                    let idx2 = rng.gen_range(0..fields.len());
                    if idx1 != idx2 {
                        let fields_mut: &mut Vec<_> = struct_item.fields.as_mut();
                        fields_mut.swap(idx1, idx2);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of two fields in a struct definition. This transformation tests the compiler's handling of struct layouts and field ordering, potentially triggering bugs in the layout and field system."
    }
}