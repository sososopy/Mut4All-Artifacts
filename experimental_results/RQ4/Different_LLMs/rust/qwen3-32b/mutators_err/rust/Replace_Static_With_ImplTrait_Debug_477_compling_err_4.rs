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

pub struct Replace_Static_With_ImplTrait_Debug_477;

impl Mutator for Replace_Static_With_ImplTrait_Debug_477 {
    fn name(&self) -> &str {
        "Replace_Static_With_ImplTrait_Debug_477"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(static_item) = item {
                if let syn::StaticMutability::Mut(_) = static_item.mutability {
                    let alias_name = Ident::new("Alias_Debug_477", Span::call_site());
                    let alias_item = parse_quote! {
                        pub type #alias_name = impl core::fmt::Debug;
                    };
                    let index = file.items.iter().position(|i| i == item).unwrap();
                    file.items.insert(index, alias_item);
                    let function_name = Ident::new("dummy_477", Span::call_site());
                    let function_item = parse_quote! {
                        pub fn #function_name() -> #alias_name {
                            Default::default()
                        }
                    };
                    file.items.insert(index + 1, function_item);
                    let path = syn::Path {
                        leading_colon: None,
                        segments: Punctuated::from_iter(vec![syn::PathSegment {
                            ident: alias_name,
                            arguments: syn::PathArguments::None,
                        }]),
                    };
                    static_item.ty = Box::new(Type::Path(syn::TypePath {
                        qself: None,
                        path,
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}.