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

pub struct Insert_Recursive_Type_Alias_500;

impl Mutator for Insert_Recursive_Type_Alias_500 {
    fn name(&self) -> &str {
        "Insert_Recursive_Type_Alias_500"
    }
    fn mutate(&self, file: &mut syn::File) {
        let type_alias: Item = parse_quote! {
            type RecursiveType = Vec<RecursiveType>;
        };

        file.items.push(type_alias);

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(type_path) = &*pat_type.ty {
                                if type_path.path.segments.last().unwrap().ident == "Vec" {
                                    pat_type.ty = Box::new(parse_quote!(RecursiveType));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type RecursiveType = Vec<RecursiveType>;` and replaces function parameter types that are `Vec` with this recursive alias. This transformation is designed to test the compiler's handling of recursive types, potentially triggering infinite recursion in type resolution, leading to ICEs or stack overflows."
    }
}