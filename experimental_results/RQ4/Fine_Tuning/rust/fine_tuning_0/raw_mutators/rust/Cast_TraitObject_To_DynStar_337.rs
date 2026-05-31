use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Cast_TraitObject_To_DynStar_337;

impl Mutator for Cast_TraitObject_To_DynStar_337 {
    fn name(&self) -> &str {
        "Cast_TraitObject_To_DynStar_337"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &local.init {
                            if let Expr::Cast(cast_expr) = expr.as_ref() {
                                if let Type::TraitObject(trait_obj) = &*cast_expr.ty {
                                    if trait_obj.bounds.iter().any(|bound| {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            trait_bound.path.segments.last().unwrap().ident == "MyTrait"
                                        } else {
                                            false
                                        }
                                    }) {
                                        let new_expr: Expr = parse_quote! {
                                            *#cast_expr.expr as dyn* MyTrait
                                        };
                                        *stmt = Stmt::Expr(new_expr);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies type casting operations involving trait objects and modifies them to use the `dyn_star` feature, specifically casting a trait object reference (`&dyn Trait`) to a `dyn* Trait`. This transformation is designed to trigger potential compiler issues related to the handling of `dyn_star` casts."
    }
}