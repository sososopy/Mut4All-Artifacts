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

pub struct Alter_Macro_Invocation_With_Unexpected_Cfg_231;

impl Mutator for Alter_Macro_Invocation_With_Unexpected_Cfg_231 {
    fn name(&self) -> &str {
        "Alter_Macro_Invocation_With_Unexpected_Cfg_231"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if let Some(mac) = item_macro.mac.path.segments.last() {
                    if mac.ident == "the_macro" {
                        let mut new_tokens = TokenStream::new();
                        new_tokens.extend(item_macro.mac.tokens.clone());
                        new_tokens.extend(quote! {
                            #[cfg(unexpected)]
                            $foo
                        });
                        item_macro.mac.tokens = new_tokens;
                    }
                }
            }
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Macro(stmt_macro) = stmt {
                        if let Some(mac) = stmt_macro.mac.path.segments.last() {
                            if mac.ident == "the_macro" {
                                let mut new_tokens = TokenStream::new();
                                new_tokens.extend(stmt_macro.mac.tokens.clone());
                                new_tokens.extend(quote! {
                                    #[cfg(unexpected)]
                                    ()
                                });
                                stmt_macro.mac.tokens = new_tokens;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}