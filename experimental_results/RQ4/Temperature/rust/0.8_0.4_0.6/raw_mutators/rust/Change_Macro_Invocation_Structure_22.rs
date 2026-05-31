use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Macro, Pat, PatType, Path as SynPath,
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

pub struct Change_Macro_Invocation_Structure_22;

impl Mutator for Change_Macro_Invocation_Structure_22 {
    fn name(&self) -> &str {
        "Change_Macro_Invocation_Structure_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(macro_item) = item {
                if let Some(mac) = &macro_item.mac {
                    let mac_path = &mac.path;
                    if mac_path.segments.len() > 1 || mac.tokens.to_string().contains("{") {
                        let new_macro: Macro = parse_quote! {
                            #mac_path!(inline)
                        };
                        *item = Item::Macro(ItemMacro {
                            mac: new_macro,
                            attrs: macro_item.attrs.clone(),
                            semi_token: macro_item.semi_token,
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies macro invocations with complex paths or inline blocks and simplifies them to use a standard attribute or path. This transformation helps test the compiler's handling of simplified macro invocations and exposes potential issues related to complex macro structures."
    }
}