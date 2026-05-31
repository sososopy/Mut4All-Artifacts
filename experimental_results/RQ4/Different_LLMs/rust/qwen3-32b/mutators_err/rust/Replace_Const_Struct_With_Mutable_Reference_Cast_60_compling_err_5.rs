use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, ExprReference, Item, ItemFn, ItemStruct, 
    Lifetime, Local, Pat, PatType, Stmt, ReturnType, FnArg, GenericArgument, GenericParam, 
    Type, TypeImplTrait, TypeParamBound, TraitBound, TraitBoundModifier, 
    visit::Visit, visit_mut::VisitMut, 
    punctuated::Punctuated, 
    spanned::Spanned, 
    token::{Paren, Comma, Plus}, 
    parse_quote, 
    Path as SynPath, 
    PathArguments, 
    ReturnType::Default, 
    ItemStatic, 
    ExprStruct, 
    ExprPath, 
    Ident, 
    Box,
};

use crate::mutator::Mutator;

pub struct Replace_Const_Struct_With_Mutable_Reference_Cast_60;

impl Mutator for Replace_Const_Struct_With_Mutable_Reference_Cast_60 {
    fn name(&self) -> &str {
        "Replace_Const_Struct_With_Mutable_Reference_Cast_60"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut static_vars = Vec::new();
        for item in &file.items {
            if let Item::Static(item_static) = item {
                static_vars.push((item_static.ident.clone(), item_static.ty.clone()));
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    if let Some(block) = &mut item_fn.block {
                        for stmt in &mut block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Struct(expr_struct) = expr.as_mut() {
                                    process_struct_expr(expr_struct, &static_vars);
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Const(item_const) = item {
                if let Some(expr) = &mut item_const.expr {
                    if let Expr::Struct(expr_struct) = expr {
                        process_struct_expr(expr_struct, &static_vars);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct initializations in const contexts with mutable reference fields. It replaces such fields with an unsafe cast from a static const variable of the same type, creating a mutable reference from an immutable source, potentially exposing undefined behavior or ICEs in const evaluation."
    }
}

fn process_struct_expr(expr_struct: &mut ExprStruct, static_vars: &Vec<(Ident, Box<Type>)>) {
    for field in &mut expr_struct.fields {
        if let syn::FieldValue { expr, .. } = field {
            if let Expr::Reference(expr_ref) = expr.as_ref() {
                if expr_ref.mutability.is_some() {
                    if let Expr::Path(expr_path) = expr_ref.expr.as_ref() {
                        for (static_ident, _) in static_vars {
                            if expr_path.path.is_ident(static_ident) {
                                let new_expr = parse_quote! {
                                    unsafe { &mut *(#static_ident as *const _ as *mut _) }
                                };
                                *expr = Box::new(new_expr);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}