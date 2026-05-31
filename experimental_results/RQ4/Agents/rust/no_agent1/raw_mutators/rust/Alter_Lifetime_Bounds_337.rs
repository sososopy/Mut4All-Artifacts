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

pub struct Alter_Lifetime_Bounds_337;

impl Mutator for Alter_Lifetime_Bounds_337 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Bounds_337"
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
                    colon_token: Some(token::Colon {
                        spans: [Span::call_site()],
                    }),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(Lifetime::new("'static", Span::call_site()));
                        bounds
                    },
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetime_param = LifetimeParam {
                            attrs: vec![],
                            lifetime: Lifetime::new("'b", Span::call_site()),
                            colon_token: Some(token::Colon {
                                spans: [Span::call_site()],
                            }),
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(Lifetime::new("'static", Span::call_site()));
                                bounds
                            },
                        };
                        func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces new lifetime parameters with explicit bounds to non-main functions and methods. By enforcing 'static bounds, it challenges the compiler's lifetime resolution and borrow checking systems, potentially leading to ICEs or inference failures due to the stringent lifetime constraints imposed."
    }
}