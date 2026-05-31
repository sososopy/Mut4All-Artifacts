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

pub struct Modify_Default_Type_Const_Generic_381;

impl Mutator for Modify_Default_Type_Const_Generic_381 {
    fn name(&self) -> &str {
        "Modify_Default_Type_Const_Generic_381"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut modified = false;
                for param in &mut item_struct.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(default) = &const_param.default {
                            if let syn::Expr::Path(expr_path) = &**default {
                                if expr_path.path.segments.last().unwrap().ident == "u8" {
                                    const_param.default = Some(Box::new(parse_quote!(bool)));
                                    modified = true;
                                }
                            }
                        }
                    }
                }
                if modified {
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}