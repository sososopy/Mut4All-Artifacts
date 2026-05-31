use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStatic, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Transform_Static_To_Mutable_Static_30;

impl Mutator for Transform_Static_To_Mutable_Static_30 {
    fn name(&self) -> &str {
        "Transform_Static_To_Mutable_Static_30"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if matches!(item_static.mutability, StaticMutability::None) {
                    item_static.mutability = StaticMutability::Mut(token::Mut::default());

                    let ident = &item_static.ident;
                    let mut usages = Vec::new();

                    struct FindUsages<'a> {
                        ident: &'a Ident,
                        usages: &'a mut Vec<Expr>,
                    }

                    impl<'a> Visit<'_> for FindUsages<'a> {
                        fn visit_expr(&mut self, expr: &Expr) {
                            if let Expr::Path(ExprPath { path, .. }) = expr {
                                if path.is_ident(self.ident) {
                                    self.usages.push(expr.clone());
                                }
                            }
                            syn::visit::visit_expr(self, expr);
                        }
                    }

                    let mut finder = FindUsages { ident, usages: &mut usages };
                    finder.visit_file(file);

                    for usage in usages {
                        let unsafe_block: Expr = parse_quote! {
                            unsafe { #usage }
                        };
                        *usage = unsafe_block.clone();
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static variables that are initialized as immutable. It transforms them into mutable static variables by changing the static keyword to static mut. This requires ensuring semantic correctness by wrapping usages of the static variable in an unsafe block, allowing for mutable modifications. This mutation can reveal issues in constant evaluation and unsafe code handling."
    }
}