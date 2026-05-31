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

pub struct Closure_Parameter_Variance_145;

impl Mutator for Closure_Parameter_Variance_145 {
    fn name(&self) -> &str {
        "Closure_Parameter_Variance_145"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                if let Some(lifetime) = item_fn.sig.generics.lifetimes().next() {
                    let mut found = false;
                    for (index, input) in item_fn.sig.inputs.iter_mut().enumerate() {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Reference(type_reference) = &*pat_type.ty {
                                if type_reference
                                    .lifetime
                                    .as_ref()
                                    .map_or(false, |lt| lt.ident == lifetime.lifetime.ident)
                                {
                                    let closure_ident =
                                        Ident::new(&format!("closure_param_{}", index), Span::call_site());
                                    let closure_type: Type = parse_quote! {
                                        impl FnOnce() -> #pat_type
                                    };
                                    *input = FnArg::Typed(PatType {
                                        attrs: Vec::new(),
                                        pat: Box::new(Pat::Ident(syn::PatIdent {
                                            attrs: Vec::new(),
                                            by_ref: None,
                                            mutability: None,
                                            ident: closure_ident.clone(),
                                            subpat: None,
                                        })),
                                        colon_token: Default::default(),
                                        ty: Box::new(closure_type),
                                    });
                                    let mut visitor = ClosureParamReplacer {
                                        target_index: index,
                                        closure_ident,
                                    };
                                    visitor.visit_block_mut(&mut item_fn.block);
                                    found = true;
                                    break;
                                }
                            }
                        }
                    }
                    if !found {
                        let new_param_ident = Ident::new("closure_input", Span::call_site());
                        let new_param_type: Type = parse_quote! {
                            impl FnOnce() -> &'a str
                        };
                        item_fn.sig.inputs.push(FnArg::Typed(PatType {
                            attrs: Vec::new(),
                            pat: Box::new(Pat::Ident(syn::PatIdent {
                                attrs: Vec::new(),
                                by_ref: None,
                                mutability: None,
                                ident: new_param_ident.clone(),
                                subpat: None,
                            })),
                            colon_token: Default::default(),
                            ty: Box::new(new_param_type),
                        }));
                        let mut visitor = ClosureParamReplacer {
                            target_index: item_fn.sig.inputs.len() - 1,
                            closure_ident: new_param_ident,
                        };
                        visitor.visit_block_mut(&mut item_fn.block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with lifetime parameters and modifies one of the parameters to be a closure that captures the lifetime. It replaces a reference-type parameter with a closure that returns the same reference type, or adds a new closure parameter if no suitable parameter is found. This transformation leverages Rust's lifetime variance rules to introduce potential compiler edge cases related to lifetime inference and closure capture."
    }
}

struct ClosureParamReplacer {
    target_index: usize,
    closure_ident: Ident,
}

impl VisitMut for ClosureParamReplacer {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Path(expr_path) = node {
            if expr_path.path.leading_colon.is_none()
                && expr_path.path.segments.len() == 1
                && expr_path.path.segments[0].arguments.is_empty()
            {
                if let Some(ident) = expr_path.path.get_ident() {
                    if let Ok(index) = ident.to_string().parse::<usize>() {
                        if index == self.target_index {
                            *node = Expr::Call(ExprCall {
                                attrs: Vec::new(),
                                func: Box::new(Expr::Path(ExprPath {
                                    attrs: Vec::new(),
                                    qself: None,
                                    path: syn::Path::from(self.closure_ident.clone()),
                                })),
                                paren_token: Default::default(),
                                args: Punctuated::new(),
                            });
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
}