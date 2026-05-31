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

pub struct Add_Lifetime_Constraints_74;

impl Mutator for Add_Lifetime_Constraints_74 {
    fn name(&self) -> &str {
        "Add_Lifetime_Constraints_74"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                let lifetime_param = LifetimeParam {
                    lifetime: Lifetime::new("'a", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                    eq_token: None,
                    default: None,
                };
                generics.params.push(GenericParam::Lifetime(lifetime_param));

                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::Reference(type_ref) = &mut **return_type {
                        type_ref.lifetime = Some(Lifetime::new("'a", Span::call_site()));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        let lifetime_param = LifetimeParam {
                            lifetime: Lifetime::new("'b", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                            eq_token: None,
                            default: None,
                        };
                        generics.params.push(GenericParam::Lifetime(lifetime_param));

                        if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                            if let Type::Reference(type_ref) = &mut **return_type {
                                type_ref.lifetime = Some(Lifetime::new("'b", Span::call_site()));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds lifetime parameters to function and method signatures, and applies these lifetimes to reference return types. This transformation introduces lifetime constraints that may lead to complex borrow checker interactions or lifetime resolution issues, potentially triggering ICEs or other compiler bugs related to lifetime inference and elision."
    }
}