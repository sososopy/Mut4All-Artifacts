use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCast, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct CoerceCastDifferentAutoTrait_335;

impl Mutator for CoerceCastDifferentAutoTrait_335 {
    fn name(&self) -> &str {
        "CoerceCastDifferentAutoTrait_335"
    }
    fn mutate(&self, file: &mut syn::File) {
        CoerceCastVisitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct CoerceCastVisitor;

impl VisitMut for CoerceCastVisitor {
    fn visit_expr(&mut self, expr: &mut Expr) {
        if let Expr::Cast(expr_cast) = expr {
            if let Type::Path(type_path) = &*expr_cast.ty {
                if let Some(segment) = type_path.path.segments.first() {
                    if segment.ident == "dyn" {
                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                            let mut bounds = Punctuated::new();
                            for arg in &args.args {
                                if let GenericArgument::Type(ty) = arg {
                                    if let Type::Path(ty_path) = &*ty {
                                        let path = ty_path.path.clone();
                                        bounds.push(TypeParamBound::Trait(TraitBound {
                                            path,
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                        }));
                                    }
                                }
                            }
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                path: SynPath::from(Ident::new("Sync", Span::call_site())),
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                            }));
                            let new_ty = Type::TraitObject(syn::TypeTraitObject {
                                dyn_token: Some(token::Dyn::default()),
                                bounds,
                                ..Default::default()
                            });
                            *expr_cast.ty = Box::new(new_ty);
                        }
                    }
                }
            }
        }
        visit_expr(self, expr);
    }
}