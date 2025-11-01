use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaNameValue, Pat, PatType, Path as SynPath,
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

pub struct Replace_Doc_Comment_With_Normal_Comment_476;

impl Mutator for Replace_Doc_Comment_With_Normal_Comment_476 {
    fn name(&self) -> &str {
        "Replace_Doc_Comment_With_Normal_Comment_476"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some((_, doc_comments)) = extract_doc_comments(&func.attrs) {
                    func.attrs.retain(|attr| !is_doc_comment(attr));
                    for doc_comment in doc_comments {
                        func.attrs.push(syn::Attribute {
                            pound_token: token::Pound { spans: [Span::call_site()] },
                            style: syn::AttrStyle::Outer,
                            bracket_token: token::Bracket { span: DelimSpan::from_single(Span::call_site()) },
                            meta: syn::Meta::NameValue(MetaNameValue {
                                path: syn::Path::from(Ident::new("doc", Span::call_site())),
                                eq_token: token::Eq { spans: [Span::call_site()] },
                                value: syn::Lit::Str(syn::LitStr::new(&doc_comment, Span::call_site())),
                            }),
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator locates function definitions with associated doc comments and replaces these doc comments with regular comments. This transformation is designed to explore how the Rust compiler and procedural macros handle the parsing and processing of documentation comments versus regular comments, especially in macro contexts. By changing the comment format, it may trigger different parsing behavior or errors, providing insights into the compiler's comment handling mechanisms."
    }
}

fn extract_doc_comments(attrs: &[syn::Attribute]) -> Option<(usize, Vec<String>)> {
    let mut doc_comments = Vec::new();
    let mut index = None;
    for (i, attr) in attrs.iter().enumerate() {
        if is_doc_comment(attr) {
            if let Ok(Meta::NameValue(nv)) = attr.meta() {
                if let syn::Lit::Str(lit_str) = nv.value {
                    doc_comments.push(lit_str.value());
                    if index.is_none() {
                        index = Some(i);
                    }
                }
            }
        }
    }
    index.map(|i| (i, doc_comments))
}

fn is_doc_comment(attr: &syn::Attribute) -> bool {
    attr.path().is_ident("doc")
}