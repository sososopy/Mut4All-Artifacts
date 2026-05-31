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

pub struct Mutator_Array_Index_Out_Of_Bounds_340;

impl Mutator for Mutator_Array_Index_Out_Of_Bounds_340 {
    fn name(&self) -> &str {
        "Mutator_Array_Index_Out_Of_Bounds_340"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ArrayIndexMutator;
        
        impl VisitMut for ArrayIndexMutator {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Index(expr_index) = expr {
                    if let Expr::Path(ref array_path) = *expr_index.expr {
                        if let Some(array_ident) = array_path.path.get_ident() {
                            if let Expr::Lit(ref mut index_lit) = *expr_index.index {
                                if let syn::Lit::Int(ref mut lit_int) = index_lit.lit {
                                    if let Ok(index_value) = lit_int.base10_parse::<usize>() {
                                        let new_index = index_value + 2; // Intentionally out-of-bounds
                                        *lit_int = syn::LitInt::new(&new_index.to_string(), lit_int.span());
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut visitor = ArrayIndexMutator;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array indexing in constant definitions. It identifies array indexing operations and modifies the index to exceed the array bounds, creating an out-of-bounds access. This mutation is useful for triggering ICEs during constant evaluation in the MIR optimization phase."
    }
}