use proc_macro2::{Span, TokenStream, TokenTree};
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

pub struct Convert_Inner_To_Outer_Attributes_509;

impl Mutator for Convert_Inner_To_Outer_Attributes_509 {
    fn name(&self) -> &str {
        "Convert_Inner_To_Outer_Attributes_509"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(item_macro) = item {
                let mut new_tokens = TokenStream::new();
                let mut inner_attr_found = false;
                for token in item_macro.mac.tokens.clone() {
                    if let TokenTree::Group(group) = &token {
                        if group.delimiter() == proc_macro2::Delimiter::Brace {
                            let mut inner_tokens = TokenStream::new();
                            for inner_token in group.stream() {
                                if let TokenTree::Punct(punct) = &inner_token {
                                    if punct.as_char() == '!' {
                                        inner_attr_found = true;
                                        continue;
                                    }
                                }
                                if inner_attr_found {
                                    if let TokenTree::Ident(ident) = &inner_token {
                                        new_tokens.extend(quote! { #[#ident] });
                                        inner_attr_found = false;
                                        continue;
                                    }
                                }
                                inner_tokens.extend(quote! { #inner_token });
                            }
                            new_tokens.extend(quote! { { #inner_tokens } });
                        } else {
                            new_tokens.extend(quote! { #token });
                        }
                    } else {
                        new_tokens.extend(quote! { #token });
                    }
                }
                item_macro.mac.tokens = new_tokens;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}