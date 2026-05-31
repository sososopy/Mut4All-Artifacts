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

pub struct Rename_Macro_Identifiers_And_Ensure_Literal_Formatting_204;

impl Mutator for Rename_Macro_Identifiers_And_Ensure_Literal_Formatting_204 {
    fn name(&self) -> &str {
        "Rename_Macro_Identifiers_And_Ensure_Literal_Formatting_204"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                let macro_name = mac.mac.path.segments.last().unwrap().ident.to_string();
                if macro_name == "macro_rules" {
                    let mut tokens = mac.mac.tokens.clone().into_iter().collect::<Vec<_>>();
                    let mut new_tokens = Vec::new();
                    let mut i = 0;
                    while i < tokens.len() {
                        if let proc_macro2::TokenTree::Punct(punct) = &tokens[i] {
                            if punct.as_char() == '$' {
                                if i + 1 < tokens.len() {
                                    if let proc_macro2::TokenTree::Ident(ident) = &tokens[i + 1] {
                                        let ident_str = ident.to_string();
                                        if ident_str == "crate"
                                            || ident_str == "ty"
                                            || ident_str == "fmt"
                                        {
                                            new_tokens.push(tokens[i].clone());
                                            new_tokens.push(tokens[i + 1].clone());
                                            i += 2;
                                            continue;
                                        } else if ident_str.starts_with("TyCtxt") {
                                            new_tokens.push(tokens[i].clone());
                                            new_tokens.push(proc_macro2::TokenTree::Ident(
                                                proc_macro2::Ident::new(
                                                    "TyCtxt",
                                                    ident.span(),
                                                ),
                                            ));
                                            i += 2;
                                            continue;
                                        }
                                    }
                                }
                            }
                        }
                        if let proc_macro2::TokenTree::Literal(lit) = &tokens[i] {
                            let lit_str = lit.to_string();
                            if lit_str.starts_with("TyCtxt") {
                                new_tokens.push(proc_macro2::TokenTree::Literal(
                                    proc_macro2::Literal::string("TyCtxt"),
                                ));
                                i += 1;
                                continue;
                            }
                        }
                        new_tokens.push(tokens[i].clone());
                        i += 1;
                    }
                    mac.mac.tokens = proc_macro2::TokenStream::from_iter(new_tokens);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets macros, specifically those defined with macro_rules! syntax. It identifies identifiers like $crate, $ty, and $fmt, ensuring they are correctly formatted, especially in type expressions like $crate::ty::TyCtxt. It renames any malformed identifiers and ensures literals that should be strings are quoted, preventing ICEs related to macro expansion and identifier resolution."
    }
}