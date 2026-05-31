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

pub struct Replace_Array_Length_With_Max_70;

impl Mutator for Replace_Array_Length_With_Max_70 {
    fn name(&self) -> &str {
        "Replace_Array_Length_With_Max_70"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ArrayLengthVisitor {
            mutator: self,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array type definitions, replacing the length expression with usize::MAX >> 2, a large constant that exceeds typical architectural address space limits. This stresses the const evaluator's handling of oversized arrays in constant contexts, potentially triggering overflow or allocation failures similar to the reported ICE."
    }
}

struct ArrayLengthVisitor<'a> {
    mutator: &'a Replace_Array_Length_With_Max_70,
}

impl<'a> VisitMut for ArrayLengthVisitor<'a> {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Array(type_array) = node {
            let new_length: Expr = parse_quote! { usize::MAX >> 2 };
            type_array.len = Box::new(new_length);
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}