use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprArray, ExprIndex, ExprLit, ExprPath, File, FnArg, GenericArgument, 
    GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Lit, Pat, 
    PatType, Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, 
    Type, TypeImplTrait, TypeParamBound, TypePath, parse_quote, punctuated::Punctuated, 
    spanned::Spanned, token, token::Comma, token::{Paren, Plus}, visit::Visit, visit_mut::VisitMut, 
    *,
};

use crate::mutator::Mutator;

pub struct Array_Index_Out_Of_Bounds_341;

impl Mutator for Array_Index_Out_Of_Bounds_341 {
    fn name(&self) -> &str {
        "Array_Index_Out_Of_Bounds_341"
    }

    fn mutate(&self, file: &mut syn::File) {
        struct IndexMutator;
        
        impl VisitMut for IndexMutator {
            fn visit_expr_index_mut(&mut self, expr_index: &mut ExprIndex) {
                if let Expr::Path(ExprPath { path, .. }) = &*expr_index.expr {
                    if let Some(segment) = path.segments.last() {
                        if segment.ident == "len" {
                            return;
                        }
                    }
                }

                let new_index = parse_quote! { #expr_index.expr.len() };
                expr_index.index = Box::new(new_index);
            }
        }

        let mut index_mutator = IndexMutator;
        index_mutator.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets array or slice indexing operations. It modifies the index to potentially cause out-of-bounds access by changing it to use the length of the array or slice, which can lead to compiler errors or unexpected behavior."
    }
}