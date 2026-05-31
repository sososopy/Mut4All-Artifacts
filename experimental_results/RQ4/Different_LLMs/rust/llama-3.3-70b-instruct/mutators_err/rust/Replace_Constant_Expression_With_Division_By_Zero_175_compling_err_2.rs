use proc_macro2::{Span, TokenTree};
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

pub struct Replace_Constant_Expression_With_Division_By_Zero_175;

impl Mutator for Replace_Constant_Expression_With_Division_By_Zero_175 {
    fn name(&self) -> &str {
        "Replace_Constant_Expression_With_Division_By_Zero_175"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(mac) = item {
                if let syn::Macro::Macro(mac) = mac {
                    if let syn::MacroAst::TokenStream(stream) = &mac.body {
                        let mut new_stream = TokenStream::new();
                        for token in stream {
                            if let TokenTree::Group(group) = token {
                                let mut new_group = group.clone();
                                new_group.stream = replace_constant_expressions(new_group.stream());
                                new_stream.extend(vec![TokenTree::Group(new_group)]);
                            } else {
                                new_stream.extend(vec![token]);
                            }
                        }
                        mac.body = syn::MacroAst::TokenStream(new_stream);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant expressions within the global_asm macro and replaces them with a division by zero expression, which can trigger the internal compiler error."
    }
}

fn replace_constant_expressions(stream: TokenStream) -> TokenStream {
    let mut new_stream = TokenStream::new();
    for token in stream {
        if let TokenTree::Ident(_) = token {
            let new_expr = parse_quote!(1 / 0);
            new_stream.extend(vec![TokenTree::Ident(new_expr.to_token_stream().into_iter().next().unwrap())]);
        } else if let TokenTree::Literal(lit) = token {
            let new_expr = parse_quote!(1 / 0);
            new_stream.extend(vec![TokenTree::Literal(new_expr.to_token_stream().into_iter().next().unwrap())]);
        } else {
            new_stream.extend(vec![token]);
        }
    }
    new_stream
}