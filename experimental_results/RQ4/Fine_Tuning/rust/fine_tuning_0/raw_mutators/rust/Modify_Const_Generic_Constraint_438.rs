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

pub struct Modify_Const_Generic_Constraint_438;

impl Mutator for Modify_Const_Generic_Constraint_438 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Constraint_438"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if let Some(const_generic) = generics.params.iter_mut().find_map(|param| {
                    if let syn::GenericParam::Const(const_param) = param {
                        Some(const_param)
                    } else {
                        None
                    }
                }) {
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Array(type_array) = &*pat_type.ty {
                                let new_type = Type::Tuple(syn::TypeTuple {
                                    paren_token: token::Paren::default(),
                                    elems: {
                                        let mut elems = Punctuated::new();
                                        elems.push(Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path::from(Ident::new("usize", Span::call_site())),
                                        }));
                                        elems.push(Type::Array(type_array.clone()));
                                        elems
                                    },
                                });
                                pat_type.ty = Box::new(new_type);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}