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

pub struct Insert_Const_Block_In_Array_Length_424;

impl Mutator for Insert_Const_Block_In_Array_Length_424 {
    fn name(&self) -> &str {
        "Insert_Const_Block_In_Array_Length_424"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ArrayLengthConstInserter;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array type definitions where the length is a constant expression, especially in generic contexts. It wraps the length expression N in a const block, transforming [T; N] into [T; const { N }]. This mutation aims to expose bugs in const evaluation with generics and inline consts, particularly triggering edge cases like ICEs when evaluating const blocks in array lengths. It applies to structs, unions, function signatures, and other items containing array types, but only when N is not already a const block."
    }
}

struct ArrayLengthConstInserter;

impl VisitMut for ArrayLengthConstInserter {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Array(arr) = ty {
            if let Expr::Const(_) = &arr.len {
                return;
            }
            if let Expr::Block(_) = &arr.len {
                return;
            }
            let len_expr = std::mem::replace(&mut arr.len, parse_quote!(0));
            arr.len = parse_quote!({ const { #len_expr } });
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}