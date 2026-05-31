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

pub struct Replace_External_Function_ABI_495;

impl Mutator for Replace_External_Function_ABI_495 {
    fn name(&self) -> &str {
        "Replace_External_Function_ABI_495"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(abi) = &func.sig.abi {
                    let new_abi = match abi.name.to_string().as_str() {
                        "C" => "rust-call",
                        "rust-call" => "C",
                        _ => abi.name.to_string(),
                    };
                    func.sig.abi = Some(syn::Abi {
                        name: Ident::new(new_abi, abi.name.span()),
                        ..abi.clone()
                    });
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the ABI of a function with an external ABI, for example, replace 'extern rust-call' with 'extern c' or vice versa. This transformation aims to test the compiler's handling of different external ABIs, which may trigger bugs like the one reported in the issue."
    }
}