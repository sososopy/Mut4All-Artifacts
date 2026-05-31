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

pub struct Insert_Empty_Attribute_Before_Stmt_Matcher_203;

impl Mutator for Insert_Empty_Attribute_Before_Stmt_Matcher_203 {
    fn name(&self) -> &str {
        "Insert_Empty_Attribute_Before_Stmt_Matcher_203"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut macro_items = Vec::new();
        
        for item in &file.items {
            if let Item::Macro(item_macro) = item {
                macro_items.push(item_macro.clone());
            }
        }
        
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                let macro_rules = &item_macro.mac;
                if macro_rules.path.is_ident("macro_rules") {
                    let body_tokens = macro_rules.tokens.clone();
                    let mut has_stmt_matcher = false;
                    let mut stmt_matcher_count = &mut 0;
                    
                    for token in body_tokens.clone().into_iter() {
                        if let proc_macro2::TokenTree::Ident(ident) = &token {
                            if ident.to_string() == "stmt" {
                                has_stmt_matcher = true;
                                *stmt_matcher_count += 1;
                            }
                        }
                    }
                    
                    if has_stmt_matcher && *stmt_matcher_count >= 1 {
                        let mut new_tokens = proc_macro2::TokenStream::new();
                        let mut in_expansion = false;
                        let mut stmt_inserted = false;
                        let mut delimiter_count = 0;
                        
                        for token in body_tokens.clone().into_iter() {
                            match &token {
                                proc_macro2::TokenTree::Punct(punct) if punct.as_char() == '=' && !in_expansion => {
                                    new_tokens.extend(quote! { #punct });
                                    in_expansion = true;
                                }
                                proc_macro2::TokenTree::Group(group) if in_expansion && !stmt_inserted => {
                                    let mut inner_stream = group.stream();
                                    let mut new_inner = proc_macro2::TokenStream::new();
                                    let mut found_stmt = false;
                                    
                                    let inner_tokens: Vec<_> = inner_stream.clone().into_iter().collect();
                                    for (i, inner_token) in inner_tokens.iter().enumerate() {
                                        if let proc_macro2::TokenTree::Ident(ident) = inner_token {
                                            if ident.to_string().starts_with('$') {
                                                let lookahead: Vec<_> = inner_tokens.iter().skip(i).collect();
                                                if lookahead.len() >= 2 {
                                                    if let proc_macro2::TokenTree::Punct(colon) = &lookahead[1] {
                                                        if colon.as_char() == ':' {
                                                            if let Some(proc_macro2::TokenTree::Ident(type_ident)) = lookahead.get(2) {
                                                                if type_ident.to_string() == "stmt" && !found_stmt {
                                                                    new_inner.extend(quote! { #[cfg()] });
                                                                    found_stmt = true;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        new_inner.extend(quote! { #inner_token });
                                    }
                                    
                                    let new_group = proc_macro2::Group::new(group.delimiter(), new_inner);
                                    new_tokens.extend(quote! { #new_group });
                                    stmt_inserted = true;
                                }
                                _ => {
                                    new_tokens.extend(quote! { #token });
                                }
                            }
                        }
                        
                        item_macro.mac.tokens = new_tokens;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro_rules! definitions that contain statement matchers ($stmt:stmt). It inserts an empty #[cfg()] attribute before the first statement matcher in the macro expansion body. This transformation aims to trigger edge cases in attribute parsing during macro expansion, particularly when attributes are placed on non-tail expressions or statements, which can lead to internal compiler errors like 'not a tail expression'. The mutation specifically affects macros with multiple statement matchers separated by delimiters, creating syntactically valid but semantically unusual code to stress the compiler's attribute handling in macro contexts."
    }
}