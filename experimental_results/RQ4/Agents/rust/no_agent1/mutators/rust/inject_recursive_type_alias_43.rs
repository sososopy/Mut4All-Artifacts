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

pub struct Inject_Recursive_Type_Alias_43;

impl Mutator for Inject_Recursive_Type_Alias_43 {
    fn name(&self) -> &str {
        "Inject_Recursive_Type_Alias_43"
    }
    fn mutate(&self, file: &mut syn::File) {
        let recursive_alias: Item = parse_quote! {
            type RecursiveType = Box<RecursiveType>;
        };
        file.items.push(recursive_alias);

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let recursive_param: FnArg = parse_quote!(param: RecursiveType);
                func.sig.inputs.push(recursive_param);
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let recursive_param: FnArg = parse_quote!(param: RecursiveType);
                        func.sig.inputs.push(recursive_param);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `RecursiveType = Box<RecursiveType>` and adds it as a parameter to non-main functions and methods. This transformation tests the compiler's handling of infinitely recursive types, potentially triggering ICEs or stack overflows during type resolution and analysis."
    }
}