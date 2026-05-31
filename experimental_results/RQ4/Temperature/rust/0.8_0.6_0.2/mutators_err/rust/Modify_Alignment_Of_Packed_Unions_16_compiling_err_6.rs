use proc_macro2::{Span, TokenStream, TokenTree};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Lit, Meta, MetaList, MetaNameValue,
    Pat, PatType, Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier,
    Type, TypeImplTrait, TypeParamBound, TypePath, parse_quote,
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

pub struct Modify_Alignment_Of_Packed_Unions_16;

impl Mutator for Modify_Alignment_Of_Packed_Unions_16 {
    fn name(&self) -> &str {
        "Modify_Alignment_Of_Packed_Unions_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(union) = item {
                let mut is_packed = false;
                let mut new_attrs = Vec::new();
                
                for attr in &union.attrs {
                    if attr.path().is_ident("repr") {
                        if let Meta::List(meta_list) = attr.parse_meta().unwrap() {
                            let mut tokens = meta_list.tokens.into_iter().collect::<Vec<_>>();
                            for nested_meta in &mut tokens {
                                if let TokenTree::Ident(ident) = nested_meta {
                                    if ident == "packed" {
                                        is_packed = true;
                                    }
                                }
                                if let TokenTree::Group(group) = nested_meta {
                                    let mut stream = group.stream().into_iter();
                                    if let Some(TokenTree::Literal(lit)) = stream.next() {
                                        if let Ok(old_alignment) = lit.to_string().parse::<u64>() {
                                            let new_alignment = if old_alignment == 1 { 2 } else { old_alignment + 1 };
                                            *nested_meta = TokenTree::Group(proc_macro2::Group::new(
                                                proc_macro2::Delimiter::Parenthesis,
                                                quote! { #new_alignment },
                                            ));
                                        }
                                    }
                                }
                            }
                            new_attrs.push(Attribute {
                                pound_token: attr.pound_token,
                                style: attr.style,
                                bracket_token: attr.bracket_token,
                                meta: Meta::List(MetaList {
                                    path: attr.path().clone(),
                                    delimiter: syn::MacroDelimiter::Paren(Default::default()),
                                    tokens: quote! { (#(#tokens),*) },
                                }),
                            });
                        }
                    } else {
                        new_attrs.push(attr.clone());
                    }
                }

                if is_packed {
                    union.attrs = new_attrs;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets unions with the `#[repr(packed)]` attribute, modifying their alignment values. It incrementally increases the alignment to test for ABI and layout alignment mismatches, aiming to expose potential bugs in the handling of packed unions during compilation."
    }
}