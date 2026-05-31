impl Mutator for Swap_Generic_Parameter_Order_396 {
    fn name(&self) -> &str {
        "Swap_Generic_Parameter_Order_396"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics {
                    let mut params: Vec<GenericParam> = generics.params.clone();
                    if params.len() > 1 {
                        // Swap the order of the first two generic parameters
                        params.swap(0, 1);
                        generics.params = params;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of generic parameters in struct definitions. This transformation tests the Rust compiler's handling of generic parameter order and default const expressions, which is relevant to the F-generic_const_exprs bug label."
    }
}

impl Swap_Generic_Parameter_Order_396 {
    pub fn new() -> Self {
        Swap_Generic_Parameter_Order_396
    }
}