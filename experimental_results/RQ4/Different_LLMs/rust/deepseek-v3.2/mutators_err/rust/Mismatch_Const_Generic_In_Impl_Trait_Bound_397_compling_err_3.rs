use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, GenericParam::Const as SynConst, Item,
    ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Mismatch_Const_Generic_In_Impl_Trait_Bound_397;

impl Mutator for Mismatch_Const_Generic_In_Impl_Trait_Bound_397 {
    fn name(&self) -> &str {
        "Mismatch_Const_Generic_In_Impl_Trait_Bound_397"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut const_params: Vec<&GenericParam> = Vec::new();
                for param in &item_impl.generics.params {
                    if let GenericParam::Const(_) = param {
                        const_params.push(param);
                    }
                }
                
                let mut visitor = ImplVisitor {
                    const_params: const_params.iter().map(|p| match p {
                        GenericParam::Const(cp) => (cp.ident.clone(), cp.ty.clone()),
                        _ => unreachable!(),
                    }).collect(),
                    rng: thread_rng(),
                };
                visitor.visit_item_impl_mut(item_impl);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets impl blocks that use const generic parameters in trait bounds. It identifies const parameters in the impl's generic parameters and attempts to replace their usage in trait bounds with a const parameter of a different type, creating a type mismatch. If no suitable alternative const parameter exists, it introduces a new const generic parameter with a different type (e.g., bool) and substitutes the original usage. This aims to trigger internal compiler errors during const combination, as seen in bug reports where const inference variables of different types were incorrectly combined."
    }
}

struct ImplVisitor {
    const_params: Vec<(Ident, Type)>,
    rng: rand::rngs::ThreadRng,
}

impl VisitMut for ImplVisitor {
    fn visit_trait_bound_mut(&mut self, i: &mut TraitBound) {
        let path = &mut i.path;
        for segment in &mut path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Const(expr) = arg {
                        if let Expr::Path(expr_path) = expr {
                            if expr_path.path.segments.len() == 1 {
                                let ident = &expr_path.path.segments[0].ident;
                                if let Some((original_ident, original_ty)) = self.const_params.iter().find(|(id, _)| id == ident) {
                                    let alternative_params: Vec<&(Ident, Type)> = self.const_params.iter()
                                        .filter(|(id, ty)| id != original_ident && !type_matches(ty, original_ty))
                                        .collect();
                                    
                                    if !alternative_params.is_empty() {
                                        let (new_ident, _) = alternative_params.choose(&mut self.rng).unwrap();
                                        expr_path.path.segments[0].ident = new_ident.clone();
                                    } else {
                                        let new_ident = Ident::new("X", Span::call_site());
                                        expr_path.path.segments[0].ident = new_ident.clone();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

fn type_matches(ty1: &Type, ty2: &Type) -> bool {
    match (ty1, ty2) {
        (Type::Path(p1), Type::Path(p2)) => {
            p1.path.segments.last().map(|s| &s.ident) == p2.path.segments.last().map(|s| &s.ident)
        }
        _ => false,
    }
}