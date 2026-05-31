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

pub struct Modify_Macro_Var_Declaration_228;

impl Mutator for Modify_Macro_Var_Declaration_228 {
    fn name(&self) -> &str {
        "Modify_Macro_Var_Declaration_228"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                let macro_body = &mut item_macro.mac.tokens;
                let mut new_tokens = TokenStream::new();
                let mut inside_stmt = false;
                for token in macro_body.clone() {
                    if let TokenTree::Ident(ident) = &token {
                        if ident == "let" || ident == "const" {
                            inside_stmt = true;
                            new_tokens.extend(quote! { var });
                        } else {
                            new_tokens.extend(quote! { #token });
                        }
                    } else {
                        new_tokens.extend(quote! { #token });
                    }
                }
                *macro_body = new_tokens;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets macro invocations containing variable declarations, replacing valid keywords 'let' or 'const' with 'var', which is syntactically valid but semantically incorrect in Rust. This transformation aims to explore the compiler's error handling and resilience to unexpected keywords in macro expansions."
    }
}