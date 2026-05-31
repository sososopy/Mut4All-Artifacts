```rust
use rand::thread_rng;
use syn::{File, GenericParam, Item, ItemFn};
use crate::mutator::Mutator;

struct Generic_Const_Exprs_Constant_Value_Swap_360;

impl Mutator for Generic_Const_Exprs_Constant_Value_Swap_360 {
    fn name(&self) -> &str {
        "Generic_Const_Exprs_Constant_Value_Swap_360"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let syn::Generics::None = func.sig.generics {
                    continue;
                }
                let generics = func.sig.generics.get_mut().unwrap();
                let const_params: Vec<&mut GenericParam> = generics
                    .params
                    .iter_mut()
                    .filter(|param| {
                        if let GenericParam::Const(_) = param {