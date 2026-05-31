use proc_macro2::{Span, Ident};
use quote::*;
use rand::{Rng, thread_rng};
use syn::{
    GenericParam, File, FnArg, Type, TypePath, PathArguments, GenericArgument, Item, ItemFn,
    parse_quote, spanned::Spanned, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Alter_Lifetime_Params_In_Fn_Signatures_40;

impl Mutator for Alter_Lifetime_Params_In_Fn_Signatures_40 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Params_In_Fn_Signatures_40"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut rng = thread_rng();
                if let Some(bound_lifetimes) = func.sig.generics.params.iter_mut().find_map(|param| {
                    if let GenericParam::Lifetime(lifetime_def) = param {
                        Some(lifetime_def)
                    } else {
                        None
                    }
                }) {
                    let new_lifetime_ident = Ident::new(&format!("'{}", ('a' as u8 + rng.gen_range(0..26)) as char), Span::call_site());
                    bound_lifetimes.lifetime.ident = new_lifetime_ident.clone();
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(type_path) = &mut *pat_type.ty {
                                for segment in &mut type_path.path.segments {
                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let GenericArgument::Lifetime(lifetime) = arg {
                                                lifetime.ident = new_lifetime_ident.clone();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator alters the lifetime parameters in function signatures, particularly those involving higher-ranked trait bounds or complex lifetime relationships. By changing lifetime identifiers, it aims to provoke discrepancies in lifetime resolution and expose bugs related to lifetime constraints in the Rust compiler."
    }
}