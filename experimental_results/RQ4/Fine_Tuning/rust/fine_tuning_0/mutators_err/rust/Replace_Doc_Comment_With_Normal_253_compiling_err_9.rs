use proc_macro2::{Span, TokenStream};
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

pub struct Replace_Doc_Comment_With_Normal_253;

impl Mutator for Replace_Doc_Comment_With_Normal_253 {
    fn name(&self) -> &str {
        "Replace_Doc_Comment_With_Normal_253"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some((doc_comment, index)) = func.attrs.iter().enumerate().find_map(|(i, attr)| {
                    if attr.path().is_ident("doc") {
                        Some((attr, i))
                    } else {
                        None
                    }
                }) {
                    if let Ok(syn::Meta::NameValue(meta)) = doc_comment.parse_meta() {
                        if let syn::Expr::Lit(syn::ExprLit { lit: syn::Lit::Str(lit_str), .. }) = meta.value {
                            let comment = format!("// {}", lit_str.value());
                            func.attrs.remove(index);
                            func.attrs.insert(0, syn::Attribute {
                                pound_token: token::Pound { spans: [Span::call_site()] },
                                style: syn::AttrStyle::Outer,
                                bracket_token: token::Bracket { span: Span::call_site() },
                                meta: syn::Meta::Path(syn::Path::from(Ident::new("comment", Span::call_site()))),
                            });
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