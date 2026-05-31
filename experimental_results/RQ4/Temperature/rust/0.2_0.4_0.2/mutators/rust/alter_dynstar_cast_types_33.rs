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

pub struct Alter_DynStar_Cast_Types_33;

impl Mutator for Alter_DynStar_Cast_Types_33 {
    fn name(&self) -> &str {
        "Alter_DynStar_Cast_Types_33"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Cast(expr_cast) = expr {
                            if let Type::TraitObject(type_trait_object) = &*expr_cast.ty {
                                if let Some(TypeParamBound::Trait(trait_bound)) = type_trait_object.bounds.first() {
                                    if trait_bound.path.is_ident("Debug") {
                                        expr_cast.ty = Box::new(parse_quote!(dyn* Display));
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
        "This mutator targets `dyn*` cast operations, specifically altering the target type in the cast. By changing the type from `dyn* Debug` to `dyn* Display`, it tests the compiler's handling of dynamic trait objects and exposes potential issues in type casting and trait resolution."
    }
}