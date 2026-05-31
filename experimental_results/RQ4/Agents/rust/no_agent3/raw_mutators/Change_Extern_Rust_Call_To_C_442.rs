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

pub struct Change_Extern_Rust_Call_To_C_442;

impl Mutator for Change_Extern_Rust_Call_To_C_442 {
    fn name(&self) -> &str {
        "Change_Extern_Rust_Call_To_C_442"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                if let Some(abi) = &func.abi {
                    if abi.name.is_some() && abi.name.as_ref().unwrap().value() == "rust-call" {
                        func.abi = Some(syn::Abi {
                            extern_token: abi.extern_token,
                            name: Some(syn::LitStr::new("C", abi.name.as_ref().unwrap().span())),
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions defined with the 'extern' keyword and the 'rust-call' ABI. The mutation changes the ABI from 'rust-call' to 'C', while maintaining the function's signature. This alteration can expose potential inconsistencies or bugs related to function calling conventions, especially in scenarios where specific ABIs are crucial for correct execution."
    }
}