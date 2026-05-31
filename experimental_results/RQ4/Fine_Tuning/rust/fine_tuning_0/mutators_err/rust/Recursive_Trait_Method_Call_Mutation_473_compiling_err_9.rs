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

pub struct Recursive_Trait_Method_Call_Mutation_473;

impl Mutator for Recursive_Trait_Method_Call_Mutation_473 {
    fn name(&self) -> &str {
        "Recursive_Trait_Method_Call_Mutation_473"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_items = Vec::new();
        let mut impl_items = Vec::new();

        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                trait_items.push(trait_item);
            }
        }

        for trait_item in &trait_items {
            if trait_item.items.iter().any(|item| matches!(item, syn::TraitItem::Fn(method) if method.sig.asyncness.is_some())) {
                let trait_name = trait_item.ident.clone();
                impl_items.clear();

                for item in &mut file.items {
                    if let syn::Item::Impl(item_impl) = item {
                        if let Some((_, path, _)) = &item_impl.trait_ {
                            if path.segments.last().unwrap().ident == trait_name {
                                impl_items.push(item_impl);
                            }
                        }
                    }
                }

                for item_impl in &mut impl_items {
                    for impl_item in &mut item_impl.items {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            if method.sig.asyncness.is_some() {
                                let call_expr: Expr = parse_quote! {
                                    self.other_fn().await
                                };
                                method.block.stmts.push(Stmt::Expr(call_expr, None));
                            }
                        }
                    }
                }
            }
        }

        trait_items.clear();
        impl_items.clear();

        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                trait_items.push(trait_item);
            }
        }

        for trait_item in &trait_items {
            let trait_name = &trait_item.ident;
            impl_items.clear();

            for item in &mut file.items {
                if let syn::Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if path.segments.last().unwrap().ident == *trait_name {
                            impl_items.push(item_impl);
                        }
                    }
                }
            }

            for item_impl in &mut impl_items {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.asyncness.is_some() {
                            let call_expr: Expr = parse_quote! {
                                self.async_fn().await
                            };
                            method.block.stmts.push(Stmt::Expr(call_expr, None));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies traits with async methods and introduces an indirect recursive call chain within their implementations. This involves creating a cycle between async methods across multiple traits, ensuring that each method calls another that eventually leads back to the original, with an await point in between. This tests the compiler's handling of async recursion and trait interaction."
    }
}