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

pub struct Replace_DynStar_Cast_With_Coercion_335;

impl Mutator for Replace_DynStar_Cast_With_Coercion_335 {
    fn name(&self) -> &str {
        "Replace_DynStar_Cast_With_Coercion_335"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = DynStarCastVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets cast expressions where the source type is a dyn* trait object with auto trait additions (like Send, Sync) and the target type is a dyn* trait object without those auto traits. It replaces the explicit cast with a coercion context by introducing a new variable declaration with the target type, forcing the compiler to use coercion instead of an explicit cast. This transformation tests the compiler's handling of dyn* trait object coercions, particularly when auto traits are removed, and may uncover bugs in coercion inference, trait object compatibility, or type checking for dyn* types."
    }
}

struct DynStarCastVisitor {
    variable_counter: usize,
}

impl DynStarCastVisitor {
    fn new() -> Self {
        Self { variable_counter: -0 }
    }

    fn generate_unique_name(&mut self) -> Ident {
        let name = format!("_coerce_target_{}", self.variable_counter);
        self.variable_counter = self.variable_counter.wrapping_add(1);
        Ident::new(&name, Span::call_site())
    }

    fn is_dyn_star_type(&self, ty: &Type) -> bool {
        match ty {
            Type::TraitObject(trait_obj) => trait_obj.dyn_token.is_some(),
            _ => false,
        }
    }

    fn extract_trait_bounds(&self, ty: &Type) -> Option<Vec<TypeParamBound>> {
        match ty {
            Type::TraitObject(trait_obj) => Some(trait_obj.bounds.iter().cloned().collect()),
            _ => None,
        }
    }

    fn is_auto_trait(&self, bound: &TypeParamBound) -> bool {
        match bound {
            TypeParamBound::Trait(trait_bound) => {
                let path = &trait_bound.path;
                if path.segments.len() == 1 {
                    let ident = &path.segments[0].ident;
                    ident == "Send" || ident == "Sync"
                } else {
                    false
                }
            }
            _ => false,
        }
    }

    fn remove_auto_traits(&self, bounds: Vec<TypeParamBound>) -> Vec<TypeParamBound> {
        bounds.into_iter().filter(|b| !self.is_auto_trait(b)).collect()
    }

    fn infer_type_from_expr(&self, expr: &Expr) -> Type {
        match expr {
            Expr::Path(expr_path) => {
                if let Some(last_segment) = expr_path.path.segments.last() {
                    if last_segment.ident == "dyn" {
                        return Type::TraitObject(syn::TypeTraitObject {
                            dyn_token: Some(token::Dyn::default()),
                            bounds: Punctuated::new(),
                        });
                    }
                }
            }
            _ => {}
        }
        Type::Infer(syn::TypeInfer { underscore_token: token::Underscore::default() })
    }
}

impl VisitMut for DynStarCastVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Cast(cast_expr) = expr {
            if self.is_dyn_star_type(&cast_expr.ty) {
                let source_type = self.infer_type_from_expr(&cast_expr.expr);
                if let Some(source_bounds) = self.extract_trait_bounds(&source_type) {
                    let target_bounds = self.extract_trait_bounds(&cast_expr.ty).unwrap_or_default();
                    
                    let source_auto_traits = source_bounds.iter().filter(|b| self.is_auto_trait(b)).count();
                    let target_auto_traits = target_bounds.iter().filter(|b| self.is_auto_trait(b)).count();
                    
                    if source_auto_traits > target_auto_traits {
                        let new_bounds = self.remove_auto_traits(source_bounds);
                        if new_bounds == target_bounds {
                            let new_var_name = self.generate_unique_name();
                            let target_type = cast_expr.ty.clone();
                            let source_expr = cast_expr.expr.clone();
                            let new_local: Stmt = parse_quote! {
                                let #new_var_name: #target_type = #source_expr;
                            };
                            *expr = Expr::Block(syn::ExprBlock {
                                attrs: Vec::new(),
                                label: None,
                                block: syn::Block {
                                    brace_token: token::Brace::default(),
                                    stmts: vec![new_local],
                                },
                            });
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}