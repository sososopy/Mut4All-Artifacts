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

pub struct Opaque_Type_Alteration_14;

impl Mutator for Opaque_Type_Alteration_14 {
    fn name(&self) -> &str {
        "Opaque_Type_Alteration_14"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut insert_debug_use = false;

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut new_bounds = Punctuated::new();
                        let mut found_sized_or_clone = false;

                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Sized") {
                                    found_sized_or_clone = true;
                                    new_bounds.push(parse_quote!(Clone));
                                } else if trait_bound.path.is_ident("Clone") {
                                    found_sized_or_clone = true;
                                    new_bounds.push(parse_quote!(Sized));
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                        }

                        if !found_sized_or_clone {
                            new_bounds.push(parse_quote!(Debug));
                            insert_debug_use = true;
                        }

                        *return_type = Box::new(Type::ImplTrait(TypeImplTrait {
                            impl_token: type_impl_trait.impl_token,
                            bounds: new_bounds,
                        }));
                    }
                }
            }
        }

        if insert_debug_use && !file.items.iter().any(|item| matches!(item, Item::Use(use_item) if use_item.tree.to_token_stream().to_string().contains("std::fmt::Debug"))) {
            file.items.insert(0, parse_quote!(use std::fmt::Debug;));
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}