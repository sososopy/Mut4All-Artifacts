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

pub struct Change_Generic_Const_Expr_To_Non_Generic_Const_Expr_390;

impl Mutator for Change_Generic_Const_Expr_To_Non_Generic_Const_Expr_390 {
    fn name(&self) -> &str {
        "Change_Generic_Const_Expr_To_Non_Generic_Const_Expr_390"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for attr in &mut struct_item.attrs {
                    if let Meta::List(list) = attr.parse_meta().unwrap() {
                        if list.path.is_ident("const") {
                            for item in &mut list.nested {
                                if let NestedMeta::Meta(Meta::Path(path)) = item {
                                    if path.is_ident("generic") {
                                        // Replace generic type with non-generic type
                                        let non_generic_type = parse_quote! { i32 };
                                        item = NestedMeta::Meta(Meta::Path(non_generic_type));
                                    }
                                }
                            }
                        }
                    }
                }
            } else if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Path(path) = expr {
                            if path.path.is_ident("generic") {
                                // Replace generic type with non-generic type
                                let non_generic_type = parse_quote! { 42 };
                                *expr = Expr::Lit(Lit::Int(non_generic_type));
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant expressions that use generic types and replaces them with non-generic constant expressions. This transformation aims to test the compiler's ability to handle generic constant expressions and ensure that it can correctly replace them with non-generic constant expressions."
    }
}