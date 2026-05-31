use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, *,
};

use crate::mutator::Mutator;

pub struct Modify_Const_Generic_Lifetimes_103;

impl Mutator for Modify_Const_Generic_Lifetimes_103 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Lifetimes_103"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut existing_lifetimes = HashSet::new();

        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                for generic_param in &item_fn.sig.generics.params {
                    if let GenericParam::Lifetime(lifetime_def) = generic_param {
                        existing_lifetimes.insert(lifetime_def.lifetime.ident.clone());
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut rng = thread_rng();
                let new_lifetime_ident = Ident::new("'a", Span::call_site());

                for generic_param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = generic_param {
                        if let Type::Reference(type_ref) = &const_param.ty {
                            if let Some(lifetime) = &type_ref.lifetime {
                                let new_lifetime = if existing_lifetimes.is_empty() {
                                    new_lifetime_ident.clone()
                                } else {
                                    existing_lifetimes
                                        .iter()
                                        .cloned()
                                        .collect::<Vec<_>>()
                                        .choose(&mut rng)
                                        .unwrap_or(&new_lifetime_ident)
                                        .clone()
                                };
                                const_param.ty = Type::Reference(TypeReference {
                                    and_token: type_ref.and_token,
                                    lifetime: Some(Lifetime {
                                        apostrophe: lifetime.apostrophe,
                                        ident: new_lifetime,
                                    }),
                                    mutability: type_ref.mutability,
                                    elem: type_ref.elem.clone(),
                                });
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator targets function signatures using const generics with lifetimes. It modifies the lifetime specifier of the const generic parameter, either changing it to another existing lifetime or introducing a new one. This transformation tests the compiler's ability to handle lifetime changes in const generics and ensures compatibility with the rest of the code."
    }
}