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

pub struct Replace_Const_Block_With_Generic_Conditional_428;

impl Mutator for Replace_Const_Block_With_Generic_Conditional_428 {
    fn name(&self) -> &str {
        "Replace_Const_Block_With_Generic_Conditional_428"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ConstBlockMutatorVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const blocks within generic contexts, replacing their bodies with a conditional expression where one branch uses a generic type parameter (via size_of or align_of) and the other uses a constant. It reuses existing generic parameters and function parameters or constants from the seed program to create a const block whose evaluation depends on generic parameters, potentially exposing missing entries in the compiler's internal maps during MIR construction."
    }
}

struct ConstBlockMutatorVisitor;

impl VisitMut for ConstBlockMutatorVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Const(expr_const) = expr {
            if let Some(enclosing_generics) = self.find_enclosing_generics(expr_const) {
                let type_param = self.first_type_param(&enclosing_generics);
                let (cond_lhs, cond_rhs) = self.find_comparison_operands(&enclosing_generics);
                let new_const_block = self.build_new_const_block(type_param, cond_lhs, cond_rhs);
                *expr_const = parse_quote!(const { #new_const_block });
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

impl ConstBlockMutatorVisitor {
    fn find_enclosing_generics(&self, expr_const: &ExprConst) -> Option<Generics> {
        let mut generics = None;
        let mut visitor = EnclosingGenericsVisitor {
            target_span: expr_const.span(),
            found_generics: &mut generics,
        };
        visitor.visit_expr_const(expr_const);
        generics
    }

    fn first_type_param(&self, generics: &Generics) -> Option<Ident> {
        generics.params.iter().find_map(|param| {
            if let GenericParam::Type(type_param) = param {
                Some(type_param.ident.clone())
            } else {
                None
            }
        })
    }

    fn find_comparison_operands(&self, generics: &Generics) -> (Expr, Expr) {
        let mut param_idents = Vec::new();
        let mut visitor = ParamCollectorVisitor {
            param_idents: &mut param_idents,
        };
        visitor.visit_generics(generics);

        if param_idents.len() >= 2 {
            let lhs = parse_quote!(#(param_idents[0]));
            let rhs = parse_quote!(#(param_idents[1]));
            (lhs, rhs)
        } else {
            let lhs = parse_quote!(0);
            let rhs = parse_quote!(1);
            (lhs, rhs)
        }
    }

    fn build_new_const_block(&self, type_param: Option<Ident>, cond_lhs: Expr, cond_rhs: Expr) -> Expr {
        let generic_branch: Expr = if let Some(t) = type_param {
            parse_quote!(std::mem::size_of::<#t>())
        } else {
            parse_quote!(0)
        };
        let constant_branch: Expr = parse_quote!(usize::MAX);
        parse_quote!({
            if #cond_lhs < #cond_rhs {
                #generic_branch
            } else {
                #constant_branch
            }
        })
    }
}

struct EnclosingGenericsVisitor<'a> {
    target_span: Span,
    found_generics: &'a mut Option<Generics>,
}

impl<'a> Visit<'_> for EnclosingGenericsVisitor<'a> {
    fn visit_item_fn(&mut self, i: &ItemFn) {
        if i.sig.generics.span().covers(self.target_span) {
            *self.found_generics = Some(i.sig.generics.clone());
        }
        syn::visit::visit_item_fn(self, i);
    }

    fn visit_impl_item_fn(&mut self, i: &ImplItemFn) {
        if i.sig.generics.span().covers(self.target_span) {
            *self.found_generics = Some(i.sig.generics.clone());
        }
        syn::visit::visit_impl_item_fn(self, i);
    }
}

struct ParamCollectorVisitor<'a> {
    param_idents: &'a mut Vec<Ident>,
}

impl<'a> Visit<'_> for ParamCollectorVisitor<'a> {
    fn visit_fn_arg(&mut self, arg: &FnArg) {
        if let FnArg::Typed(PatType { pat, .. }) = arg {
            if let Pat::Ident(pat_ident) = &**pat {
                self.param_idents.push(pat_ident.ident.clone());
            }
        }
        syn::visit::visit_fn_arg(self, arg);
    }
}