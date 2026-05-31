use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, TypeTraitObject,
};

use crate::mutator::Mutator;

pub struct Substitute_DynStar_With_TraitObject_33;

impl Mutator for Substitute_DynStar_With_TraitObject_33 {
    fn name(&self) -> &str {
        "Substitute_DynStar_With_TraitObject_33"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::TraitObject(ref trait_object) = **ty {
                        if trait_object.dyn_token.is_some() {
                            let new_ty: Type = parse_quote!(Box<dyn #trait_object>);
                            *ty = Box::new(new_ty);
                            let mut new_block = func.block.clone();
                            for stmt in &mut new_block.stmts {
                                if let Stmt::Expr(expr) = stmt {
                                    *expr = parse_quote!(Box::new(#expr));
                                }
                            }
                            func.block = Box::new(new_block);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}