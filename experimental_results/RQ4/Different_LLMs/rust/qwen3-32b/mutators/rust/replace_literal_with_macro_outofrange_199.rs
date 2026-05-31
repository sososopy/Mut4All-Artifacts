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

pub struct Replace_Literal_With_Macro_OutOfRange_199;

impl Mutator for Replace_Literal_With_Macro_OutOfRange_199 {
    fn name(&self) -> &str {
        "Replace_Literal_With_Macro_OutOfRange_199"
    }

    fn mutate(&self, file: &mut syn::File) {
        // Add macro definition with synthetic span
        let macro_def: Item = parse_quote! {
            macro_rules! out_of_range {
                () => {
                    256u8
                };
            }
        };
        file.items.insert(0, macro_def);

        // Traverse and replace u8 literals with macro calls
        let mut visitor = LiteralReplacer::new();
        visitor.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces u8 literals with a macro that expands to an out-of-range value (256u8). The macro's expansion is marked with a synthetic span, making the source inaccessible to the compiler. This transformation tests the compiler's ability to detect and report out-of-range literals expanded from macros, potentially exposing issues in macro hygiene and error diagnostics."
    }
}

struct LiteralReplacer;

impl LiteralReplacer {
    fn new() -> Self {
        Self
    }
}

impl VisitMut for LiteralReplacer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Lit(expr_lit) = expr {
            if let syn::Lit::Int(int_lit) = &expr_lit.lit {
                if int_lit.suffix() == "u8" {
                    // Replace with macro call
                    *expr = parse_quote!(out_of_range!());
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}