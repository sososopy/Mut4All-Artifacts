use syn::parse_quote;
use crate::mutator::Mutator;

pub struct Replace_Param_With_Self_In_Trait_ExternC_201;

impl Mutator for Replace_Param_With_Self_In_Trait_ExternC_201 {
    fn name(&self) -> &str {
        "Replace_Param_With_Self_In_Trait_ExternC_201"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_method in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_method {
                        if method.sig.abi.as_ref().map_or(false, |abi| {
                            abi.name.as_ref().map_or(false, |lit| lit.value() == "C")
                        }) {
                            if let Some(first_arg) = method.sig.inputs.iter_mut().next() {
                                if let syn::FnArg::Typed(pat_type) = first_arg {
                                    let new_pat_type: syn::PatType = parse_quote! { self: Self };
                                    *pat_type = new_pat_type;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a parameter in an extern \"C\" function within a trait with `Self` as the type. This targets lint checks for FFI-compatibility when the compiler attempts to determine sizedness of `Self` in an extern context. It introduces potential type mismatches and challenges the compiler's handling of `Self` in FFI function signatures."
    }
}