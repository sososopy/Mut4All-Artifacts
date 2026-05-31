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

pub struct Convert_Trait_Method_With_Receiver_To_Async_299;

impl Mutator for Convert_Trait_Method_With_Receiver_To_Async_299 {
    fn name(&self) -> &str {
        "Convert_Trait_Method_With_Receiver_To_Async_299"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_async_feature = false;
        for attr in &file.attrs {
            if let Ok(meta) = attr.meta() {
                if let syn::Meta::List(list) = meta {
                    if list.path.is_ident("feature") {
                        let nested: Punctuated<syn::NestedMeta, Comma> = list.parse_args_with(Punctuated::parse_terminated).unwrap();
                        for nested in nested {
                            if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = nested {
                                if path.is_ident("async_fn_in_trait") {
                                    has_async_feature = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        if !has_async_feature {
            let feature_attr = parse_quote! {
                #![feature(async_fn_in_trait)]
            };
            file.attrs.insert(0, feature_attr);
        }

        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = trait_item {
                        if let Some(first_arg) = method.sig.inputs.iter().next() {
                            if let FnArg::Receiver(_) = first_arg {
                                method.sig.inputs.remove(0);
                                method.sig.asyncness = Some(token::Async {
                                    span: Span::call_site(),
                                });
                            }
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