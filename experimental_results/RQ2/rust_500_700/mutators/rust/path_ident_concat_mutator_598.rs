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

pub struct Path_Ident_Concat_Mutator_598;

impl Mutator for Path_Ident_Concat_Mutator_598 {
    fn name(&self) -> &str {
        "Path_Ident_Concat_Mutator_598"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if let Some(mac_ident) = mac.mac.path.get_ident() {
                    if mac_ident == "macro_rules" {
                        if let Some(tt) = mac.mac.tokens.clone().into_iter().next() {
                            if let TokenTree::Group(group) = tt {
                                let mut new_tokens = TokenStream::new();
                                for token in group.stream() {
                                    if let TokenTree::Ident(ident) = &token {
                                        if ident == "concat_idents" {
                                            new_tokens.extend(quote! {
                                                concat_idents!($prefix, _middle, _, $name)
                                            });
                                        } else {
                                            new_tokens.extend(quote! { #ident });
                                        }
                                    } else {
                                        new_tokens.extend(quote! { #token });
                                    }
                                }
                                mac.mac.tokens = new_tokens;
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