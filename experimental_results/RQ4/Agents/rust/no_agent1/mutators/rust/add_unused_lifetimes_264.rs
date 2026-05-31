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

pub struct Add_Unused_Lifetimes_264;

impl Mutator for Add_Unused_Lifetimes_264 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetimes_264"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: vec![],
                    lifetime: Lifetime::new("'unused1", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
                generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: vec![],
                    lifetime: Lifetime::new("'unused2", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: vec![],
                            lifetime: Lifetime::new("'unused1", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                        generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: vec![],
                            lifetime: Lifetime::new("'unused2", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unused lifetime parameters to non-main function and impl signatures. By introducing lifetimes that are not utilized in the function body or signature, it challenges the compiler's lifetime resolution and checking mechanisms. This transformation aims to uncover bugs related to lifetime inference, unused lifetime detection, and potential ICEs in complex lifetime scenarios."
    }
}