use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Generic_Const_In_Match_Pattern_369;

impl Mutator for Replace_Generic_Const_In_Match_Pattern_369 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_In_Match_Pattern_369"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceGenericConstVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match patterns that use generic const parameters directly, particularly in slice patterns for arrays with generic const lengths. It transforms empty slice patterns `[]` into `[..]` patterns with a guard condition that compares the length to a const expression involving the generic parameter (e.g., N - 1). This introduces const arithmetic in pattern guards, potentially triggering compiler bugs related to const evaluation in generic contexts, type-checking of const expressions, and match pattern exhaustiveness analysis."
    }
}

struct ReplaceGenericConstVisitor;

impl VisitMut for ReplaceGenericConstVisitor {
    fn visit_expr_match_mut(&mut self, expr: &mut syn::ExprMatch) {
        let mut const_params = Vec::new();
        self.collect_const_params_from_expr(&expr.expr, &mut const_params);
        
        for arm in &mut expr.arms {
            if let Pat::Slice(slice_pat) = &mut arm.pat {
                if slice_pat.elems.is_empty() {
                    if let Some(const_param) = const_params.first() {
                        let new_pat: Pat = parse_quote! { [..] };
                        arm.pat = new_pat;
                        
                        let guard_expr: Expr = parse_quote! { #const_param - 1 == 0 };
                        
                        arm.guard = Some((
                            token::If::default(),
                            Box::new(guard_expr)
                        ));
                    }
                }
            }
        }
        
        visit_mut::visit_expr_match_mut(self, expr);
    }
    
    fn visit_item_fn_mut(&mut self, item: &mut ItemFn) {
        for param in &item.sig.generics.params {
            if let GenericParam::Const(const_param) = param {
                let _const_ident = &const_param.ident;
            }
        }
        
        visit_mut::visit_item_fn_mut(self, item);
    }
    
    fn visit_item_impl_mut(&mut self, item: &mut ItemImpl) {
        for param in &item.generics.params {
            if let GenericParam::Const(const_param) = param {
                let _const_ident = &const_param.ident;
            }
        }
        
        visit_mut::visit_item_impl_mut(self, item);
    }
}

impl ReplaceGenericConstVisitor {
    fn collect_const_params_from_expr(&self, expr: &Expr, params: &mut Vec<Ident>) {
        if let Expr::Path(expr_path) = expr {
            if let Some(segment) = expr_path.path.segments.last() {
                if segment.ident.to_string().chars().next().map_or(false, |c| c.is_uppercase()) {
                    params.push(segment.ident.clone());
                }
            }
        }
    }
}