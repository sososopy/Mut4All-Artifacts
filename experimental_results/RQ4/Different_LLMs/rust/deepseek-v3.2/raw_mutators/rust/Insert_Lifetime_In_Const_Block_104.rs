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

pub struct Insert_Lifetime_In_Const_Block_104;

impl Mutator for Insert_Lifetime_In_Const_Block_104 {
    fn name(&self) -> &str {
        "Insert_Lifetime_In_Const_Block_104"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = InsertLifetimeVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic arguments that are block expressions. It inserts a local variable binding with an explicit lifetime annotation inside the block, before the final expression. The lifetime must be an existing named lifetime parameter from the surrounding function's generic parameters. This transformation stresses the interaction between region inference and const evaluation within generic const arguments, mimicking the pattern that triggered an ICE in a bug report."
    }
}

struct InsertLifetimeVisitor {
    available_lifetimes: Vec<Lifetime>,
}

impl InsertLifetimeVisitor {
    fn new() -> Self {
        Self {
            available_lifetimes: Vec::new(),
        }
    }

    fn collect_lifetimes(&mut self, generics: &Generics) {
        for param in &generics.params {
            if let GenericParam::Lifetime(lifetime_param) = param {
                self.available_lifetimes.push(lifetime_param.lifetime.clone());
            }
        }
    }

    fn clear_lifetimes(&mut self) {
        self.available_lifetimes.clear();
    }
}

impl VisitMut for InsertLifetimeVisitor {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        self.collect_lifetimes(&i.sig.generics);
        syn::visit_mut::visit_item_fn_mut(self, i);
        self.clear_lifetimes();
    }

    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(call_expr) = expr {
            if let Expr::Path(path_expr) = &*call_expr.func {
                if let PathArguments::AngleBracketed(args) = &mut path_expr.path.segments.last_mut().unwrap().arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(const_arg) = arg {
                            if let Expr::Block(block_expr) = &mut *const_arg {
                                if !self.available_lifetimes.is_empty() {
                                    let lifetime = self.available_lifetimes.first().unwrap().clone();
                                    let new_stmt: Stmt = parse_quote! {
                                        let _: & #lifetime ();
                                    };
                                    block_expr.block.stmts.insert(0, new_stmt);
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if let PathArguments::AngleBracketed(args) = &mut type_path.path.segments.last_mut().unwrap().arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Const(const_arg) = arg {
                        if let Expr::Block(block_expr) = &mut *const_arg {
                            if !self.available_lifetimes.is_empty() {
                                let lifetime = self.available_lifetimes.first().unwrap().clone();
                                let new_stmt: Stmt = parse_quote! {
                                    let _: & #lifetime ();
                                };
                                block_expr.block.stmts.insert(0, new_stmt);
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}