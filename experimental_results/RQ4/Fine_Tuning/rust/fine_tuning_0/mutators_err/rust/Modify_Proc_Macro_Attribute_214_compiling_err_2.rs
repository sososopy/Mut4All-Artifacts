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

pub struct Modify_Proc_Macro_Attribute_214;

impl Mutator for Modify_Proc_Macro_Attribute_214 {
    fn name(&self) -> &str {
        "Modify_Proc_Macro_Attribute_214"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_identity_function = false;

        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "identity_function" {
                    has_identity_function = true;
                    break;
                }
            }
        }

        if !has_identity_function {
            let identity_function: ItemFn = parse_quote! {
                pub fn identity_function(input: TokenStream) -> TokenStream {
                    input
                }
            };
            file.items.push(Item::Fn(identity_function));
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.attrs.iter().any(|attr| attr.path().is_ident("proc_macro_attribute")) {
                    if let Some(index) = item_fn.block.stmts.iter().position(|stmt| {
                        if let Stmt::Expr(Expr::Macro(expr_macro), _) = stmt {
                            expr_macro.mac.path.is_ident("panic")
                        } else {
                            false
                        }
                    }) {
                        item_fn.block.stmts[index] = parse_quote! {
                            identity_function(item)
                        };
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}