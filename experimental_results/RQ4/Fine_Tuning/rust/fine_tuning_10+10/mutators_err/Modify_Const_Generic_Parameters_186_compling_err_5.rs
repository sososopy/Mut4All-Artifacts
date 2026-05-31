use crate::mutator::Mutator;
use syn::{parse_quote, GenericParam, Item, ItemStruct, Type, TypePath};

pub struct Modify_Const_Generic_Parameters_186;

impl Mutator for Modify_Const_Generic_Parameters_186 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameters_186"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, .. }) = item {
                for param in &mut generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(TypePath { path, .. }) = &*const_param.ty {
                            if !matches!(path.segments.last().unwrap().ident.to_string().as_str(), "usize" | "bool" | "char") {
                                const_param.ty = parse_quote!(usize);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with const generic parameters that are not of type integers, bool, or char. It modifies these const generic parameters to use a supported type, such as usize. This change ensures compliance with Rust's const generic constraints and tests the robustness of the compiler's const evaluation path."
    }
}