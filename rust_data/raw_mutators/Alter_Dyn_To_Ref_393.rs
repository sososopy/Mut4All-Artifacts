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

pub struct Alter_Dyn_To_Ref_393;

impl Mutator for Alter_Dyn_To_Ref_393 {
    fn name(&self) -> &str {
        "Alter_Dyn_To_Ref_393"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct DynToRefVisitor;

        impl VisitMut for DynToRefVisitor {
            fn visit_fn_arg_mut(&mut self, arg: &mut FnArg) {
                if let FnArg::Typed(pat_type) = arg {
                    if let Type::TraitObject(trait_obj) = &*pat_type.ty {
                        if trait_obj.bounds.iter().any(|bound| matches!(bound, TypeParamBound::Trait(_))) {
                            pat_type.ty = Box::new(Type::Reference(syn::TypeReference {
                                and_token: token::And { spans: [Span::call_site()] },
                                lifetime: None,
                                mutability: None,
                                elem: pat_type.ty.clone(),
                            }));
                        }
                    }
                }
                syn::visit_mut::visit_fn_arg_mut(self, arg);
            }

            fn visit_expr_call_mut(&mut self, expr_call: &mut ExprCall) {
                for arg in &mut expr_call.args {
                    if let Expr::Path(expr_path) = arg {
                        if expr_path.path.segments.last().map_or(false, |segment| segment.ident == "Union") {
                            *arg = parse_quote!(&#arg);
                        }
                    }
                }
                syn::visit_mut::visit_expr_call_mut(self, expr_call);
            }
        }

        let mut visitor = DynToRefVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function parameters and assignments using `dyn* Trait` and alters them to `&dyn Trait`. This involves modifying function signatures to accept trait object references and adjusting function calls to pass references. The goal is to explore how changing from `dyn*` to `&dyn` affects trait object handling in the compiler."
    }
}