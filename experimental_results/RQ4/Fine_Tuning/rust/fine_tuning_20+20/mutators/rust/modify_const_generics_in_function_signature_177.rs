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

pub struct Modify_Const_Generics_In_Function_Signature_177;

impl Mutator for Modify_Const_Generics_In_Function_Signature_177 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_In_Function_Signature_177"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_ident = None;
        let mut target_type = None;
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if segment.ident == "usize" {
                                    target_ident = Some(item_fn.sig.ident.clone());
                                    target_type = Some(const_param.ident.clone());
                                }
                            }
                        }
                    }
                }
            }
        }
        if let Some(target_ident) = target_ident {
            for item in &mut file.items {
                if let Item::Fn(item_fn) = item {
                    if item_fn.sig.ident == target_ident {
                        for param in &mut item_fn.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if let Type::Path(type_path) = &const_param.ty {
                                    if let Some(segment) = type_path.path.segments.last() {
                                        if segment.ident == "usize" {
                                            const_param.ty = parse_quote!(u32);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            for item in &mut file.items {
                if let Item::Fn(item_fn) = item {
                    if item_fn.sig.ident == target_ident {
                        let mut visitor = ConstGenericVisitor {
                            target: target_type.clone(),
                            found: false,
                        };
                        visitor.visit_block(&item_fn.block);
                        if visitor.found {
                            for stmt in &mut item_fn.block.stmts {
                                if let Stmt::Item(Item::Const(item_const)) = stmt {
                                    if item_const.ident == "N" {
                                        item_const.ty = parse_quote!(u32);
                                    }
                                }
                            }
                        } else {
                            item_fn.block.stmts.insert(
                                0,
                                parse_quote!(const N: u32 = N as u32;),
                            );
                        }
                    }
                }
            }
            for item in &mut file.items {
                if let Item::Fn(item_fn) = item {
                    let mut visitor = InlineAsmVisitor {
                        target: target_type.clone(),
                        found: false,
                    };
                    visitor.visit_block(&item_fn.block);
                    if visitor.found {
                        for stmt in &mut item_fn.block.stmts {
                            if let Stmt::Item(Item::Const(item_const)) = stmt {
                                if item_const.ident == "N" {
                                    item_const.ty = parse_quote!(u32);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with const generics of type `usize` and changes them to `u32`. It ensures that the function body and usages are updated accordingly, particularly handling inline assembly by introducing or modifying auxiliary constants. This transformation stresses the compiler's const generic system by altering type assumptions and requires robust handling of type changes in generic contexts."
    }
}

struct ConstGenericVisitor {
    target: Option<Ident>,
    found: bool,
}

impl<'ast> Visit<'ast> for ConstGenericVisitor {
    fn visit_expr(&mut self, node: &'ast Expr) {
        if let Expr::Path(expr_path) = node {
            if let Some(ident) = expr_path.path.get_ident() {
                if let Some(target) = &self.target {
                    if ident == target {
                        self.found = true;
                    }
                }
            }
        }
        syn::visit::visit_expr(self, node);
    }
    fn visit_stmt(&mut self, node: &'ast Stmt) {
        if let Stmt::Item(Item::Const(item_const)) = node {
            if item_const.ident == "N" {
                self.found = true;
            }
        }
        syn::visit::visit_stmt(self, node);
    }
}

struct InlineAsmVisitor {
    target: Option<Ident>,
    found: bool,
}

impl<'ast> Visit<'ast> for InlineAsmVisitor {
    fn visit_expr(&mut self, node: &'ast Expr) {
        if let Expr::Path(expr_path) = node {
            if let Some(ident) = expr_path.path.get_ident() {
                if let Some(target) = &self.target {
                    if ident == target {
                        self.found = true;
                    }
                }
            }
        }
        syn::visit::visit_expr(self, node);
    }
    fn visit_stmt(&mut self, node: &'ast Stmt) {
        if let Stmt::Item(Item::Const(item_const)) = node {
            if item_const.ident == "N" {
                self.found = true;
            }
        }
        syn::visit::visit_stmt(self, node);
    }
}