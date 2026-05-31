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

pub struct Add_Unused_Generic_Params_369;

impl Mutator for Add_Unused_Generic_Params_369 {
    fn name(&self) -> &str {
        "Add_Unused_Generic_Params_369"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                generics.params.push(parse_quote!(T: Default));
                generics.params.push(parse_quote!(U: Clone));
                generics.params.push(parse_quote!(V: Copy));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let generics = &mut func.sig.generics;
                        generics.params.push(parse_quote!(T: Default));
                        generics.params.push(parse_quote!(U: Clone));
                        generics.params.push(parse_quote!(V: Copy));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unused generic parameters with trait bounds (Default, Clone, Copy) to non-main function and impl signatures. These parameters are not utilized within the function body, potentially causing the compiler to handle unnecessary complexity in type resolution and generic constraints, which might trigger ICEs or other deep compiler bugs related to unused generics."
    }
}