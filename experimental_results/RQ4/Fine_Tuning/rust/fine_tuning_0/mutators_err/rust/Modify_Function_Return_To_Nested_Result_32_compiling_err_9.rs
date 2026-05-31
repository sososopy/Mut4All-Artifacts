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

pub struct Modify_Function_Return_To_Nested_Result_32;

impl Mutator for Modify_Function_Return_To_Nested_Result_32 {
    fn name(&self) -> &str {
        "Modify_Function_Return_To_Nested_Result_32"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, ref mut return_type) = func.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if let Some(last_segment) = type_path.path.segments.last() {
                            if last_segment.ident == "Result" {
                                let original_type = return_type.clone();
                                *return_type = Box::new(parse_quote! {
                                    Result<#original_type, _>
                                });

                                let mut new_block = func.block.clone();
                                for stmt in &mut new_block.stmts {
                                    if let Stmt::Expr(expr) | Stmt::Semi(expr, _) = stmt {
                                        *expr = parse_quote! {
                                            Ok(#expr)
                                        };
                                    }
                                }
                                func.block = new_block;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions returning a Result type and modifies the return type to a nested Result structure. It updates the function implementation to wrap existing return expressions in an additional Ok layer. This transformation aims to test the borrow checker's handling of complex nested result types and their interactions with lifetimes and ownership rules."
    }
}