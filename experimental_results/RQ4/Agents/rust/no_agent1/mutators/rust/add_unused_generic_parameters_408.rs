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

pub struct Add_Unused_Generic_Parameters_408;

impl Mutator for Add_Unused_Generic_Parameters_408 {
    fn name(&self) -> &str {
        "Add_Unused_Generic_Parameters_408"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                let unused_generic_params = vec![
                    parse_quote!(U: std::fmt::Debug),
                    parse_quote!(V: std::fmt::Display),
                ];
                for param in unused_generic_params {
                    generics.params.push(param);
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        let unused_generic_params = vec![
                            parse_quote!(U: std::fmt::Debug),
                            parse_quote!(V: std::fmt::Display),
                        ];
                        for param in unused_generic_params {
                            generics.params.push(param);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces unused generic parameters with trait bounds into non-main function and impl signatures. These parameters are not utilized within the function body or signature, aiming to explore how the compiler handles extraneous generic parameters and trait bounds. This may uncover issues in generic parameter handling, trait resolution, or cause unexpected behavior in type inference."
    }
}