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

pub struct TAIT_Unification_Conflict_472;

impl Mutator for TAIT_Unification_Conflict_472 {
    fn name(&self) -> &str {
        "TAIT_Unification_Conflict_472"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut relevant_structs = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                for param in &item_struct.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if !type_param.bounds.is_empty() {
                            relevant_structs.push( (item_struct.ident.clone(), type_param.clone()) );
                            break;
                        }
                    }
                }
            }
        }

        struct Visitor<'a> {
            relevant_structs: &'a Vec<(Ident, syn::TypeParam)>,
        }

        impl<'a> Visitor<'a> {
            fn new(relevant_structs: &'a Vec<(Ident, syn::TypeParam)>) -> Self {
                Self { relevant_structs }
            }
        }

        impl<'a> VisitMut for Visitor<'a> {
            fn visit_expr_struct_mut(&mut self, node: &mut ExprStruct) {
                let path = &node.path;
                let last_segment = path.segments.last_mut().unwrap();
                let struct_name = &last_segment.ident;

                for (relevant_name, _type_param) in self.relevant_structs {
                    if struct_name == relevant_name {
                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                            if let Some(first_arg) = args.args.iter_mut().next() {
                                *first_arg = GenericArgument::Type(Type::Path(TypePath {
                                    qself: None,
                                    path: parse_quote!(i32),
                                }));
                            }
                        }
                        break;
                    }
                }

                syn::visit_mut::visit_expr_struct_mut(self, node);
            }
        }

        let mut visitor = Visitor::new(&relevant_structs);
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs with generic type parameters constrained by trait bounds and replaces the first such parameter's concrete instantiation with `i32`, a type that likely does not satisfy the trait. This forces the compiler to unify incompatible types, exposing potential bugs in trait resolution and TAIT inference under conflicting constraints."
    }
}