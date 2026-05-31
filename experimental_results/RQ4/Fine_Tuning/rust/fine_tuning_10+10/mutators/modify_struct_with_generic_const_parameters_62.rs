use crate::mutator::Mutator;
use syn::{parse_quote, Item, GenericParam, Type, TypePath, Lifetime};
use proc_macro2::Span;

pub struct Modify_Struct_With_Generic_Const_Parameters_62;

impl Mutator for Modify_Struct_With_Generic_Const_Parameters_62 {
    fn name(&self) -> &str {
        "Modify_Struct_With_Generic_Const_Parameters_62"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                if let Some(GenericParam::Const(const_param)) = generics.params.iter_mut().find(|param| matches!(param, GenericParam::Const(_))) {
                    if let Type::Path(TypePath { path, .. }) = &const_param.ty {
                        if path.segments.iter().any(|seg| seg.ident == "TypeWithLifetime") {
                            let new_lifetime = Lifetime::new("'static", Span::call_site());
                            let new_type: Type = parse_quote! { SomeType<'static> };
                            const_param.ty = new_type;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator modifies struct definitions that use const generics. It identifies structs with const generic parameters involving types with lifetimes and alters the const parameter type to introduce a lifetime discrepancy. This change aims to exploit compiler weaknesses in handling lifetimes within const parameters, potentially triggering unresolved or conflicting lifetime issues."
    }
}