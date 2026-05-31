use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, visit::Visit, visit_mut::VisitMut, *,
    Attribute, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident, Item,
    ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaList, MetaNameValue, Pat, PatType,
    Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath,
};

use crate::mutator::Mutator;

pub struct Modify_Union_Field_Alignment_16;

impl Mutator for Modify_Union_Field_Alignment_16 {
    fn name(&self) -> &str {
        "Modify_Union_Field_Alignment_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(union) = item {
                if let Some(attr) = union.attrs.iter_mut().find(|attr| attr.path().is_ident("repr")) {
                    if let Meta::List(meta_list) = attr.meta.clone() {
                        let mut new_nested = Punctuated::new();
                        let nested_iter = meta_list.tokens.clone().into_iter();
                        for nested_meta in nested_iter {
                            if let Ok(Meta::NameValue(name_value)) = syn::parse2::<Meta>(nested_meta.into()) {
                                if name_value.path.is_ident("packed") {
                                    if let Expr::Lit(expr_lit) = &name_value.value {
                                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                            if let Ok(value) = lit_int.base10_parse::<u8>() {
                                                let new_value = if value == 1 { 2 } else { 1 };
                                                let new_lit = syn::Lit::Int(syn::LitInt::new(&new_value.to_string(), lit_int.span()));
                                                let new_expr = Expr::Lit(ExprLit { attrs: vec![], lit: new_lit });
                                                new_nested.push(Meta::NameValue(MetaNameValue {
                                                    path: name_value.path.clone(),
                                                    eq_token: name_value.eq_token,
                                                    value: new_expr,
                                                }));
                                            }
                                        }
                                    }
                                } else {
                                    new_nested.push(Meta::NameValue(name_value));
                                }
                            }
                        }
                        attr.meta = Meta::List(MetaList {
                            path: attr.path().clone(),
                            delimiter: syn::MacroDelimiter::Paren(Default::default()),
                            tokens: new_nested.into_token_stream(),
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets unions with the #[repr(packed)] attribute, specifically modifying the packed alignment value. By changing the alignment from 1 to 2 (or vice versa), it aims to reveal potential misalignments and inconsistencies in the union's field layout as processed by the Rust compiler. This can expose latent bugs related to layout computation and alignment handling."
    }
}