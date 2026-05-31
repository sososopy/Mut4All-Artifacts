use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Change_Const_Generic_Associativity_98;

impl Mutator for Change_Const_Generic_Associativity_98 {
    fn name(&self) -> &str {
        "Change_Const_Generic_Associativity_98"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Const(const_item) = item {
                        let const_type = &const_item.ty;
                        let new_type = match const_type {
                            Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments,
                                },
                            }) => {
                                if segments.len() == 1 && segments[0].ident == "usize" {
                                    Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: punctuated::Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new("isize", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    })
                                } else {
                                    const_type.clone()
                                }
                            }
                            _ => const_type.clone(),
                        };
                        const_item.ty = Box::new(new_type);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait associated constants and changes their type from `usize` to `isize`. This transformation aims to test the compiler's handling of const generics and associated constants with different types, potentially triggering type mismatches or inference failures."
    }
}