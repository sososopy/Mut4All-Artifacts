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

pub struct Introduce_DynStar_Cast_In_ImplTrait_Return_330;

impl Mutator for Introduce_DynStar_Cast_In_ImplTrait_Return_330 {
    fn name(&self) -> &str {
        "Introduce_DynStar_Cast_In_ImplTrait_Return_330"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                // Check if function has lifetime parameters
                let has_lifetime_param = item_fn.sig.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Lifetime(_))
                });
                
                if !has_lifetime_param {
                    continue;
                }
                
                // Check if return type is impl Trait
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        // Check if impl Trait has at least one trait bound
                        if type_impl_trait.bounds.is_empty() {
                            continue;
                        }
                        
                        // Get the first trait bound
                        let first_trait_bound = type_impl_trait.bounds.iter().find_map(|bound| {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                Some(trait_bound)
                            } else {
                                None
                            }
                        });
                        
                        if let Some(trait_bound) = first_trait_bound {
                            // Find expressions in the function body that could be cast
                            let mut visitor = ExprVisitor {
                                target_trait_path: trait_bound.path.clone(),
                                expressions_to_modify: Vec::new(),
                            };
                            visitor.visit_block_mut(&mut item_fn.block);
                            
                            // Modify the first suitable expression
                            if let Some(expr_span) = visitor.expressions_to_modify.first() {
                                let mut expr_modifier = ExprModifier {
                                    target_trait_path: trait_bound.path.clone(),
                                    target_span: *expr_span,
                                    modified: false,
                                };
                                expr_modifier.visit_block_mut(&mut item_fn.block);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with lifetime parameters that return `impl Trait` with trait bounds. It identifies expressions within the function body that implement the return trait and modifies them by adding a `dyn*` cast. This transformation introduces dynamic trait objects with explicit `dyn*` syntax, potentially triggering compiler bugs related to `dyn*` handling, lifetime inference, and incremental compilation. The mutation preserves the original expression's semantics while adding a cast that may expose edge cases in the compiler's trait object resolution."
    }
}

struct ExprVisitor {
    target_trait_path: SynPath,
    expressions_to_modify: Vec<proc_macro2::Span>,
}

impl VisitMut for ExprVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        // Check if this expression could be a candidate for casting
        // We look for expressions that are simple enough to cast
        match expr {
            Expr::Path(_) | Expr::Call(_) | Expr::MethodCall(_) | 
            Expr::Field(_) | Expr::Index(_) | Expr::Paren(_) => {
                self.expressions_to_modify.push(expr.span());
            }
            _ => {}
        }
        
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

struct ExprModifier {
    target_trait_path: Syn2Path,
    target_span: proc_macro2::Span,
    modified: bool,
}

impl VisitMut for ExprModifier {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if !self.modified && expr.span().into() == self.target_span.into() {
            // Create the dyn* cast expression
            let original_expr = expr.clone();
            let trait_path = self.target_trait_path.clone();
            
            *expr = parse_quote! {
                #original_expr as dyn* #trait_path
            };
            
            self.modified = true;
            return;
        }
        
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}