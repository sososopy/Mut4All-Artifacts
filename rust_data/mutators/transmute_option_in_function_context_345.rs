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

pub struct Transmute_Option_In_Function_Context_345;

impl Mutator for Transmute_Option_In_Function_Context_345 {
    fn name(&self) -> &str {
        "Transmute_Option_In_Function_Context_345"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        struct FunctionVisitor {
            found: bool,
        }
        
        impl VisitMut for FunctionVisitor {
            fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
                if self.found {
                    return;
                }
                
                let mut option_type: Option<Type> = None;
                let mut numeric_type: Option<Type> = None;
                
                for stmt in &i.block.stmts {
                    if let Stmt::Local(Local { pat, init: Some(init), .. }) = stmt {
                        if let Expr::Path(ExprPath { path, .. }) = &*init.expr {
                            if path.segments.iter().any(|seg| seg.ident == "Option") {
                                if let Pat::Type(PatType { ty, .. }) = pat {
                                    option_type = Some((**ty).clone());
                                }
                            }
                        }
                    }
                }
                
                if let ReturnType::Type(_, ty) = &i.sig.output {
                    numeric_type = Some((**ty).clone());
                }
                
                if let (Some(option_ty), Some(numeric_ty)) = (option_type, numeric_type) {
                    let transmute_fn: ItemFn = parse_quote! {
                        unsafe fn transmute_option(option: #option_ty) -> #numeric_ty {
                            std::mem::transmute(option)
                        }
                    };
                    
                    i.block.stmts.insert(0, Stmt::Item(Item::Fn(transmute_fn)));
                    
                    for stmt in &mut i.block.stmts {
                        if let Stmt::Local(Local { init: Some(init), .. }) = stmt {
                            if let Expr::Unsafe(unsafe_expr) = &mut *init.expr {
                                if let Some(Stmt::Expr(Expr::Call(ExprCall { func, args, .. }), _)) = unsafe_expr.block.stmts.first_mut() {
                                    if let Expr::Path(ExprPath { path, .. }) = &**func {
                                        if path.is_ident("std::mem::transmute") {
                                            if let Some(arg) = args.first() {
                                                let new_call: Expr = parse_quote! {
                                                    transmute_option(#arg)
                                                };
                                                *init.expr = new_call;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                    self.found = true;
                }
            }
        }
        
        let mut visitor = FunctionVisitor { found: false };
        visitor.visit_file_mut(file);
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions with Option<T> variables and numeric return types, refactoring the transmutation logic into a standalone unsafe function. This separation may influence compiler behavior and expose optimization-related issues."
    }
}