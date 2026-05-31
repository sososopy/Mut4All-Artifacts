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

pub struct Replace_Const_Generic_Function_With_Non_Const_Function_49;

impl Mutator for Replace_Const_Generic_Function_With_Non_Const_Function_49 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Function_With_Non_Const_Function_49"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for generic in &mut struct_item.generics.params {
                    if let GenericParam::Const(const_param) = generic {
                        if let Type::Path(path) = &*const_param.ty {
                            if let Some(segment) = path.path.segments.last() {
                                if segment.ident == "fn" {
                                    let non_const_function = Ident::new("non_const_function", Span::call_site());
                                    let new_path = SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: non_const_function,
                                            arguments: PathArguments::None,
                                        }]),
                                    };
                                    const_param.ty = Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: new_path,
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic functions with non-const functions in struct definitions. This transformation aims to test the compiler's handling of const evaluation and valtree compatibility by introducing non-const functions in const contexts."
    }
}