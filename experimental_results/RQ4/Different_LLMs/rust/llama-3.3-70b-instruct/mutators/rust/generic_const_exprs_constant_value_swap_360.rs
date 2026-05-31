use rand::thread_rng;
use syn::{File, GenericParam, Item, ItemFn, Expr};
use crate::mutator::Mutator;

struct Generic_Const_Exprs_Constant_Value_Swap_360;

impl Mutator for Generic_Const_Exprs_Constant_Value_Swap_360 {
    fn name(&self) -> &str {
        "Generic_Const_Exprs_Constant_Value_Swap_360"
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator swaps the constant values of generic parameters in functions."
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.generics.params.is_empty() {
                    continue;
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(constant) = param {
                        // Swap constant value
                        if let Some(expr) = &constant.default {
                            if let Expr::Lit(_) = expr {
                                constant.default = Some(syn::parse_quote!(42));
                            }
                        }
                    }
                }
            }
        }
    }
}