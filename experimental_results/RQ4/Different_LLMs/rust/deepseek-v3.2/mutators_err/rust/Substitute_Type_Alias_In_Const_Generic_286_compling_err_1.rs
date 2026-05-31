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

pub struct Substitute_Type_Alias_In_Const_Generic_286;

impl Mutator for Substitute_Type_Alias_In_Const_Generic_286 {
    fn name(&self) -> &str {
        "Substitute_Type_Alias_In_Const_Generic_286"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases = Vec::new();
        let mut const_decls = Vec::new();
        
        for item in &file.items {
            if let Item::Type(type_item) = item {
                type_aliases.push((type_item.ident.clone(), type_item.ty.clone()));
            }
            if let Item::Const(const_item) = item {
                const_decls.push((const_item.ident.clone(), const_item.ty.clone(), const_item.expr.clone()));
            }
        }
        
        let mut visitor = ConstGenericVisitor {
            type_aliases: &type_aliases,
            const_decls: &const_decls,
            changes: Vec::new(),
        };
        visitor.visit_file_mut(file);
        
        for (path, replacement) in visitor.changes {
            if let Some(segment) = path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(expr) = arg {
                            if let Expr::Path(expr_path) = expr {
                                if expr_path.path.segments.len() == 1 {
                                    let const_name = &expr_path.path.segments[0].ident;
                                    if let Some((_, const_ty, const_expr)) = const_decls.iter().find(|(ident, _, _)| ident == const_name) {
                                        if let Type::Path(type_path) = const_ty {
                                            if type_path.path.segments.len() == 1 {
                                                let alias_name = &type_path.path.segments[0].ident;
                                                if let Some((_, underlying_ty)) = type_aliases.iter().find(|(ident, _)| ident == alias_name) {
                                                    if is_simple_literal(const_expr) {
                                                        *expr = const_expr.clone();
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
        }
    }
    fn chain_of_ought(&self) -> &str {
        "The mutation operator substitutes type aliases with their underlying types in const generic argument positions. It identifies constants defined with type alias types and replaces their usage in const generic arguments with the literal expressions from their initializers. This transformation tests the compiler's handling of type equivalence between aliases and their underlying types in const generic contexts, potentially revealing bugs in type relation or const evaluation."
    }
}

struct ConstGenericVisitor<'a> {
    type_aliases: &'a Vec<(Ident, Box<Type>)>,
    const_decls: &'a Vec<(Ident, Box<Type>, Box<Expr>)>,
    changes: Vec<(SynPath, Box<Expr>)>,
}

impl<'a> VisitMut for ConstGenericVisitor<'a> {
    fn visit_item_impl_mut(&mut self, i: &mut ItemImpl) {
        if let Some((_, path, _)) = &i.trait_ {
            self.visit_path_mut(path);
        }
        for item in &mut i.items {
            match item {
                ImplItem::Fn(f) => {
                    self.visit_signature_mut(&mut f.sig);
                }
                _ => {}
            }
        }
        visit_mut::visit_item_impl_mut(self, i);
    }
    
    fn visit_path_mut(&mut self, path: &mut SynPath) {
        for segment in &mut path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Const(expr) = arg {
                        if let Expr::Path(expr_path) = &**expr {
                            if expr_path.path.segments.len() == 1 {
                                let const_name = &expr_path.path.segments[0].ident;
                                if let Some((_, const_ty, const_expr)) = self.const_decls.iter().find(|(ident, _, _)| ident == const_name) {
                                    if let Type::Path(type_path) = &**const_ty {
                                        if type_path.path.segments.len() == 1 {
                                            let alias_name = &type_path.path.segments[0].ident;
                                            if self.type_aliases.iter().any(|(ident, _)| ident == alias_name) {
                                                if is_simple_literal(const_expr) {
                                                    self.changes.push((path.clone(), const_expr.clone()));
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
        visit_mut::visit_path_mut(self, path);
    }
}

fn is_simple_literal(expr: &Expr) -> bool {
    match expr {
        Expr::Lit(_) => true,
        Expr::Array(_) => true,
        Expr::Repeat(repeat) => is_simple_literal(&repeat.expr),
        _ => false,
    }
}