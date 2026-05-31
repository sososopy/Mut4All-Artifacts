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

pub struct Const_Generic_Lifetime_In_Function_Signature_285;

impl Mutator for Const_Generic_Lifetime_In_Function_Signature_285 {
    fn name(&self) -> &str {
        "Const_Generic_Lifetime_In_Function_Signature_285"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    let has_const_generic = generics.params.iter().any(|param| match param {
                        GenericParam::Const(_) => true,
                        _ => false,
                    });
                    if has_const_generic {
                        generics.params.push(parse_quote!(const Z: &'static str));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with const generics in their signature and adds a const generic parameter with a concrete lifetime ('static). The transformation aims to explore interactions between const generics and lifetimes, potentially revealing compiler bugs related to lifetime handling in const contexts."
    }
}