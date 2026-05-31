use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaList, MetaNameValue, NestedMeta, Pat, PatType, Path as SynPath,
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
                for attr in &mut func.attrs {
                    if attr.path.is_ident("cfg") {
                        if let Ok(Meta::List(mut meta_list)) = attr.parse_meta() {
                            for nested_meta in &mut meta_list.nested {
                                if let NestedMeta::Meta(Meta::NameValue(meta_nv)) = nested_meta {
                                    if meta_nv.path.is_ident("target_os") {
                                        meta_nv.path = parse_quote!(target);
                                        meta_nv.value = syn::Lit::Str(syn::LitStr::new("128::MAX, true", Span::call_site()));
                                    }
                                }
                            }
                            attr.tokens = quote! { (#meta_list) };
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