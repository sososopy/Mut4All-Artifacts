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

pub struct Delete_Attribute_Spans_29;

impl Mutator for Delete_Attribute_Spans_29 {
    fn name(&self) -> &str {
        "Delete_Attribute_Spans_29"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = AttributeSpanDeleter::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets attribute spans by removing them from parsed attributes. It applies to any attribute applied to items (structs, enums, functions, etc.) where the attribute has an inner parsed representation, such as repr, derive, or cfg. The operator works by modifying the internal representation of the attribute to delete its span information, simulating a scenario where the span is missing or corrupted. This is likely to trigger bugs in compiler passes that assume attribute spans are always available, particularly in lints or analyses that inspect attribute details."
    }
}

struct AttributeSpanDeleter;

impl AttributeSpanDeleter {
    fn new() -> Self {
        Self {}
    }
}

impl VisitMut for AttributeSpanDeleter {
    fn visit_attribute_mut(&mut self, attr: &mut Attribute) {
        if let Meta::List(list) = &attr.meta {
            if let Some(path) = list.path.get_ident() {
                let ident_str = path.to_string();
                if ident_str == "repr" || ident_str == "derive" || ident_str == "cfg" {
                    let dummy_span = proc_macro2::Span::call_site();
                    let mut new_tokens = proc_macro2::TokenStream::new();
                    for token in list.tokens.clone() {
                        let mut token = token;
                        if let proc_macro2::TokenTree::Group(group) = &mut token {
                            let mut inner_stream = group.stream();
                            let mut new_inner = proc_macro2::TokenStream::new();
                            for inner_token in inner_stream {
                                let mut inner_token = inner_token;
                                inner_token.set_span(dummy_span);
                                new_inner.extend(std::iter::once(inner_token));
                            }
                            let mut new_group = proc_macro2::Group::new(group.delimiter(), new_inner);
                            new_group.set_span(dummy_span);
                            new_tokens.extend(std::iter::once(proc_macro2::TokenTree::Group(new_group)));
                        } else {
                            token.set_span(dummy_span);
                            new_tokens.extend(std::iter::once(token));
                        }
                    }
                    attr.meta = Meta::List(MetaList {
                        path: list.path.clone(),
                        delimiter: list.delimiter.clone(),
                        tokens: new_tokens,
                    });
                }
            }
        }
        syn::visit_mut::visit_attribute_mut(self, attr);
    }
}