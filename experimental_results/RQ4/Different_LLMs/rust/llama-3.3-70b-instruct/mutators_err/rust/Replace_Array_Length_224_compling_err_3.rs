use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Array_Length_224;

impl Mutator for Replace_Array_Length_224 {
    fn name(&self) -> &str {
        "Replace_Array_Length_224"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let Type::Array(array_type) = &field.ty {
                        let new_length = thread_rng().gen_range(1..10);
                        field.ty = Type::Array(TypeArray {
                            bracket_token: array_type.bracket_token,
                            elem: array_type.elem,
                            semi_token: array_type.semi_token,
                            len: Expr::Lit(ExprLit {
                                attrs: Default::default(),
                                lit: Lit::Int(LitInt {
                                    base: 10,
                                    suffix: Default::default(),
                                    span: Span::call_site(),
                                    n: new_length.to_string(),
                                }),
                            }),
                        });
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the length of array types in struct fields with a random value between 1 and 10. This transformation tests the program's handling of arrays with different lengths and may expose bugs related to array indexing or bounds checking."
    }
}