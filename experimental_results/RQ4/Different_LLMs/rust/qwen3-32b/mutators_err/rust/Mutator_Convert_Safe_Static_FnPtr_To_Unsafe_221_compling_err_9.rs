use crate::mutator::Mutator;
use syn::Expr;

pub struct Mutator_Convert_Safe_Static_FnPtr_To_Unsafe_221;

impl Mutator for Mutator_Convert_Safe_Static_FnPtr_To_Unsafe_221 {
    fn name(&self) -> &str {
        "Convert_Safe_Static_FnPtr_To_Unsafe_221"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(static_item) = item {
                if let syn::Type::BareFn(bare_fn) = &*static_item.ty {
                    if bare_fn.unsafety.is_none() {
                        if let Some(init) = static_item.expr.as_ref() {
                            if let Expr::Closure(_) = **init {
                                bare_fn.unsafety = Some(syn::token::Unsafe::default());
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}