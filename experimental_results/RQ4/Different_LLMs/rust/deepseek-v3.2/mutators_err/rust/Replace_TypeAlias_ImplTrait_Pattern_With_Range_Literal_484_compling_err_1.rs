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

pub struct Replace_TypeAlias_ImplTrait_Pattern_With_Range_Literal_484;

impl Mutator for Replace_TypeAlias_ImplTrait_Pattern_With_Range_Literal_484 {
    fn name(&self) -> &str {
        "Replace_TypeAlias_ImplTrait_Pattern_With_Range_Literal_484"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases = Vec::new();
        for item in &file.items {
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(_) = &item_type.ty {
                    type_aliases.push(item_type.ident.to_string());
                }
            }
        }
        
        if type_aliases.is_empty() {
            return;
        }
        
        let mut visitor = PatternVisitor {
            type_aliases: &type_aliases,
            rng: thread_rng(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator locates type aliases defined with `impl Trait` and patterns that destructure values of those types. It replaces binding patterns within tuple or struct patterns with range patterns (e.g., 0..=255) or literal patterns (e.g., true, 42). This transformation creates type mismatches and pattern exhaustiveness issues, testing the compiler's handling of impl trait type inference, pattern matching validation, and internal consistency checks for opaque types in destructuring contexts."
    }
}

struct PatternVisitor<'a> {
    type_aliases: &'a Vec<String>,
    rng: rand::rngs::ThreadRng,
}

impl<'a> VisitMut for PatternVisitor<'a> {
    fn visit_local_mut(&mut self, local: &mut Local) {
        if let Some((_, init)) = &local.init {
            self.visit_pattern_in_context(&mut local.pat, init);
        }
        syn::visit_mut::visit_local_mut(self, local);
    }
    
    fn visit_expr_match_mut(&mut self, expr_match: &mut ExprMatch) {
        for arm in &mut expr_match.arms {
            self.visit_pattern_in_context(&mut arm.pat, &expr_match.expr);
        }
        syn::visit_mut::visit_expr_match_mut(self, expr_match);
    }
    
    fn visit_expr_if_let_mut(&mut self, expr_if_let: &mut ExprIfLet) {
        self.visit_pattern_in_context(&mut expr_if_let.pat, &expr_if_let.expr);
        syn::visit_mut::visit_expr_if_let_mut(self, expr_if_let);
    }
    
    fn visit_expr_while_let_mut(&mut self, expr_while_let: &mut ExprWhileLet) {
        self.visit_pattern_in_context(&mut expr_while_let.pat, &expr_while_let.expr);
        syn::visit_mut::visit_expr_while_let_mut(self, expr_while_let);
    }
}

impl<'a> PatternVisitor<'a> {
    fn visit_pattern_in_context(&mut self, pat: &mut Pat, expr: &Expr) {
        if self.should_mutate_pattern(pat, expr) {
            self.mutate_pattern(pat);
        }
        syn::visit_mut::visit_pat_mut(self, pat);
    }
    
    fn should_mutate_pattern(&mut self, pat: &Pat, expr: &Expr) -> bool {
        let expr_type = self.infer_expr_type(expr);
        if let Some(type_name) = expr_type {
            self.type_aliases.contains(&type_name)
        } else {
            false
        }
    }
    
    fn infer_expr_type(&self, expr: &Expr) -> Option<String> {
        match expr {
            Expr::Path(expr_path) => {
                if let Some(segment) = expr_path.path.segments.last() {
                    if let PathArguments::AngleBracketed(args) = &segment.arguments {
                        for arg in &args.args {
                            if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                if let Some(segment) = type_path.path.segments.last() {
                                    return Some(segment.ident.to_string());
                                }
                            }
                        }
                    }
                    return Some(segment.ident.to_string());
                }
            }
            Expr::Call(expr_call) => {
                return self.infer_expr_type(&expr_call.func);
            }
            Expr::MethodCall(expr_method) => {
                return self.infer_expr_type(&expr_method.receiver);
            }
            _ => {}
        }
        None
    }
    
    fn mutate_pattern(&mut self, pat: &mut Pat) {
        match pat {
            Pat::Tuple(tuple_pat) => {
                for elem in &mut tuple_pat.elems {
                    self.mutate_binding_pattern(elem);
                }
            }
            Pat::Struct(struct_pat) => {
                for field in &mut struct_pat.fields {
                    self.mutate_binding_pattern(&mut field.pat);
                }
            }
            Pat::TupleStruct(tuple_struct_pat) => {
                for elem in &mut tuple_struct_pat.elems {
                    self.mutate_binding_pattern(elem);
                }
            }
            Pat::Ident(ident_pat) => {
                if ident_pat.by_ref.is_none() && ident_pat.mutability.is_none() {
                    *pat = self.generate_replacement_pattern();
                }
            }
            Pat::Wild(_) => {
                *pat = self.generate_replacement_pattern();
            }
            _ => {}
        }
    }
    
    fn mutate_binding_pattern(&mut self, pat: &mut Pat) {
        match pat {
            Pat::Ident(ident_pat) => {
                if ident_pat.by_ref.is_none() && ident_pat.mutability.is_none() {
                    *pat = self.generate_replacement_pattern();
                }
            }
            Pat::Wild(_) => {
                *pat = self.generate_replacement_pattern();
            }
            Pat::Tuple(tuple_pat) => {
                for elem in &mut tuple_pat.elems {
                    self.mutate_binding_pattern(elem);
                }
            }
            Pat::Struct(struct_pat) => {
                for field in &mut struct_pat.fields {
                    self.mutate_binding_pattern(&mut field.pat);
                }
            }
            _ => {}
        }
    }
    
    fn generate_replacement_pattern(&mut self) -> Pat {
        let choice = self.rng.gen_range(0..3);
        match choice {
            0 => parse_quote!(0..=255),
            1 => parse_quote!(true),
            2 => parse_quote!(42),
            _ => parse_quote!(false),
        }
    }
}