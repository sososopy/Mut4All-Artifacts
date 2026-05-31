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

pub struct Replace_Const_Generic_With_NonSupported_Type_397;

impl Mutator for Replace_Const_Generic_With_NonSupported_Type_397 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_NonSupported_Type_397"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_type_declared = false;
        let mut new_type: Option<ItemStruct> = None;

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if matches!(const_param.ty, Type::Path(_)) {
                            if !new_type_declared {
                                new_type = Some(parse_quote! {
                                    struct Dimension;
                                });
                                new_type_declared = true;
                            }
                            const_param.ty = parse_quote!(Dimension);
                        }
                    }
                }
            }
            if let Item::Fn(item_fn) = item {
                for param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if matches!(const_param.ty, Type::Path(_)) {
                            if !new_type_declared {
                                new_type = Some(parse_quote! {
                                    struct Dimension;
                                });
                                new_type_declared = true;
                            }
                            const_param.ty = parse_quote!(Dimension);
                        }
                    }
                }
            }
            if let Item::Trait(item_trait) = item {
                for param in &mut item_trait.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if matches!(const_param.ty, Type::Path(_)) {
                            if !new_type_declared {
                                new_type = Some(parse_quote! {
                                    struct Dimension;
                                });
                                new_type_declared = true;
                            }
                            const_param.ty = parse_quote!(Dimension);
                        }
                    }
                }
            }
        }

        if let Some(new_type) = new_type {
            file.items.insert(0, Item::Struct(new_type));
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}