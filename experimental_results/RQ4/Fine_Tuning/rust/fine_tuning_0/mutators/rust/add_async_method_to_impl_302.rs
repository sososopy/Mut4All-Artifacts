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

pub struct Add_Async_Method_To_Impl_302;

impl Mutator for Add_Async_Method_To_Impl_302 {
    fn name(&self) -> &str {
        "Add_Async_Method_To_Impl_302"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let trait_methods: HashSet<_> = item_impl.items.iter().filter_map(|impl_item| {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        Some(method.sig.ident.clone())
                    } else {
                        None
                    }
                }).collect();

                let new_method_name = Ident::new("new_async_method", Span::call_site());
                if !trait_methods.contains(&new_method_name) {
                    let new_method: syn::ImplItemFn = parse_quote! {
                        async fn new_async_method(&self) -> String {
                            let result = self.existing_method().await + self.another_method().await;
                            format!("Async Result: {}", result)
                        }
                    };
                    item_impl.items.push(syn::ImplItem::Fn(new_method));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}