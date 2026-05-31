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

pub struct Wrap_Static_Struct_Initializer_78;

impl Mutator for Wrap_Static_Struct_Initializer_78 {
    fn name(&self) -> &str {
        "Wrap_Static_Struct_Initializer_78"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(item_static) = item {
                if let Some(expr) = &mut item_static.expr {
                    mutate_static_initializer(expr);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static items with nested struct initializers involving unsized types and references. It changes the static item's initializer expression by adding an extra layer of struct construction or reference indirection using existing types and variables. When a static initializer is a struct constructor call (or a block containing one), it locates a subexpression that is a reference to a struct value and wraps that subexpression in another instantiation of the same struct type (if possible) or adds an explicit dereference followed by an immediate reborrow (i.e., &*expr). If the struct type has a field that is a reference, replicate the field initialization with the wrapped or reborrowed value. This increases complexity of const-evaluated nested initializers and may trigger missing const-eval handling for unevaluated consts."
    }
}

fn mutate_static_initializer(expr: &mut Box<syn::Expr>) {
    struct StructFinder {
        found: Option<syn::Expr>,
    }

    impl syn::visit::Visit<'_> for StructFinder {
        fn visit_expr(&mut self, expr: &syn::Expr) {
            if let syn::Expr::Struct(struct_expr) = expr {
                if let syn::Expr::Reference(_) = &struct_expr.fields.iter().next().map(|f| &f.expr).unwrap_or(&syn::Expr::Verbatim(Default::default())) {
                    self.found = Some(expr.clone());
                }
            }
            syn::visit::visit_expr(self, expr);
        }
    }

    let mut finder = StructFinder { found: None };
    finder.visit_expr(&*expr);
    
    if let Some(target_expr) = finder.found {
        if let syn::Expr::Struct(struct_expr) = target_expr {
            let mut rng = thread_rng();
            if rng.gen_bool(0.5) {
                // Wrap in another struct instantiation
                let new_struct = syn::ExprStruct {
                    attrs: struct_expr.attrs.clone(),
                    qself: struct_expr.qself.clone(),
                    path: struct_expr.path.clone(),
                    brace_token: struct_expr.brace_token.clone(),
                    fields: struct_expr.fields.clone(),
                    dot2_token: struct_expr.dot2_token,
                    rest: struct_expr.rest.clone(),
                };
                let new_expr = syn::Expr::Struct(new_struct);
                *expr = Box::new(syn::Expr::Reference(syn::ExprReference {
                    attrs: vec![],
                    and_token: syn::token::And::default(),
                    mutability: None,
                    expr: Box::new(new_expr),
                }));
            } else {
                // Add dereference-reborrow chain
                let deref_expr = syn::Expr::Unary(syn::ExprUnary {
                    attrs: vec![],
                    op: syn::UnOp::Deref(syn::token::Star::default()),
                    expr: Box::new(syn::Expr::Struct(struct_expr.clone())),
                });
                let reborrow_expr = syn::Expr::Reference(syn::ExprReference {
                    attrs: vec![],
                    and_token: syn::token::And::default(),
                    mutability: None,
                    expr: Box::new(deref_expr),
                });
                *expr = Box::new(reborrow_expr);
            }
        }
    }
}