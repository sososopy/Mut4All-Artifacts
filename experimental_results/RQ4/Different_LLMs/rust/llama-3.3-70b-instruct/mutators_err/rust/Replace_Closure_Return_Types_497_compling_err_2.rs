use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use rand::prelude::IteratorRandom;
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

pub struct Replace_Closure_Return_Types_497;

impl Mutator for Replace_Closure_Return_Types_497 {
    fn name(&self) -> &str {
        "Replace_Closure_Return_Types_497"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut existing_types = HashSet::new();

        // Collect existing types in the file
        for item in &file.items {
            if let Item::Struct(struct_item) = item {
                existing_types.insert(struct_item.ident.clone());
            }
            if let Item::Enum(enum_item) = item {
                existing_types.insert(enum_item.ident.clone());
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, ref mut ty) = &mut func.sig.output {
                    if let Type::Path(TypePath { qself: None, path }) = &**ty {
                        let replacement_type = existing_types.iter().choose(&mut thread_rng()).unwrap();
                        *ty = parse_quote! { #replacement_type };
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let ReturnType::Type(_, ref mut ty) = &mut func.sig.output {
                            if let Type::Path(TypePath { qself: None, path }) = &**ty {
                                let replacement_type = existing_types.iter().choose(&mut thread_rng()).unwrap();
                                *ty = parse_quote! { #replacement_type };
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the return types of closures with existing types in the program. This transformation aims to test the compiler's ability to handle closures with different return types, potentially triggering type inference and checking issues."
    }
}