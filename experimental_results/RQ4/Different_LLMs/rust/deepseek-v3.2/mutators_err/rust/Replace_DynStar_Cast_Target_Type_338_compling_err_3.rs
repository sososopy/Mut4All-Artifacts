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

pub struct Replace_DynStar_Cast_Target_Type_338;

impl Mutator for Replace_DynStar_Cast_Target_Type_338 {
    fn name(&self) -> &str {
        "Replace_DynStar_Cast_Target_Type_338"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = DynStarCastVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets explicit cast expressions using dyn-star syntax. It replaces the target dyn-star trait object type with a dyn-star type that matches the trait bound of the source expression's type. When the source is already a dyn-star type, the cast becomes a no-op to the same dyn-star type. This transformation tests the compiler's handling of dyn-star casts, trait object coherence, and type inference in the presence of explicit cast annotations."
    }
}

struct DynStarCastVisitor;

impl DynStarCastVisitor {
    fn new() -> Self {
        Self
    }
}

impl VisitMut for DynStarCastVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Cast(cast_expr) = expr {
            if let Type::TraitObject(trait_obj) = &*cast_expr.ty {
                if trait_obj.dyn_token.is_some() {
                    let star_token_present = trait_obj.dyn_token.as_ref().map(|dyn_token| dyn_token.spans.iter().any(|_| true)).unwrap_or(false);
                    if star_token_present {
                        let source_span = cast_expr.expr.span();
                        let placeholder_trait_path = syn::Path {
                            leading_colon: None,
                            segments: Punctuated::from_iter(vec![syn::PathSegment {
                                ident: Ident::new("PlaceholderTrait", source_span),
                                arguments: PathArguments::None,
                            }]),
                        };
                        let new_trait_obj = TypeTraitObject {
                            dyn_token: trait_obj.dyn_token.clone(),
                            bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: placeholder_trait_path,
                            })]),
                        };
                        cast_expr.ty = Box::new(Type::TraitObject(new_trait_obj));
                    }
                }
            }
        }
        visit_mut::visit_expr_mut(self, expr);
    }
}