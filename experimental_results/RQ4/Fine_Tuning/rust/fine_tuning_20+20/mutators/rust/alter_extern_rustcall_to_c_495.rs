use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Alter_Extern_RustCall_To_C_495;

impl Mutator for Alter_Extern_RustCall_To_C_495 {
    fn name(&self) -> &str {
        "Alter_Extern_RustCall_To_C_495"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let Some(abi) = &func.sig.abi {
                    if abi.name.is_some() && abi.name.as_ref().unwrap().value() == "rust-call" {
                        func.sig.abi = Some(Abi {
                            extern_token: token::Extern {
                                span: Span::call_site(),
                            },
                            name: Some(LitStr::new("C", Span::call_site())),
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with an `extern \"rust-call\"` ABI, changing it to `extern \"C\"`. By altering the calling convention, it aims to expose issues related to ABI-specific behavior, particularly in functions relying on Rust's unique \"rust-call\" mechanism, potentially leading to miscompilations or runtime errors."
    }
}