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
    visit_mut::{self, VisitMut},
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Generic_Pattern_With_Literal_241;

impl Mutator for Replace_Generic_Pattern_With_Literal_241 {
    fn name(&self) -> &str {
        "Replace_Generic_Pattern_With_Literal_241"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct PatternMutator {
            rng: rand::prelude::ThreadRng,
        }

        impl VisitMut for PatternMutator {
            fn visit_pat_mut(&mut self, i: &mut Pat) {
                if let Pat::TupleStruct(pat) = i {
                    if let Some(segment) = pat.path.segments.iter().next() {
                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                            if !args.args.is_empty() {
                                let lit = self.rng.gen_range(0..2);
                                let replacement = if lit == 0 {
                                    parse_quote! { 123 }
                                } else {
                                    parse_quote! { "literal" }
                                };
                                if let Expr::Lit(expr_lit) = *replacement {
                                    *i = Pat::Lit(syn::PatLit {
                                        attrs: vec![],
                                        lit: Box::new(expr_lit.lit),
                                    });
                                }
                            }
                        }
                    }
                }
                self.visit_pat_mut(i);
            }
        }

        let mut visitor = PatternMutator {
            rng: thread_rng(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}