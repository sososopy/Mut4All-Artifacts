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

pub struct Change_Closure_Return_Type_216;

impl Mutator for Change_Closure_Return_Type_216 {
    fn name(&self) -> &str {
        "Change_Closure_Return_Type_216"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let syn::Stmt::Local(local) = stmt {
                        if let syn::Expr::Closure(closure) = &mut *local.init.as_mut().unwrap().1 {
                            if let syn::ReturnType::Type(_, ref mut ty) = closure.output {
                                if let Type::Path(type_path) = &**ty {
                                    if let Some(segment) = type_path.path.segments.last() {
                                        match segment.ident.to_string().as_str() {
                                            "u8" => {
                                                *ty = Box::new(parse_quote!(i16));
                                                if let Expr::Lit(expr_lit) = &mut *closure.body {
                                                    if let syn::Lit::Int(lit_int) = &mut expr_lit.lit {
                                                        lit_int.set_suffix("i16");
                                                    }
                                                }
                                            }
                                            "i32" => {
                                                *ty = Box::new(parse_quote!(u64));
                                                if let Expr::Lit(expr_lit) = &mut *closure.body {
                                                    if let syn::Lit::Int(lit_int) = &mut expr_lit.lit {
                                                        lit_int.set_suffix("u64");
                                                    }
                                                }
                                            }
                                            _ => {}
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
        "This mutation operator targets closures within functions, specifically altering their return types from primitive types like `u8` to `i16`, or `i32` to `u64`. This transformation tests the compiler's ability to handle implicit or explicit type conversions, potentially revealing issues with type inference or optimization."
    }
}