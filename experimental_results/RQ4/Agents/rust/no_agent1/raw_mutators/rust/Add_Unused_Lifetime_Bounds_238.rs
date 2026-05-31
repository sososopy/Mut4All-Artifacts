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

pub struct Add_Unused_Lifetime_Bounds_238;

impl Mutator for Add_Unused_Lifetime_Bounds_238 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Bounds_238"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    let lifetime_param = LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: Lifetime::new("'a", Span::call_site()),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    };
                    generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));
                    generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        if func.sig.ident != "main" {
                            let lifetime_param = LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: Lifetime::new("'a", Span::call_site()),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            };
                            generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));
                            generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unused lifetime parameters to non-main function and impl signatures. By introducing redundant lifetimes, it aims to stress the compiler's lifetime resolution and checking mechanisms, potentially triggering ICEs or assertion failures due to unexpected interactions with lifetime elision or inference."
    }
}