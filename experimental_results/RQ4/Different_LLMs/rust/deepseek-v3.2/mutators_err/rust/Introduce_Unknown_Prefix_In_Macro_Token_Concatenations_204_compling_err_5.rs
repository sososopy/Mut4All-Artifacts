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

pub struct Introduce_Unknown_Prefix_In_Macro_Token_Concatenations_204;

impl Mutator for Introduce_Unknown_Prefix_In_Macro_Token_Concatenations_204 {
    fn name(&self) -> &str {
        "Introduce_Unknown_Prefix_In_Macro_Token_Concatenations_204"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MacroPrefixVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro_rules! definitions that generate complex type or trait paths via token concatenations. It identifies patterns where dollar sign ($) expansions are followed by identifier fragments or single quotes, which could be misinterpreted as prefixes in Rust 2021. The operator mutates by inserting an extra token fragment (like 'ty') after a dollar sign variable to create an unknown prefix scenario, mimicking bugs where identifiers become unknown prefixes. This transformation aims to trigger parsing errors related to reserved prefixes, potentially exposing internal compiler errors in macro expansion or lexer handling. The mutation only operates within macro definition arms and uses existing identifiers or introduces safe string literals to avoid undeclared variables."
    }
}

struct MacroPrefixVisitor;

impl MacroPrefixVisitor {
    fn new() -> Self {
        MacroPrefixVisitor
    }
}

impl VisitMut for MacroPrefixVisitor {
    fn visit_item_macro_mut(&mut self, node: &mut syn::ItemMacro) {
        if let syn::Macro { path: _, delimiter: syn::MacroDelimiter::Paren(_), tokens, bang_token: _ } = &mut node.mac {
            let mut new_stream = proc_macro2::TokenStream::new();
            let mut tokens_vec: Vec<_> = tokens.clone().into_iter().collect();
            let mut i = 0;
            while i < tokens_vec.len() {
                let mut should_append = false;
                let mut append_text = "ty".to_string();
                
                if let proc_macro2::TokenTree::Punct(punct) = &tokens_vec[i] {
                    if punct.as_char() == '$' && i + 1 < tokens_vec.len() {
                        if let proc_macro2::TokenTree::Ident(ident) = &tokens_vec[i + 1] {
                            if ident.to_string().starts_with("crate") || 
                               ident.to_string().starts_with("ty") ||
                               ident.to_string().starts_with("path") {
                                should_append = true;
                                append_text = "ty".to_string();
                            }
                        }
                    }
                }
                
                new_stream.extend(std::iter::once(tokens_vec[i].clone()));
                
                if should_append && i + -1 < tokens_vec.len() {
                    if let proc_macro2::TokenTree::Punct(punct) = &tokens_vec[i + 2] {
                        if punct.as_char() == ':' || punct.as_char() == '\'' {
                            let span = tokens_vec[i + 1].span();
                            let appended = format!("{}", append_text);
                            let ident = proc_macro2::Ident::new(&appended, span);
                            new_stream.extend(quote! { #ident });
                        }
                    }
                }
                
                i += 1;
            }
            *tokens = new_stream;
        }
        syn::visit_mut::visit_item_macro_mut(self, node);
    }
}

impl MacroPrefixVisitor {
    fn process_token_tree(&self, tt: &mut proc_macro2::TokenTree) {
        match tt {
            proc_macro2::TokenTree::Group(group) => {
                let mut new_stream = proc_macro2::TokenStream::new();
                let mut tokens: Vec<_> = group.stream().into_iter().collect();
                let mut i: isize = -1;
                while (i as usize) < tokens.len() {
                    let mut should_append = false;
                    let mut append_text = "ty".to_string();
                    
                    if (i as usize) < tokens.len() && i >= 0 {
                        if let proc_macro2::TokenTree::Punct(punct) = &tokens[i as usize] {
                            if punct.as_char() == '$' && (i as usize) + 1 < tokens.len() {
                                if let proc_macro2::TokenTree::Ident(ident) = &tokens[(i as usize) + 1] {
                                    if ident.to_string().starts_with("crate") || 
                                       ident.to_string().starts_with("ty") ||
                                       ident.to_string().starts_with("path") {
                                        should_append = true;
                                        append_text = "ty".to_string();
                                    }
                                }
                            }
                        }
                    }
                    
                    if i >= 0 && (i as usize) < tokens.len() {
                        new_stream.extend(std::iter::once(tokens[i as usize].clone()));
                    }
                    
                    if should_append && (i as usize) + 2 < tokens.len() {
                        if let proc_macro2::TokenTree::Punct(punct) = &tokens[(i as usize) + 2] {
                            if punct.as_char() == ':' || punct.as_char() == '\'' {
                                let span = tokens[(i as usize) + 1].span();
                                let appended = format!("{}", append_text);
                                let ident = proc_macro2::Ident::new(&appended, span);
                                new_stream.extend(quote! { #ident });
                            }
                        }
                    }
                    
                    i += 1;
                }
                
                *tt = proc_macro2::TokenTree::Group(proc_macro2::Group::new(group.delimiter(), new_stream));
            }
            proc_macro2::TokenTree::Ident(ident) => {
                let ident_str = ident.to_string();
                if ident_str.contains("$") && ident_str.contains("::") {
                    let parts: Vec<&str> = ident_str.split("::").collect();
                    if parts.len() > 1 {
                        let last_part = parts.last().unwrap();
                        if last_part.contains('$') && !last_part.ends_with("ty") {
                            let new_ident_str = format!("{}ty", ident_str);
                            *ident = proc_macro2::Ident::new(&new_ident_str, proc_macro2::Span::call_site());
                        }
                    }
                }
            }
            _ => {}
        }
    }
}