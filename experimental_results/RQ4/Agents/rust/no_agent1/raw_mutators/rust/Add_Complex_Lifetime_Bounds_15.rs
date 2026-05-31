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

pub struct Add_Complex_Lifetime_Bounds_15;

impl Mutator for Add_Complex_Lifetime_Bounds_15 {
    fn name(&self) -> &str {
        "Add_Complex_Lifetime_Bounds_15"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetimes = &mut func.sig.generics.params;
                let lifetime_param: LifetimeParam = parse_quote!('a: 'b + 'c);
                lifetimes.push(GenericParam::Lifetime(lifetime_param));
                
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(type_path) = &**ty {
                        let segment = type_path.path.segments.last_mut().unwrap();
                        segment.arguments = PathArguments::AngleBracketed(parse_quote!(<'a>));
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetimes = &mut func.sig.generics.params;
                        let lifetime_param: LifetimeParam = parse_quote!('x: 'y + 'z);
                        lifetimes.push(GenericParam::Lifetime(lifetime_param));
                        
                        if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                            if let Type::Path(type_path) = &**ty {
                                let segment = type_path.path.segments.last_mut().unwrap();
                                segment.arguments = PathArguments::AngleBracketed(parse_quote!(<'x>));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds complex lifetime bounds to function and method signatures, introducing multiple lifetime dependencies and constraints. By altering both the generics and return types, it aims to stress the compiler's lifetime resolution system, potentially leading to ICEs or inference failures in scenarios with intricate lifetime hierarchies."
    }
}