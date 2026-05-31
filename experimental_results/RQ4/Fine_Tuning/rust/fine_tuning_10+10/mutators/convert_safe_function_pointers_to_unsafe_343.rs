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

pub struct Convert_Safe_Function_Pointers_To_Unsafe_343;

impl Mutator for Convert_Safe_Function_Pointers_To_Unsafe_343 {
    fn name(&self) -> &str {
        "Convert_Safe_Function_Pointers_To_Unsafe_343"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::Type(pat_type) = &mut local.pat {
                            if let Type::Path(type_path) = &mut *pat_type.ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    if segment.ident == "fn" {
                                        let mut new_type_path = type_path.clone();
                                        new_type_path.path.segments.insert(0, parse_quote!(unsafe));
                                        pat_type.ty = Box::new(Type::Path(new_type_path));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function pointer declarations within let statements that are declared as safe and modifies them to be unsafe by adding the `unsafe` keyword before the function type. This mutation can potentially lead to unexpected behaviors, especially when optimizations are applied."
    }
}