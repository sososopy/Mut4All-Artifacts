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

pub struct Modify_Cfg_Attributes_26;

impl Mutator for Modify_Cfg_Attributes_26 {
    fn name(&self) -> &str {
        "Modify_Cfg_Attributes_26"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(attrs) = &mut func.attrs {
                    for attr in attrs.iter_mut() {
                        if attr.path.is_ident("cfg") {
                            if let Ok(mut meta) = attr.parse_meta() {
                                if let syn::Meta::List(ref mut meta_list) = meta {
                                    for nested_meta in &mut meta_list.nested {
                                        if let syn::NestedMeta::Meta(syn::Meta::NameValue(meta_nv)) = nested_meta {
                                            if meta_nv.path.is_ident("target_os") {
                                                meta_nv.path = parse_quote!(target);
                                                meta_nv.lit = syn::Lit::Str(syn::LitStr::new("128::MAX, true", Span::call_site()));
                                            }
                                        }
                                    }
                                }
                                *attr = syn::Attribute {
                                    pound_token: attr.pound_token,
                                    style: attr.style,
                                    bracket_token: attr.bracket_token,
                                    path: attr.path.clone(),
                                    tokens: quote! { (#meta) },
                                };
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies `cfg` attributes by replacing `target_os` with a non-standard target specification using integer limits and boolean values. This transformation aims to test the compiler's handling of unusual and potentially unsupported `cfg` conditions, which could reveal issues in attribute parsing and evaluation."
    }
}