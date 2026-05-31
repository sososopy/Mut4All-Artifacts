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

pub struct Static_Item_Field_Type_Mismatch_5;

impl Mutator for Static_Item_Field_Type_Mismatch_5 {
    fn name(&self) -> &str {
        "Static_Item_Field_Type_Mismatch_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut structs = vec![];
        let mut static_instances = vec![];

        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.fields.iter().any(|f| matches!(f.ty, Type::Path(_))) &&
                   item_struct.fields.iter().any(|f| matches!(f.ty, Type::Path(_))) {
                    structs.push(item_struct);
                }
            }
        }

        for item in &file.items {
            if let Item::Static(item_static) = item {
                if let Type::Path(TypePath { path, .. }) = &*item_static.ty {
                    if structs.iter().any(|s| s.ident == path.segments.last().unwrap().ident) {
                        static_instances.push(item_static);
                    }
                }
            }
        }

        for item_static in static_instances {
            if let Expr::Struct(expr_struct) = &mut *item_static.expr.clone() {
                let fields = &mut expr_struct.fields;
                if fields.len() >= 2 {
                    let mut rng = thread_rng();
                    let indices: Vec<usize> = (0..fields.len()).collect();
                    let mut shuffled_indices = indices.clone();
                    shuffled_indices.shuffle(&mut rng);

                    let first_index = shuffled_indices[0];
                    let second_index = shuffled_indices[1];

                    fields.swap_pairs(first_index, second_index);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static instances of structs with multiple fields of different types. It swaps the initialization values of two fields, introducing a type mismatch. This transformation is intended to test the compiler's type checking and constant evaluation mechanisms, potentially uncovering bugs related to type inference and static analysis."
    }
}

trait SwapPairs {
    fn swap_pairs(&mut self, i: usize, j: usize);
}

impl<T> SwapPairs for Punctuated<T, Comma> {
    fn swap_pairs(&mut self, i: usize, j: usize) {
        if i != j && i < self.len() && j < self.len() {
            let (a, b) = if i < j { (i, j) } else { (j, i) };
            let mut iter = self.iter_mut();
            let first = iter.nth(a).unwrap();
            let second = iter.nth(b - a - 1).unwrap();
            std::mem::swap(first, second);
        }
    }
}