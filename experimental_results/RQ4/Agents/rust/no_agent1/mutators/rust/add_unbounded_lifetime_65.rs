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

pub struct Add_Unbounded_Lifetime_65;

impl Mutator for Add_Unbounded_Lifetime_65 {
    fn name(&self) -> &str {
        "Add_Unbounded_Lifetime_65"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime_param = LifetimeParam {
                    attrs: vec![],
                    lifetime: Lifetime::new("'a", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));
                func.sig.inputs.iter_mut().for_each(|arg| {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Reference(type_ref) = &mut *pat_type.ty {
                            type_ref.lifetime = Some(lifetime_param.lifetime.clone());
                        }
                    }
                });
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetime_param = LifetimeParam {
                            attrs: vec![],
                            lifetime: Lifetime::new("'a", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        };
                        func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));
                        func.sig.inputs.iter_mut().for_each(|arg| {
                            if let FnArg::Typed(pat_type) = arg {
                                if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                    type_ref.lifetime = Some(lifetime_param.lifetime.clone());
                                }
                            }
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unbounded lifetime `'a` to function and method signatures, and applies it to reference types in parameters. This transformation introduces potential lifetime mismatches and conflicts, aiming to stress-test the compiler's borrow checker and lifetime resolution mechanisms, potentially leading to ICEs or unexpected behavior in complex lifetime scenarios."
    }
}