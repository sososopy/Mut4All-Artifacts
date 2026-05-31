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

pub struct Replace_Function_Pointer_Comparison_58;

impl Mutator for Replace_Function_Pointer_Comparison_58 {
    fn name(&self) -> &str {
        "Replace_Function_Pointer_Comparison_58"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Binary(binary_expr) = &**expr {
                            if let (Expr::Path(path1), Expr::Path(path2)) = (&binary_expr.left, &binary_expr.right) {
                                if let Some(segment1) = path1.path.segments.last() {
                                    if let Some(segment2) = path2.path.segments.last() {
                                        if segment1.ident == "fn" && segment2.ident == "fn" {
                                            let equal_functions = quote! {
                                                equal_functions(#path1, #path2)
                                            };
                                            *expr = parse_quote! {
                                                #equal_functions
                                            };
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
        "The mutation operator replaces the comparison of function pointers using the equality operator with a call to a new function that takes the two function pointers as arguments and returns a boolean value indicating whether they are equal. This transformation aims to test the compiler's handling of function pointer comparisons and may lead to bugs in the const evaluation pipeline related to the comparison of function pointers with different types or lifetime parameters."
    }
}