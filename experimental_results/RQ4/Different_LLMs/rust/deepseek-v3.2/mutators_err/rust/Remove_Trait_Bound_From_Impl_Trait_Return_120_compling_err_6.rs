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

pub struct Remove_Trait_Bound_From_Impl_Trait_Return_120;

impl Mutator for Remove_Trait_Bound_From_Impl_Trait_Return_120 {
    fn name(&self) -> &str {
        "Remove_Trait_Bound_From_Impl_Trait_Return_120"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut visitor = YieldVisitor { found_yield: false };
                        visitor.visit_block(&item_fn.block);
                        if visitor.found_yield {
                            type_impl_trait.bounds = Punctuated::new();
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with an `impl Trait` return type that contain a yield expression. It removes the trait bound from the impl Trait return type, leaving only the `impl` keyword. This creates an incomplete impl Trait syntax. The function body, including the yield expression, remains unchanged. This mutation targets the inconsistency between the generator's yield type inference and the opaque type's trait constraints, potentially triggering internal compiler errors in the borrow checker or type checker related to coroutines and impl Trait."
    }
}

struct YieldVisitor {
    found_yield: bool,
}

impl Visit<'_> for YieldVisitor {
    fn visit_expr(&mut self, expr: &Expr) {
        if let Expr::Yield(_) = expr {
            self.found_yield = true;
        }
        syn::visit::visit_expr(self, expr);
    }
}