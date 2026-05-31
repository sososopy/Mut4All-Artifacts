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

pub struct Modify_Scalar_Init_In_Structs_4;

impl Mutator for Modify_Scalar_Init_In_Structs_4 {
    fn name(&self) -> &str {
        "Modify_Scalar_Init_In_Structs_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Type::Path(type_path) = &*item_static.ty {
                    for segment in &type_path.path.segments {
                        if let Item::Struct(ItemStruct { fields, .. }) = item_static {
                            for field in fields {
                                if let Type::Path(type_path) = &field.ty {
                                    if let Some(ident) = &field.ident {
                                        if let Expr::Lit(expr_lit) = &mut *item_static.expr {
                                            if matches!(type_path.path.segments.last().unwrap().ident.to_string().as_str(), "i32" | "i64" | "f32" | "f64" | "bool") {
                                                let new_expr: Expr = match expr_lit.lit {
                                                    syn::Lit::Int(_) => parse_quote! { "unexpected_string" },
                                                    syn::Lit::Bool(_) => parse_quote! { 42 },
                                                    _ => continue,
                                                };
                                                *item_static.expr = Box::new(new_expr);
                                            }
                                        }
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
        "This mutation operator modifies the initialization of scalar fields within struct definitions. It targets scalar fields expected to be of primitive types and replaces their initialization with a mismatched type, such as replacing an integer with a string. This aims to expose bugs in constant evaluation or type checking by creating intentional type mismatches."
    }
}