use proc_macro2::{Span, Ident};
use quote::*;
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit_mut::VisitMut, *,
};
use crate::mutator::Mutator;

pub struct Modify_Opaque_Type_Usage_279;

impl Mutator for Modify_Opaque_Type_Usage_279 {
    fn name(&self) -> &str {
        "Modify_Opaque_Type_Usage_279"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases = std::collections::HashMap::new();

        // Collect all type aliases with `impl Trait`
        for item in &file.items {
            if let Item::Type(type_item) = item {
                if let Type::ImplTrait(type_impl_trait) = &*type_item.ty {
                    type_aliases.insert(type_item.ident.clone(), type_impl_trait.clone());
                }
            }
        }

        // Modify function signatures using these type aliases
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut new_generics: Punctuated<GenericParam, token::Comma> = Punctuated::new();
                let mut to_replace = Vec::new();

                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(type_impl_trait) = type_aliases.get(&type_path.path.segments.last().unwrap().ident) {
                                let trait_bound = type_impl_trait.bounds.first().unwrap().clone();
                                let generic_ident = Ident::new("T", Span::call_site());
                                new_generics.push(GenericParam::Type(TypeParam {
                                    attrs: Vec::new(),
                                    ident: generic_ident.clone(),
                                    colon_token: Some(token::Colon { spans: [Span::call_site()] }),
                                    bounds: Punctuated::from_iter(vec![trait_bound]),
                                    eq_token: None,
                                    default: None,
                                }));
                                to_replace.push((pat_type, generic_ident));
                            }
                        }
                    }
                }

                // Apply replacements
                for (pat_type, generic_ident) in to_replace {
                    pat_type.ty = Box::new(Type::Path(TypePath {
                        qself: None,
                        path: Path::from(generic_ident),
                    }));
                }

                // Add new generics to the function signature
                func.sig.generics.params.extend(new_generics);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions using opaque types via type aliases and modifies their signatures to explicitly use generic parameters with trait bounds. This transformation replaces the alias with explicit trait constraints, testing the compiler's handling of impl Trait and type alias resolution."
    }
}