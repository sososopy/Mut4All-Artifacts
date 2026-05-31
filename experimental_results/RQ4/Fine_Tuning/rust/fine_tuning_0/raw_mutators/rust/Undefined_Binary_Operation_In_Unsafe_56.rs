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

pub struct Undefined_Binary_Operation_In_Unsafe_56;

impl Mutator for Undefined_Binary_Operation_In_Unsafe_56 {
    fn name(&self) -> &str {
        "Undefined_Binary_Operation_In_Unsafe_56"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut rng = thread_rng();
                let mut found_unsafe_block = false;
                
                for stmt in &mut func.block.stmts {
                    if let syn::Stmt::Expr(expr) = stmt {
                        if let syn::Expr::Unsafe(expr_unsafe) = expr {
                            found_unsafe_block = true;

                            for inner_stmt in &mut expr_unsafe.block.stmts {
                                if let syn::Stmt::Local(local) = inner_stmt {
                                    if let Some((_, expr)) = &mut local.init {
                                        if let syn::Expr::Binary(expr_binary) = expr.as_mut() {
                                            let new_float_var: syn::Local = parse_quote! {
                                                let c: f32 = 3.14;
                                            };

                                            expr_unsafe.block.stmts.insert(0, syn::Stmt::Local(new_float_var));

                                            expr_binary.right = Box::new(parse_quote!(c));
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if found_unsafe_block {
                    break;
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets arithmetic operations within unsafe blocks, replacing one operand with a floating-point type to create undefined or incompatible operations. This aims to test the compiler's handling of type mismatches in unsafe contexts, potentially exposing issues in type checking or code generation."
    }
}