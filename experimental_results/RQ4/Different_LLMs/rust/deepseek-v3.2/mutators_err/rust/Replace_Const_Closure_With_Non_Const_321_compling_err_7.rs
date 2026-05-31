use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprClosure, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Const_Closure_With_Non_Const_321;

impl Mutator for Replace_Const_Closure_With_Non_Const_321 {
    fn name(&self) -> &str {
        "Replace_Const_Closure_With_Non_Const_321"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ConstClosureReplacer;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const functions returning `impl ~const Trait` where the return expression is a closure. It removes the `const` modifier from closure expressions while preserving `move` keywords. This creates a mismatch between the function's const trait promise and the non-const closure, testing the compiler's const checking logic for opaque return types and closure constness validation."
    }
}

struct ConstClosureReplacer;

impl VisitMut for ConstClosureReplacer {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        let is_const_fn = i.sig.constness.is_some();
        let has_impl_const_return = match &i.sig.output {
            ReturnType::Type(_, ty) => {
                if let Type::ImplTrait(type_impl) = &**ty {
                    type_impl.bounds.iter().any(|bound| {
                        if let TypeParamBound::Trait(trait_bound) = bound {
                            trait_bound.constness.is_some()
                        } else {
                            false
                        }
                    })
                } else {
                    false
                }
            }
            _ => false,
        };

        if is_const_fn && has_impl_const_return {
            self.visit_block_mut(&mut i.block);
        }

        syn::visit_mut::visit_item_fn_mut(self, i);
    }

    fn visit_expr_closure_mut(&mut self, e: &mut ExprClosure) {
        if e.constness.is_some() {
            e.constness = None;
        }
        syn::visit_mut::visit_expr_closure_mut(self, e);
    }
}