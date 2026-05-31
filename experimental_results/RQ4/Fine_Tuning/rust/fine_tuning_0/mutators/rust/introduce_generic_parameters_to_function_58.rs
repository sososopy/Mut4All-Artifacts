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

pub struct Introduce_Generic_Parameters_To_Function_58;

impl Mutator for Introduce_Generic_Parameters_To_Function_58 {
    fn name(&self) -> &str {
        "Introduce_Generic_Parameters_To_Function_58"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                
                // Add a new generic parameter T
                let new_generic: GenericParam = parse_quote!(T);
                func.sig.generics.params.push(new_generic);

                // Replace the type of the first parameter with the new generic type T
                if let Some(FnArg::Typed(pat_type)) = func.sig.inputs.first_mut() {
                    pat_type.ty = Box::new(parse_quote!(T));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a new generic parameter T to function signatures and replaces the type of the first parameter with T. This transformation aims to test the compiler's handling of generic parameters in function signatures, potentially leading to type-checking errors or ICEs when the function body is not compatible with the new generic type."
    }
}