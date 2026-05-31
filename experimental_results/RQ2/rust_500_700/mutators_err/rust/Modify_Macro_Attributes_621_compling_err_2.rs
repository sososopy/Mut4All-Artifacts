use proc_macro2::{Span, TokenStream};
use quote::ToTokens;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, Attribute, Expr, ExprCall, ExprPath,
    File, FnArg, GenericArgument, GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime,
    LifetimeParam, Local, Meta, MetaList, MetaNameValue, NestedMeta, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, visit::Visit, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Modify_Macro_Attributes_621;

impl Mutator for Modify_Macro_Attributes_621 {
    fn name(&self) -> &str {
        "Modify_Macro_Attributes_621"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut rng = thread_rng();
                for attr in &mut item_struct.attrs {
                    if attr.path().is_ident("serde") {
                        if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                            let mut new_meta_list = meta_list.clone();
                            for nested_meta in &mut new_meta_list.nested {
                                if let NestedMeta::Meta(Meta::NameValue(meta_name_value)) = nested_meta {
                                    if meta_name_value.path.is_ident("bound") {
                                        if let syn::Lit::Str(lit_str) = &meta_name_value.lit {
                                            if lit_str.value() == "deserialize = \"\"" {
                                                meta_name_value.lit = syn::Lit::Str(syn::LitStr::new("deserialize = \"T\"", lit_str.span()));
                                            }
                                        }
                                    }
                                }
                            }
                            *attr = syn::Attribute {
                                pound_token: attr.pound_token,
                                style: attr.style,
                                bracket_token: attr.bracket_token,
                                meta: Meta::List(new_meta_list),
                            };
                        }
                    }
                    if attr.path().is_ident("derive") {
                        if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                            let mut new_meta_list = meta_list.clone();
                            if !new_meta_list.nested.iter().any(|nested| {
                                if let NestedMeta::Meta(Meta::Path(path)) = nested {
                                    path.is_ident("Clone")
                                } else {
                                    false
                                }
                            }) {
                                new_meta_list.nested.push(NestedMeta::Meta(Meta::Path(SynPath::from(Ident::new("Clone", Span::call_site())))));
                            }
                            *attr = syn::Attribute {
                                pound_token: attr.pound_token,
                                style: attr.style,
                                bracket_token: attr.bracket_token,
                                meta: Meta::List(new_meta_list),
                            };
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