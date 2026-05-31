use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Insert_Doc_Comment_Before_Attribute_253;

impl Mutator for Insert_Doc_Comment_Before_Attribute_253 {
    fn name(&self) -> &str {
        "Insert_Doc_Comment_Before_Attribute_253"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(attrs) = &func.attrs {
                    for attr in attrs {
                        if let Attribute { pound_token, style, path, tokens, .. } = attr {
                            if style == AttrStyle::Inner {
                                let doc_comment = Attribute {
                                    pound_token: token::Pound::default(),
                                    style: AttrStyle::Inner,
                                    path: parse_quote!(doc),
                                    tokens: TokenStream::from(quote!(///)),
                                };
                                let new_attrs = attrs.clone();
                                let mut new_attrs_vec: Vec<Attribute> = new_attrs.into_iter().collect();
                                if let Some(index) = new_attrs_vec.iter().position(|a| a == attr) {
                                    new_attrs_vec.insert(index, doc_comment);
                                }
                                func.attrs = Some(new_attrs_vec);
                            }
                        }
                    }
                }
            }
            if let syn::Item::Struct(struct_item) = item {
                if let Some(attrs) = &struct_item.attrs {
                    for attr in attrs {
                        if let Attribute { pound_token, style, path, tokens, .. } = attr {
                            if style == AttrStyle::Inner {
                                let doc_comment = Attribute {
                                    pound_token: token::Pound::default(),
                                    style: AttrStyle::Inner,
                                    path: parse_quote!(doc),
                                    tokens: TokenStream::from(quote!(///)),
                                };
                                let new_attrs = attrs.clone();
                                let mut new_attrs_vec: Vec<Attribute> = new_attrs.into_iter().collect();
                                if let Some(index) = new_attrs_vec.iter().position(|a| a == attr) {
                                    new_attrs_vec.insert(index, doc_comment);
                                }
                                struct_item.attrs = Some(new_attrs_vec);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a doc comment before each attribute in the code. This transformation tests the compiler's handling of doc comments before attributes and may trigger bugs in proc macro expansion, attribute parsing, and overall compiler stability."
    }
}