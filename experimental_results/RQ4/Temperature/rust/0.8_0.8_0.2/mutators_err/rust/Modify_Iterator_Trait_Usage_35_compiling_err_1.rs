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

pub struct Modify_Iterator_Trait_Usage_35;

impl Mutator for Modify_Iterator_Trait_Usage_35 {
    fn name(&self) -> &str {
        "Modify_Iterator_Trait_Usage_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                            if let syn::Type::ImplTrait(type_impl_trait) = &**return_type {
                                for stmt in &mut func.block.stmts {
                                    if let syn::Stmt::Expr(expr) = stmt {
                                        if let syn::Expr::MethodCall(method_call) = expr {
                                            let method_name = method_call.method.to_string();
                                            let replacement_method = match method_name.as_str() {
                                                "iter_mut" => "iter",
                                                "iter" => "into_iter",
                                                "into_iter" => "iter_mut",
                                                _ => continue,
                                            };
                                            method_call.method = syn::Ident::new(
                                                replacement_method,
                                                method_call.method.span(),
                                            );
                                        }
                                    }
                                }
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