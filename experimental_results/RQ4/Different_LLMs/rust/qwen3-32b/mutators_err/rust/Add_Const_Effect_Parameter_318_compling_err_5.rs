use crate::mutator::Mutator;
use syn::parse_quote;
use syn::FnArg;

pub struct Add_Const_Effect_Parameter_318;

impl Mutator for Add_Const_Effect_Parameter_318 {
    fn name(&self) -> &str {
        "Add_Const_Effect_Parameter_318"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_block) = item {
                if impl_block.trait_.is_some() {
                    for impl_item in &mut impl_block.items {
                        if let syn::ImplItem::Fn(func) = impl_item {
                            if func.sig.constness.is_some() {
                                let new_arg: FnArg = parse_quote! { const _: () };
                                func.sig.inputs.push(new_arg);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a const effect parameter with a unit type to const trait functions. This forces the compiler to validate the presence and usage of effect parameters in const contexts, potentially exposing bugs in effect system integration or const function verification."
    }
}