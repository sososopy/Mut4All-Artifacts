use proc_macro2::{Span, TokenStream, TokenTree};
use quote::ToTokens;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, Attribute, Expr, ExprCall, ExprPath,
    File, FnArg, GenericArgument, GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime,
    LifetimeParam, Local, Meta, MetaList, MetaNameValue, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, visit::Visit, visit_mut::VisitMut, NestedMeta, LitStr, Lit,
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
                        if let Meta::List(meta_list) = attr.meta.clone() {
                            let mut new_meta_list = meta_list.clone();
                            let mut nested_metas = Punctuated::new();
                            for nested_meta in meta_list.nested {
                                if let NestedMeta::Meta(Meta::NameValue(meta_name_value)) = &nested_meta {
                                    if meta_name_value.path.is_ident("bound") {
                                        if let Lit::Str(lit_str) = &meta_name_value.lit {
                                            if lit_str.value() == "deserialize = \"\"" {
                                                nested_metas.push(NestedMeta::Meta(Meta::NameValue(MetaNameValue {
                                                    path: meta_name_value.path.clone(),
                                                    eq_token: meta_name_value.eq_token,
                                                    lit: Lit::Str(LitStr::new("deserialize = \"T\"", lit_str.span())),
                                                })));
                                            }
                                        }
                                    }
                                }
                                nested_metas.push(nested_meta);
                            }
                            new_meta_list.nested = nested_metas;
                            attr.meta = Meta::List(new_meta_list);
                        }
                    }
                    if attr.path().is_ident("derive") {
                        if let Meta::List(meta_list) = attr.meta.clone() {
                            let mut new_meta_list = meta_list.clone();
                            let mut nested_metas = Punctuated::new();
                            let mut has_clone = false;
                            for nested_meta in meta_list.nested {
                                if let NestedMeta::Meta(Meta::Path(path)) = &nested_meta {
                                    if path.is_ident("Clone") {
                                        has_clone = true;
                                    }
                                }
                                nested_metas.push(nested_meta);
                            }
                            if !has_clone {
                                nested_metas.push(NestedMeta::Meta(Meta::Path(SynPath::from(Ident::new("Clone", Span::call_site())))));
                            }
                            new_meta_list.nested = nested_metas;
                            attr.meta = Meta::List(new_meta_list);
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