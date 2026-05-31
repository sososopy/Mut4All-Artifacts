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

pub struct Add_Unbounded_Lifetimes_453;

impl Mutator for Add_Unbounded_Lifetimes_453 {
    fn name(&self) -> &str {
        "Add_Unbounded_Lifetimes_453"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime = Lifetime::new("'a", Span::call_site());
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        let ty = &mut *pat_type.ty;
                        *ty = Type::Reference(syn::TypeReference {
                            and_token: token::And::default(),
                            lifetime: Some(lifetime.clone()),
                            mutability: None,
                            elem: Box::new(ty.clone()),
                        });
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetime = Lifetime::new("'b", Span::call_site());
                        let lifetime_param = LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: lifetime.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        };
                        func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                let ty = &mut *pat_type.ty;
                                *ty = Type::Reference(syn::TypeReference {
                                    and_token: token::And::default(),
                                    lifetime: Some(lifetime.clone()),
                                    mutability: None,
                                    elem: Box::new(ty.clone()),
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unbounded lifetimes to function and method signatures, transforming parameter types into references with explicit lifetimes. This aims to stress the compiler's lifetime resolution system, potentially leading to ICEs or failures in handling complex lifetime scenarios and borrow checking."
    }
}