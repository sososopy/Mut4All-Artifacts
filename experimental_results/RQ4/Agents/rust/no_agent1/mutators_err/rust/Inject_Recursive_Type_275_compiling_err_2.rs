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

pub struct Inject_Recursive_Type_275;

impl Mutator for Inject_Recursive_Type_275 {
    fn name(&self) -> &str {
        "Inject_Recursive_Type_275"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let struct_ident = &item_struct.ident;
                let recursive_type: Type = parse_quote! {
                    Box<#struct_ident>
                };
                item_struct.fields.iter_mut().for_each(|field| {
                    field.ty = recursive_type.clone();
                });
            }
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let recursive_type: Type = parse_quote! {
                        Box<dyn Fn() -> ()
                    };
                    func.sig.output = syn::ReturnType::Type(
                        syn::token::RArrow {
                            spans: [Span::call_site(), Span::call_site()],
                        },
                        Box::new(recursive_type),
                    );
                    let placeholder_expr: Expr = parse_quote! { Box::new(|| {}) };
                    let mut new_block = func.block.clone();
                    new_block.stmts.clear();
                    new_block.stmts.push(Stmt::Expr(placeholder_expr));
                    func.block = new_block;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces recursive types by modifying struct fields to contain boxed instances of the struct itself and changes function return types to boxed closures. This transformation aims to stress the compiler's handling of recursive type definitions and closure boxing, potentially triggering ICEs or infinite recursion in type resolution."
    }
}