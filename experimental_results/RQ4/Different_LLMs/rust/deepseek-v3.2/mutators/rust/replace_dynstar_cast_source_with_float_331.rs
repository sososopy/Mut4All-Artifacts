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

pub struct Replace_DynStar_Cast_Source_With_Float_331;

impl Mutator for Replace_DynStar_Cast_Source_With_Float_331 {
    fn name(&self) -> &str {
        "Replace_DynStar_Cast_Source_With_Float_331"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = DynStarCastVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets cast expressions with dyn-star trait object target types (dyn* Trait). It replaces the source expression with a floating-point literal (f32 or f64), which is syntactically valid but semantically incompatible for a dyn-star cast. This transformation aims to trigger unexpected type handling in the compiler, testing its error recovery and type checking for invalid dyn-star casts with primitive numeric source types."
    }
}

struct DynStarCastVisitor;

impl DynStarCastVisitor {
    fn new() -> Self {
        Self
    }
}

impl VisitMut for DynStarCastVisitor {
    fn visit_expr_cast_mut(&mut self, cast: &mut ExprCast) {
        if let Type::TraitObject(trait_obj) = &*cast.ty {
            if trait_obj.dyn_token.is_some() {
                let mut segments = trait_obj.bounds.iter().filter_map(|bound| {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        Some(&trait_bound.path.segments)
                    } else {
                        None
                    }
                }).flatten();
                if let Some(first_seg) = segments.next() {
                    if first_seg.ident == "dyn" {
                        let mut rng = thread_rng();
                        let float_lit = if rng.gen_bool(0.5) {
                            parse_quote! { 1.0f64 }
                        } else {
                            parse_quote! { 2.0f32 }
                        };
                        cast.expr = Box::new(float_lit);
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_cast_mut(self, cast);
    }
}