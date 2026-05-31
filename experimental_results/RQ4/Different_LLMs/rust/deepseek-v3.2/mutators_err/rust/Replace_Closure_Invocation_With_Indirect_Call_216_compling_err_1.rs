use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Replace_Closure_Invocation_With_Indirect_Call_216;

impl Mutator for Replace_Closure_Invocation_With_Indirect_Call_216 {
    fn name(&self) -> &str {
        "Replace_Closure_Invocation_With_Indirect_Call_216"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ClosureCallVisitor {
            changes: Vec::new(),
        };
        visitor.visit_file_mut(file);
        for change in visitor.changes {
            let stmts = &mut change.block.stmts;
            let index = change.index;
            if index < stmts.len() {
                if let Stmt::Local(local) = &stmts[index] {
                    if let Some(init) = &local.init {
                        if let Expr::Closure(closure) = &*init.expr {
                            let captures = find_captures(closure);
                            if captures.is_empty() {
                                let closure_ty = infer_closure_type(closure);
                                let ptr_name = generate_unique_name(&local.pat, stmts);
                                let ptr_stmt: Stmt = parse_quote! {
                                    let #ptr_name = #init as #closure_ty;
                                };
                                stmts.insert(index + 1, ptr_stmt);
                                for i in (index + 2)..stmts.len() {
                                    replace_closure_calls(&mut stmts[i], &local.pat, &ptr_name);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closure invocations where a closure is stored in a variable and called directly. It transforms the direct call into an indirect call via a function pointer by coercing the closure to a function pointer type. This introduces an extra layer of indirection that may affect MIR inlining, value stealing, and query invalidation, potentially exposing compiler optimization bugs. The mutation only applies to closures with no captures, as those cannot be coerced to function pointers."
    }
}

struct ClosureCallVisitor {
    changes: Vec<BlockChange>,
}

struct BlockChange {
    block: *mut Block,
    index: usize,
}

impl<'ast> VisitMut for ClosureCallVisitor {
    fn visit_block_mut(&mut self, block: &'ast mut Block) {
        for (i, stmt) in block.stmts.iter().enumerate() {
            if let Stmt::Local(local) = stmt {
                if let Some(init) = &local.init {
                    if let Expr::Closure(_) = &*init.expr {
                        self.changes.push(BlockChange {
                            block: block as *mut Block,
                            index: i,
                        });
                    }
                }
            }
        }
        syn::visit_mut::visit_block_mut(self, block);
    }
}

fn find_captures(closure: &ExprClosure) -> Vec<Ident> {
    let mut captures = Vec::new();
    let mut visitor = CaptureVisitor {
        captures: &mut captures,
    };
    visitor.visit_expr_closure(closure);
    captures
}

struct CaptureVisitor<'a> {
    captures: &'a mut Vec<Ident>,
}

impl<'ast> Visit<'ast> for CaptureVisitor<'ast> {
    fn visit_ident(&mut self, ident: &'ast Ident) {
        self.captures.push(ident.clone());
    }
}

fn infer_closure_type(closure: &ExprClosure) -> Type {
    let inputs = closure.inputs.iter().map(|input| {
        if let Pat::Type(pat_type) = input {
            *pat_type.ty.clone()
        } else {
            parse_quote! { _ }
        }
    }).collect::<Vec<_>>();
    let output = match &closure.output {
        ReturnType::Default => parse_quote! { () },
        ReturnType::Type(_, ty) => *ty.clone(),
    };
    let fn_type: Type = if inputs.is_empty() {
        parse_quote! { fn() -> #output }
    } else {
        let inputs_ty = inputs.into_iter().fold(Punctuated::<Type, Comma>::new(), |mut acc, ty| {
            acc.push(ty);
            acc
        });
        parse_quote! { fn(#inputs_ty) -> #output }
    };
    fn_type
}

fn generate_unique_name(pat: &Pat, stmts: &[Stmt]) -> Ident {
    let base_name = match pat {
        Pat::Ident(pat_ident) => pat_ident.ident.to_string(),
        _ => "closure_ptr".to_string(),
    };
    let mut name = base_name.clone();
    let mut counter = 0;
    while stmts.iter().any(|stmt| {
        match stmt {
            Stmt::Local(local) => match &local.pat {
                Pat::Ident(pat_ident) => pat_ident.ident == name,
                _ => false,
            },
            _ => false,
        }
    }) {
        counter += 1;
        name = format!("{}_{}", base_name, counter);
    }
    Ident::new(&name, Span::call_site())
}

fn replace_closure_calls(expr: &mut Expr, old_pat: &Pat, new_name: &Ident) {
    match expr {
        Expr::Call(call) => {
            if let Expr::Path(path) = &*call.func {
                if let Some(segment) = path.path.segments.last() {
                    if let Pat::Ident(old_ident) = old_pat {
                        if segment.ident == old_ident.ident {
                            let new_path: ExprPath = parse_quote! { #new_name };
                            call.func = Box::new(Expr::Path(new_path));
                        }
                    }
                }
            }
        }
        _ => {
            syn::visit_mut::visit_expr_mut(&mut ReplaceVisitor { old_pat, new_name }, expr);
        }
    }
}

struct ReplaceVisitor<'a> {
    old_pat: &'a Pat,
    new_name: &'a Ident,
}

impl<'a> VisitMut for ReplaceVisitor<'a> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(call) = expr {
            if let Expr::Path(path) = &*call.func {
                if let Some(segment) = path.path.segments.last() {
                    if let Pat::Ident(old_ident) = self.old_pat {
                        if segment.ident == old_ident.ident {
                            let new_path: ExprPath = parse_quote! { #self.new_name };
                            call.func = Box::new(Expr::Path(new_path));
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}