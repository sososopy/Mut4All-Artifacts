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

pub struct Rename_Macro_Identifiers_95;

impl Mutator for Rename_Macro_Identifiers_95 {
    fn name(&self) -> &str {
        "Rename_Macro_Identifiers_95"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MacroRenamer;
        
        impl VisitMut for MacroRenamer {
            fn visit_item_macro_mut(&mut self, i: &mut syn::ItemMacro) {
                if let Some((_, ref mut tokens)) = i.mac.tokens.clone().into_iter().next() {
                    let mut new_tokens = proc_macro2::TokenStream::new();
                    let mut iter = tokens.clone().into_iter().peekable();
                    
                    while let Some(token) = iter.next() {
                        match &token {
                            proc_macro2::TokenTree::Ident(ident) => {
                                let new_ident = match ident.to_string().as_str() {
                                    "$crate" => Ident::new("crate", ident.span()),
                                    "$ty" => Ident::new("ty", ident.span()),
                                    "$fmt" => Ident::new("fmt", ident.span()),
                                    _ => ident.clone(),
                                };
                                new_tokens.extend(quote! { #new_ident });
                            }
                            proc_macro2::TokenTree::Literal(lit) => {
                                let lit_str = lit.to_string();
                                if lit_str.starts_with("$") {
                                    let quoted_lit = format!("\"{}\"", lit_str);
                                    new_tokens.extend(quote! { #quoted_lit });
                                } else {
                                    new_tokens.extend(quote! { #lit });
                                }
                            }
                            _ => new_tokens.extend(quote! { #token }),
                        }
                    }
                    
                    *tokens = new_tokens;
                }
                syn::visit_mut::visit_item_macro_mut(self, i);
            }
        }
        
        let mut renamer = MacroRenamer;
        renamer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation involves renaming identifiers within Rust macros, specifically those using macro_rules! syntax. Identifiers like $crate, $ty, and $fmt are checked and renamed to ensure correct formatting and avoid unknown prefixes. Additionally, literals that appear similar to prefixes are enclosed in quotes to prevent issues during macro processing."
    }
}