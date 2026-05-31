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

pub struct Add_Async_Method_With_Await_302;

impl Mutator for Add_Async_Method_With_Await_302 {
    fn name(&self) -> &str {
        "Add_Async_Method_With_Await_302"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut current_trait_info: Option<(Ident, Ident, Ident, Box<Type>)> = None;

        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let existing_methods: Vec<_> = trait_item.items.iter_mut()
                    .filter_map(|trait_item_item| {
                        if let syn::TraitItem::Fn(func) = trait_item_item {
                            if func.sig.asyncness.is_some() {
                                Some(func)
                            } else {
                                None
                            }
                        } else {
                            None
                        }
                    })
                    .collect();

                if !existing_methods.is_empty() {
                    let existing_method = &existing_methods[0];
                    let existing_method_name = existing_method.sig.ident.clone();
                    let return_type = match &existing_method.sig.output {
                        ReturnType::Type(_, ty) => ty.clone(),
                        _ => parse_quote! { () },
                    };

                    let new_method_name = Ident::new("bar", Span::call_site());

                    let new_trait_method = parse_quote! {
                        async fn #new_method_name(&self) -> #return_type;
                    };

                    trait_item.items.push(syn::TraitItem::Fn(new_trait_method));

                    current_trait_info = Some((
                        trait_item.ident.clone(),
                        new_method_name,
                        existing_method_name,
                        return_type,
                    ));
                }
            } else if let syn::Item::Impl(impl_item) = item {
                if let Some((trait_name, new_method_name, existing_method_name, return_type)) = &current_trait_info {
                    if let Some((_, trait_path, _)) = &impl_item.trait_ {
                        if let Some(last_segment) = trait_path.segments.last() {
                            if last_segment.ident == *trait_name {
                                let new_impl_method = parse_quote! {
                                    async fn #new_method_name(&self) -> #return_type {
                                        self.#existing_method_name().await
                                    }
                                };
                                impl_item.items.push(syn::ImplItem::Fn(new_impl_method));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an async method in a trait that awaits another async method from the same trait. This transformation tests the compiler's handling of async method resolution within traits, particularly with the `async_fn_in_trait` feature enabled. By creating a call chain between async methods, it increases the likelihood of exposing bugs in async expansion and trait method resolution."
    }
}