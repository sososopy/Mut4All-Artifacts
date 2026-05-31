use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Mutate_Array_Indexing_To_OutOfBounds_219;

impl Mutator for Mutate_Array_Indexing_To_OutOfBounds_219 {
    fn name(&self) -> &str {
        "Mutate_Array_Indexing_To_OutOfBounds_219"
    }
    fn mutate(&self, file: &mut syn::File) {
        fn generate_out_of_bounds_expr(array_ident: &Ident) -> Expr {
            let mut rng = thread_rng();
            let choice = rng.gen_range(0..2);
            match choice {
                0 => parse_quote! { #array_ident[#array_ident.len()] },
                1 => {
                    let constant = rng.gen_range(1..=5);
                    parse_quote! { #array_ident[#array_ident.len() + #constant] }
                }
                _ => unreachable!(),
            }
        }

        struct ArrayIndexVisitor<'a> {
            array_names: &'a HashSet<String>,
            replacements: Vec<(Span, Expr)>,
        }

        impl<'a> Visit<'a> for ArrayIndexVisitor<'a> {
            fn visit_expr(&mut self, node: &'a Expr) {
                if let Expr::Index(expr_index) = node {
                    if let Expr::Path(ExprPath { path, .. }) = &*expr_index.expr {
                        if let Some(ident) = path.get_ident() {
                            if self.array_names.contains(&ident.to_string()) {
                                let new_expr = generate_out_of_bounds_expr(ident);
                                self.replacements.push((node.span(), new_expr));
                            }
                        }
                    }
                }
                syn::visit::visit_expr(self, node);
            }
        }

        struct ArrayIndexMutator<'a> {
            replacements: &'a [(Span, Expr)],
        }

        impl VisitMut for ArrayIndexMutator<'_> {
            fn visit_expr_mut(&mut self, node: &mut Expr) {
                for (span, new_expr) in self.replacements {
                    if node.span().start() == span.start() {
                        *node = new_expr.clone();
                        return;
                    }
                }
                syn::visit_mut::visit_expr_mut(self, node);
            }
        }

        let mut array_names = HashSet::new();
        for item in &file.items {
            if let Item::Local(Local {
                pat: Pat::Ident(pat_ident),
                init: Some(LocalInit { expr: init_expr, .. }),
                ..
            }) = item
            {
                if let Expr::Array(_) = &**init_expr {
                    array_names.insert(pat_ident.ident.to_string());
                }
            }
        }

        let mut visitor = ArrayIndexVisitor {
            array_names: &array_names,
            replacements: Vec::new(),
        };
        visitor.visit_file(file);

        let mut mutator = ArrayIndexMutator {
            replacements: &visitor.replacements,
        };
        mutator.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets array or slice indexing operations and modifies them to potentially cause out-of-bounds access by using expressions like `arr[arr.len()]` or `arr[arr.len() + constant]`. It aims to provoke compiler errors or runtime panics related to bounds checking, testing the compiler's robustness in handling invalid index expressions."
    }
}