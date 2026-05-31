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

pub struct Modify_Array_Slice_Signature_23;

impl Mutator for Modify_Array_Slice_Signature_23 {
    fn name(&self) -> &str {
        "Modify_Array_Slice_Signature_23"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &*pat_type.ty {
                            if let Type::Slice(type_slice) = &*type_ref.elem {
                                // Change slice to array with specific size
                                pat_type.ty = Box::new(Type::Array(TypeArray {
                                    bracket_token: token::Bracket::default(),
                                    elem: type_slice.elem.clone(),
                                    len: Expr::Lit(ExprLit {
                                        attrs: Vec::new(),
                                        lit: syn::Lit::Int(syn::LitInt::new("5", Span::call_site())),
                                    }),
                                    semi_token: token::Semi::default(),
                                }));
                                // Add 'Default' constraint to the element type
                                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                                    where_clause.predicates.push(parse_quote!(T: Default));
                                } else {
                                    func.sig.generics.where_clause = Some(parse_quote!(where T: Default));
                                }
                            }
                        }
                    }
                }
                // Add operations to stress-test MIR optimizations
                let mut new_stmts = Vec::new();
                new_stmts.push(parse_quote! {
                    let _element = data[0].clone();
                });
                new_stmts.push(parse_quote! {
                    let transformed_data: Vec<T> = data.iter().cloned().collect();
                });
                func.block.stmts.splice(0..0, new_stmts);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function signatures with slice parameters, transforming them into fixed-size arrays and adding 'Default' constraints. It introduces element access and transformation operations to test MIR optimizations, potentially exposing bugs in handling array and slice manipulations."
    }
}