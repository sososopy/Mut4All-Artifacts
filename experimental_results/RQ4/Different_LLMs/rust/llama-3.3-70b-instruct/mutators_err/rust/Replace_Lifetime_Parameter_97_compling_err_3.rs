use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Lifetime_Parameter_97;

impl Mutator for Replace_Lifetime_Parameter_97 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameter_97"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_def) = item {
                if let Some(generics) = &mut struct_def.generics {
                    let mut lifetimes: Vec<LifetimeParam> = generics
                        .params
                        .iter()
                        .filter_map(|param| match param {
                            GenericParam::Lifetime(lifetime) => Some(lifetime.clone()),
                            _ => None,
                        })
                        .collect();

                    if lifetimes.len() > 1 {
                        let mut rng = thread_rng();
                        let index_to_replace = rng.gen_range(0..lifetimes.len());
                        let replacement_lifetime = lifetimes
                            .iter()
                            .filter(|lifetime| lifetime.lifetime != lifetimes[index_to_replace].lifetime)
                            .choose(&mut rng)
                            .unwrap()
                            .clone();

                        for param in &mut generics.params {
                            if let GenericParam::Lifetime(lifetime) = param {
                                if lifetime.lifetime == lifetimes[index_to_replace].lifetime {
                                    lifetime.lifetime = replacement_lifetime.lifetime.clone();
                                }
                            }
                        }

                        for field in &mut struct_def.fields {
                            match field {
                                syn::Fields::Named(fields_named) => {
                                    for field_named in &mut fields_named.named {
                                        if let Some(ty) = &mut field_named.ty {
                                            if let Type::Reference(type_ref) = ty {
                                                if let Some(lifetime) = &type_ref.lifetime {
                                                    if lifetime == &lifetimes[index_to_replace].lifetime {
                                                        type_ref.lifetime = Some(replacement_lifetime.lifetime.clone());
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                _ => {}
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a lifetime parameter in a struct definition with another existing lifetime parameter in the same scope. This transformation tests the compiler's handling of lifetime parameters and their substitution in generic types."
    }
}