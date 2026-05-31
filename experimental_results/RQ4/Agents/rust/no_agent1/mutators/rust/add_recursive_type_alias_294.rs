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

pub struct Add_Recursive_Type_Alias_294;

impl Mutator for Add_Recursive_Type_Alias_294 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_294"
    }
    fn mutate(&self, file: &mut syn::File) {
        let type_alias: Item = parse_quote! {
            type Recursive = Box<Recursive>;
        };
        file.items.push(type_alias);

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    func.sig.inputs.push(parse_quote!(recursive: Recursive));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident != "main" {
                            func.sig.inputs.push(parse_quote!(recursive: Recursive));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type Recursive = Box<Recursive>;` and adds it as a parameter to non-main functions and methods. This transformation creates a scenario that can lead to infinite type recursion, challenging the compiler's type resolution capabilities and potentially triggering ICEs or other deep compiler bugs related to recursive type handling."
    }
}