use syn::{parse_quote, Attribute, Meta, MetaList, MetaNameValue};
use crate::mutator::Mutator;

struct Remove_Must_Use_Attribute_197;

impl Mutator for Remove_Must_Use_Attribute_197 {
    fn name(&self) -> &str {
        "Remove_Must_Use_Attribute_197"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                func.attrs.retain(|attr| {
                    if let Ok(met) = attr.parse_meta() {
                        if let Meta::Path(path) = met {
                            if path.is_ident("must_use") {
                                return false;
                            }
                        }
                    }
                    true
                });
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator removes the `must_use` attribute from functions, testing the compiler's handling of return value usage and potential bugs related to the attribute's interaction with function calls and return values."
    }
}