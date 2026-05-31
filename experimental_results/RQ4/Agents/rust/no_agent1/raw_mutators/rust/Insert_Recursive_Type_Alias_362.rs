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

pub struct Insert_Recursive_Type_Alias_362;

impl Mutator for Insert_Recursive_Type_Alias_362 {
    fn name(&self) -> &str {
        "Insert_Recursive_Type_Alias_362"
    }
    fn mutate(&self, file: &mut syn::File) {
        let recursive_type_alias: Item = parse_quote! {
            type RecursiveAlias = Vec<RecursiveAlias>;
        };

        file.items.push(recursive_type_alias);

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    *ty = Box::new(Type::Path(TypePath {
                        qself: None,
                        path: syn::Path::from(Ident::new("RecursiveAlias", Span::call_site())),
                    }));
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                            *ty = Box::new(Type::Path(TypePath {
                                qself: None,
                                path: syn::Path::from(Ident::new("RecursiveAlias", Span::call_site())),
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type RecursiveAlias = Vec<RecursiveAlias>;` and replaces the return type of non-main functions and methods with this alias. This transformation aims to test the compiler's handling of recursive type definitions, potentially leading to infinite type expansion, ICEs, or other deep compiler bugs related to type resolution."
    }
}