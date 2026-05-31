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

pub struct Mutator_Type_Cast_Dyn_398;

impl Mutator for Mutator_Type_Cast_Dyn_398 {
    fn name(&self) -> &str {
        "Mutator_Type_Cast_Dyn_398"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TypeCastVisitor;

        impl VisitMut for TypeCastVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Cast(expr_cast) = expr {
                    if let Type::TraitObject(trait_obj) = &*expr_cast.ty {
                        if let Expr::Reference(expr_ref) = &*expr_cast.expr {
                            if let Type::TraitObject(_) = &*expr_cast.ty {
                                let new_expr: Expr = parse_quote! {
                                    *#expr_ref.expr as dyn* #trait_obj
                                };
                                *expr = new_expr;
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut visitor = TypeCastVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with type casting involving trait objects. It replaces safe casting operations with a potentially buggy scenario by introducing a dereference and casting to a `dyn* Trait`, leveraging the `dyn_star` feature."
    }
}