use rand::Rng;
use rand::thread_rng;
use syn::{File, Item, ItemStruct};

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
                if struct_item.fields.len() >= 2 {
                    let mut rng = thread_rng();
                    let idx1 = rng.gen_range(0..struct_item.fields.len());
                    let idx2 = rng.gen_range(0..struct_item.fields.len());
                    if idx1 != idx2 {
                        struct_item.fields.swap(idx1, idx2);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of two fields in a struct definition. This transformation tests the compiler's handling of struct layouts and field ordering, potentially triggering bugs in the layout and field system."
    }
}