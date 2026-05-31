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

pub struct Replace_Const_Function_Call_In_Trait_Impl_360;

impl Mutator for Replace_Const_Function_Call_In_Trait_Impl_360 {
    fn name(&self) -> &str {
        "Replace_Const_Function_Call_In_Trait_Impl_360"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ConstFunctionCollector {
            const_functions: HashSet<String>,
        }

        impl<'ast> Visit<'ast> for ConstFunctionCollector {
            fn visit_item_fn(&mut self, item_fn: &'ast ItemFn) {
                if item_fn.sig.constness.is_some() {
                    self.const_functions
                        .insert(item_fn.sig.ident.to_string());
                }
                syn::visit::visit_item_fn(self, item_fn);
            }
        }

        let mut collector = ConstFunctionCollector {
            const_functions: HashSet::new(),
        };

        collector.visit_file(file);

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &mut item_impl.trait_ {
                    if let Some(last_segment) = path.segments.last_mut() {
                        if let PathArguments::AngleBracketed(angle_bracketed) =
                            &mut last_segment.arguments
                        {
                            for arg in &mut angle_bracketed.args {
                                if let GenericArgument::Const(expr) = arg {
                                    if let Expr::Call(expr_call) = expr {
                                        if let Expr::Path(expr_path) = &*expr_call.func {
                                            if let Some(last_segment) =
                                                expr_path.path.segments.last()
                                            {
                                                if collector
                                                    .const_functions
                                                    .contains(&last_segment.ident.to_string())
                                                {
                                                    *arg = GenericArgument::Const(parse_quote! {
                                                        100
                                                    });
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations that use const functions to determine constant generic parameters. It identifies const function calls within the trait's angle-bracketed arguments and replaces them with a direct constant value (e.g., 100). This transformation disrupts the expected behavior of the trait implementation, potentially leading to type mismatches or logical inconsistencies, thereby testing the compiler's robustness in handling const generics and trait resolution."
    }
}