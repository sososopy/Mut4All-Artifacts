use rand::Rng;
use rand::thread_rng;
use syn::{File, Item, ItemStruct, punctuated::Punctuated};

struct Swap_Field_Order_181;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Swap_Field_Order_181 {
    fn name(&self) -> &str {
        "Swap_Field_Order_181"
    }
    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let syn::Fields::Named(fields) = &mut struct_item.fields {
                    if fields.named.len() >= 2 {
                        let mut rng = thread_rng();
                        let idx1 = rng.gen_range(0..fields.named.len());
                        let idx2 = rng.gen_range(0..fields.named.len());
                        if idx1 != idx2 {
                            fields.named.swap(idx1, idx2);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of two fields in a struct definition. This transformation tests the compiler's handling of struct layouts and field ordering, potentially triggering bugs in the layout and field system."
    }
}