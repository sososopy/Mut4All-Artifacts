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

pub struct Alter_Lifetime_Constraints_335;

impl Mutator for Alter_Lifetime_Constraints_335 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Constraints_335"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new("'a", Span::call_site()),
                    colon_token: None,
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(Lifetime::new("'static", Span::call_site()));
                        bounds
                    },
                    eq_token: None,
                    default: None,
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetime_param = LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: Lifetime::new("'a", Span::call_site()),
                            colon_token: None,
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(Lifetime::new("'static", Span::call_site()));
                                bounds
                            },
                            eq_token: None,
                            default: None,
                        };
                        func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a new lifetime parameter 'a with a constraint to 'static for non-main functions and methods. This transformation introduces potentially conflicting lifetime constraints, aiming to test the compiler's ability to handle complex lifetime resolution scenarios and uncover bugs related to lifetime inference and constraint satisfaction."
    }
}