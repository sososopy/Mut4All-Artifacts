use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Generator_Return_Type_122;

impl Mutator for Replace_Generator_Return_Type_122 {
    fn name(&self) -> &str {
        "Replace_Generator_Return_Type_122"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref mut ty) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**ty {
                        if let Some(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments,
                            },
                        }) = type_impl_trait.bounds.iter().next()
                        {
                            if segments.iter().next().unwrap().ident == "Generator" {
                                let new_return_type = parse_quote! {
                                    impl Generator<(), Return = i32, Yield = i32>
                                };
                                *ty = Box::new(new_return_type);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the return type of generator functions with a different generator type. It aims to test the compiler's handling of different return types for generators, which may expose bugs related to type checking and code generation."
    }
}