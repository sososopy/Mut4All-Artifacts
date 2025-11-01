use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemMacro, ItemStruct, Lifetime, LifetimeParam, Local, Macro, Pat, PatType, Path as SynPath,
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

pub struct Modify_Macro_Rules_Remove_Unsupported_Syntax_358;

impl Mutator for Modify_Macro_Rules_Remove_Unsupported_Syntax_358 {
    fn name(&self) -> &str {
        "Modify_Macro_Rules_Remove_Unsupported_Syntax_358"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(ItemMacro { mac, .. }) = item {
                if mac.path.is_ident("macro_rules") {
                    let mut tokens = mac.tokens.clone().into_iter().collect::<Vec<_>>();
                    for token in &mut tokens {
                        if let TokenTree::Ident(ident) = token {
                            if ident == "box" {
                                *ident = Ident::new("Box::new", Span::call_site());
                            }
                        }
                    }
                    mac.tokens = tokens.into_iter().collect();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets macro_rules! macros that use the deprecated 'box' syntax. It replaces 'box' with 'Box::new()', ensuring compatibility with current Rust syntax. The operator identifies the 'box' keyword and modifies it while preserving the macro's functionality."
    }
}