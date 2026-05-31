use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Swap_Default_Generic_Parameter_With_Previous_348;

impl Mutator for Swap_Default_Generic_Parameter_With_Previous_348 {
    fn name(&self) -> &str {
        "Swap_Default_Generic_Parameter_With_Previous_348"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                let generics = &mut s.generics;
                process_generics(generics);
            } else if let syn::Item::Enum(e) = item {
                let generics = &mut e.generics;
                process_generics(generics);
            } else if let syn::Item::Fn(f) = item {
                let generics = &mut f.sig.generics;
                process_generics(generics);
            } else if let syn::Item::Impl(i) = item {
                for impl_item in &mut i.items {
                    if let syn::ImplItem::Fn(f) = impl_item {
                        let generics = &mut f.sig.generics;
                        process_generics(generics);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the position of a generic parameter with a default and a preceding non-default generic parameter in a struct, enum, or function definition. This reorders parameters to ensure default parameters are not trailing, violating compiler expectations and potentially exposing ICEs during well-formedness checks by disrupting the expected order of generic parameters."
    }
}

fn process_generics(generics: &mut syn::Generics) {
    if generics.params.len() < 2 {
        return;
    }
    for i in 0..generics.params.len() - 1 {
        let param1 = &generics.params[i];
        let param2 = &generics.params[i + 1];
        if has_default(param2) && !has_default(param1) {
            let temp = generics.params[i].clone();
            generics.params[i] = generics.params[i + 1].clone();
            generics.params[i + 1] = temp;
            break;
        }
    }
}

fn has_default(param: &syn::GenericParam) -> bool {
    match param {
        syn::GenericParam::Type(ty) => ty.default.is_some(),
        syn::GenericParam::Const(ct) => ct.default.is_some(),
        _ => false,
    }
}