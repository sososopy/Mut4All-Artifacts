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

pub struct Insert_Dereference_Before_DynStar_Cast_337;

impl Mutator for Insert_Dereference_Before_DynStar_Cast_337 {
    fn name(&self) -> &str {
        "Insert_Dereference_Before_DynStar_Cast_337"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = DynStarCastVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets explicit type casts (`as`) where the source expression is a reference to a `dyn Trait` and the target type is `dyn* Trait`. It inserts an explicit dereference (`*`) before the source expression, transforming e.g., `&dyn Trait as dyn* Trait` into `*(&dyn Trait) as dyn* Trait`. This mutation aims to trigger edge cases in coercion and cast handling between `dyn` and `dyn*` types, particularly around dereferencing and ownership semantics, potentially exposing internal compiler errors."
    }
}

struct DynStarCastVisitor;

impl VisitMut for DynStarCastVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Cast(cast_expr) = expr {
            if is_dyn_star_target(&cast_expr.ty) {
                if is_reference_to_dyn_trait(&cast_expr.expr) && !is_dereference(&cast_expr.expr) {
                    let deref_expr: Expr = parse_quote! { *(&cast_expr.expr) };
                    *cast_expr.expr = deref_expr;
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

fn is_dyn_star_target(ty: &Type) -> bool {
    if let Type::Path(type_path) = ty {
        if let Some(segment) = type_path.path.segments.last() {
            if segment.ident.to_string().starts_with("dyn*") {
                return true;
            }
        }
    }
    false
}

fn is_reference_to_dyn_trait(expr: &Expr) -> bool {
    match expr {
        Expr::Reference(ref_expr) => {
            if let Type::Path(type_path) = &*ref_expr.expr {
                if let Some(segment) = type_path.path.segments.last() {
                    if segment.ident.to_string().starts_with("dyn") {
                        return true;
                    }
                }
            }
            false
        }
        Expr::Path(path_expr) => {
            if let Some(segment) = path_expr.path.segments.last() {
                if segment.ident.to_string().starts_with("dyn") {
                    return true;
                }
            }
            false
        }
        _ => false,
    }
}

fn is_dereference(expr: &Expr) -> bool {
    matches!(expr, Expr::Unary(unary) if matches!(unary.op, UnOp::Deref(_)))
}