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

pub struct Modify_Macro_Rules_To_Remove_Unsupported_Syntax_236;

impl Mutator for Modify_Macro_Rules_To_Remove_Unsupported_Syntax_236 {
    fn name(&self) -> &str {
        "Modify_Macro_Rules_To_Remove_Unsupported_Syntax_236"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::MacroRules(macro_rules) = item {
                for rule in &mut macro_rules.rules {
                    rule.transform = rule.transform.clone().and_then(|mut transform| {
                        transform.macros = transform
                            .macros
                            .into_iter()
                            .map(|mut mac| {
                                if mac.path.is_ident("box") {
                                    let tokens = mac.tokens;
                                    mac.path = parse_quote!(Box::new);
                                    mac.tokens = parse_quote!((#tokens));
                                }
                                mac
                            })
                            .collect();
                        Some(transform)
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Locate macro_rules! definitions and identify usages of the deprecated `box` syntax. Replace `box` with `Box::new()` to ensure compatibility with current Rust syntax, maintaining the macro's functionality while adhering to updated language standards."
    }
}