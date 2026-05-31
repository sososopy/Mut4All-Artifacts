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

pub struct Add_Unused_Generic_Parameters_355;

impl Mutator for Add_Unused_Generic_Parameters_355 {
    fn name(&self) -> &str {
        "Add_Unused_Generic_Parameters_355"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let generics = &mut func.sig.generics;
                    for i in 0..5 {
                        let ident = Ident::new(&format!("T{}", i), Span::call_site());
                        generics.params.push(GenericParam::Type(parse_quote!(#ident)));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident != "main" {
                            let generics = &mut func.sig.generics;
                            for i in 0..5 {
                                let ident = Ident::new(&format!("T{}", i), Span::call_site());
                                generics.params.push(GenericParam::Type(parse_quote!(#ident)));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds multiple unused generic type parameters (T0, T1, T2, T3, T4) to non-main function and impl signatures. This stresses the compiler's generic handling and type resolution systems by introducing potentially redundant and unreferenced generic parameters. The goal is to explore compiler behavior in the presence of excessive generic parameters, potentially triggering ICEs or other deep compiler bugs."
    }
}