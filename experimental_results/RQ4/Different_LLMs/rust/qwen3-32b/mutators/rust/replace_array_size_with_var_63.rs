use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprArray, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Array_Size_With_Var_63;

impl Mutator for Replace_Array_Size_With_Var_63 {
    fn name(&self) -> &str {
        "Replace_Array_Size_With_Var_63"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    struct ArraySizeReplacer;
                    impl VisitMut for ArraySizeReplacer {
                        fn visit_expr_array_mut(&mut self, array: &mut ExprArray) {
                            if array.elems.len() == 2 {
                                let second = array.elems.iter_mut().nth(1).unwrap();
                                *second = parse_quote! { W + 1 };
                            }
                            syn::visit_mut::visit_expr_array_mut(self, array);
                        }
                    }
                    let mut visitor = ArraySizeReplacer;
                    visitor.visit_block_mut(&mut func.block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces fixed-size array initializations in const functions with computed sizes using an expression like W + 1. This introduces potential unsized or invalid array sizes during const evaluation, testing the compiler's handling of dynamic array sizing in compile-time contexts and possibly triggering layout assertion failures or ICEs."
    }
}