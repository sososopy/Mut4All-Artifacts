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

pub struct Change_Extern_RustCall_To_C_495;

impl Mutator for Change_Extern_RustCall_To_C_495 {
    fn name(&self) -> &str {
        "Change_Extern_RustCall_To_C_495"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(abi) = &func.sig.abi {
                    if abi.name.as_ref().map(|n| n.value()) == Some("rust-call".to_string()) {
                        func.sig.abi = Some(syn::Abi {
                            extern_token: abi.extern_token,
                            name: Some(syn::LitStr::new("C", abi.name.as_ref().unwrap().span())),
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}