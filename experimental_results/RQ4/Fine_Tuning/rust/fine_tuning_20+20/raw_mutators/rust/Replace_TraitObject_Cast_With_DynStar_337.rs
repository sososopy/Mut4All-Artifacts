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

pub struct Replace_TraitObject_Cast_With_DynStar_337;

impl Mutator for Replace_TraitObject_Cast_With_DynStar_337 {
    fn name(&self) -> &str {
        "Replace_TraitObject_Cast_With_DynStar_337"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = TraitObjectCastReplacer;
                visitor.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions that perform type casting involving trait objects. It specifically replaces casts to or from `&dyn Trait` with a cast using the `dyn* Trait` syntax, which is known to be problematic. By introducing this specific casting transformation, the mutator aims to trigger compiler bugs related to the `dyn_star` feature and increase the likelihood of encountering internal compiler errors."
    }
}

struct TraitObjectCastReplacer;

impl VisitMut for TraitObjectCastReplacer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::As(expr_as) = expr {
            if let Type::TraitObject(type_trait_object) = &*expr_as.ty {
                if type_trait_object
                    .bounds
                    .iter()
                    .any(|bound| matches!(bound, TypeParamBound::Trait(_)))
                {
                    if let Expr::Unary(expr_unary) = &*expr_as.expr {
                        if let UnOp::Deref(_) = expr_unary.op {
                            if let Expr::Reference(expr_reference) = &*expr_unary.expr {
                                if let Type::TraitObject(type_trait_object_inner) =
                                    &*expr_reference.elem
                                {
                                    if type_trait_object_inner
                                        .bounds
                                        .iter()
                                        .any(|bound| matches!(bound, TypeParamBound::Trait(_)))
                                    {
                                        *expr = Expr::As(ExprAs {
                                            attrs: Vec::new(),
                                            expr: expr_unary.expr.clone(),
                                            as_token: Default::default(),
                                            ty: Box::new(Type::Verbatim(quote! { dyn* #type_trait_object })),
                                        });
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}